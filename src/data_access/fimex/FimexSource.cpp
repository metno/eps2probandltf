/*
 pgen

 Copyright (C) 2011 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: post@met.no

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA  02110-1301, USA
 */

#include "FimexSource.h"
#include <localtime.h>
#include <fimex/CDMFileReaderFactory.h>
#include <fimex/CDMReader.h>
#include <fimex/CDM.h>
#include <fimex/Data.h>
#include <boost/foreach.hpp>
#include <stdexcept>
#include <iostream>

using namespace MetNoFimex;


namespace data_access
{
namespace fimex
{


void FimexSource::init_(const std::string & file, const std::string & configFile, FileType fileType)
{
	switch ( fileType )
	{
	case Wdb:
		reader_ = CDMFileReaderFactory::create(MIFI_FILETYPE_WDB, file, XMLInputFile(configFile));
		break;
	case Felt:
		reader_ = CDMFileReaderFactory::create(MIFI_FILETYPE_FELT, file, configFile);
		break;
	case NetCdf:
		reader_ = CDMFileReaderFactory::create(MIFI_FILETYPE_NETCDF, file, std::string());
		break;
	default:
		throw std::runtime_error("Unsupported file type");
	}
}


FimexSource::FimexSource(const ProbabilityGenerationConfiguration::Source & options) :
		translator_(options.parameterSpecificationFile)
{
	init_(options.fileName, options.configFileName, options.fileType);
}

FimexSource::~FimexSource()
{
}

type::TimeListPtr FimexSource::getValidTimes()
{
	if ( ! validTimes_ )
	{
		validTimes_ = type::TimeListPtr(new type::TimeList);

		const CDM & cdm = reader_->getCDM();
		const CDMVariable & timeVariable = cdm.getVariable(getTimeDimension_());

		const boost::shared_ptr<Data> & timeData = reader_->getData(timeVariable.getName());
		const boost::shared_array<double> & times = timeData->asDouble();

		std::size_t timeCount = timeData->size();
		for ( std::size_t i = 0; i < timeCount; ++ i )
		{
			validTimes_->push_back(get_time(times[i]));
			//std::cout << string_from_local_date_time(validTimes->back()) << std::endl;
		}
	}
	return validTimes_;
}

std::string FimexSource::getTimeDimension_() const
{
	std::string cfName = translator_.get(CloudCover).cfName();
	const CDM & cdm = reader_->getCDM();
	std::string ret = cdm.getTimeAxis(cfName);

	if ( ret.empty() )
		throw std::runtime_error("No time dimensions found");

	return ret;
}


type::BlobCollectionPtr FimexSource::getData(WeatherData weatherData, const type::Time & t)
{
	const FimexDataSpecification & dataSpec = translator_.get(weatherData);

	const MetNoFimex::CDM & cdm = reader_->getCDM();

	if ( not cdm.hasVariable(dataSpec.cfName()) )
		throw data_access::Source::DataNotAvailable(dataSpec.cfName());

	std::vector<std::string> shape = cdm.getVariable(dataSpec.cfName()).getShape();
	if ( shape.size() < 3 )
		throw std::runtime_error("Invalid shape for variable " + dataSpec.cfName());
	if ( shape[2] != "ensemble_member" and shape[2] != "realization" )
		throw std::runtime_error("missing ensemble data - got " + shape[2]);

	std::size_t gridSize = cdm.getDimension(shape[0]).getLength();
	gridSize *= cdm.getDimension(shape[1]).getLength();

	std::size_t ensembleCount = cdm.getDimension(shape[2]).getLength();

	boost::shared_array<float> data = getRawData_(dataSpec, t);
	type::BlobCollectionPtr ret = extractData(data.get(), gridSize, ensembleCount);
	scaleData_(dataSpec, * ret);

	return ret;
}

boost::shared_array<float> FimexSource::getRawData_(const FimexDataSpecification & dataSpec, const type::Time & workingTime)
{
	const type::Time & wantedTime = dataSpec.getParametersValidTime(workingTime);
	type::TimeListPtr times = getValidTimes();
	int timeIndex = 0;
	while ( times->at(timeIndex) != wantedTime )
		++ timeIndex;

	return reader_->getDataSlice(dataSpec.cfName(), timeIndex)->asFloat();

}

type::BlobCollectionPtr FimexSource::extractData(const float * rawData, int gridSize, int ensembleCount)
{
	type::BlobCollectionPtr ret(new type::BlobCollection);

	const float * start = rawData;
	const float * stop = start;
	for ( std::size_t i = 0; i < ensembleCount; ++ i )
	{
		stop += gridSize;
		ret->push_back(std::vector<float>());
		ret->back().assign(start, stop);
		start = stop;
	}
	return ret;
}

void FimexSource::scaleData_(const FimexDataSpecification & dataSpec, type::BlobCollection & blobs) const
{
	double offset = 0;
	CDMAttribute add_offset;
	if ( reader_->getCDM().getAttribute(dataSpec.cfName(), "add_offset", add_offset) )
		offset = add_offset.getData()->asDouble()[0];

	double scale = 1;
	CDMAttribute scale_factor;
	if ( reader_->getCDM().getAttribute(dataSpec.cfName(), "scale_factor", scale_factor) )
		scale = scale_factor.getData()->asDouble()[0];

	if ( offset != 0 or scale != 1 )
		BOOST_FOREACH(type::BlobContainer & blobContainer, blobs)
			BOOST_FOREACH(float & val, blobContainer)
				val = (val * scale) + offset;
}

}
}
