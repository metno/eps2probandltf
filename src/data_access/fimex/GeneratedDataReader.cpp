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

#include "GeneratedDataReader.h"
#include <localtime.h>
#include <fimex/CDM.h>
#include <fimex/Data.h>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>
#include <limits>


namespace data_access
{
namespace fimex
{
using namespace ::MetNoFimex;


GeneratedDataReader::GeneratedDataReader(int wantedNumberOfHours, const std::string & translation, const boost::shared_ptr<MetNoFimex::CDMReader> & sourceReader) :
		sourceReader_(sourceReader), translation_(translation)
{
	BOOST_FOREACH(const CDMAttribute & attr, translation_.globalAttributes())
		cdm_->addAttribute(CDM::globalAttributeNS(), attr);

	const CDM & sourceCdm = sourceReader_->getCDM();

	// Transfer some variables from source to sink
	std::set<std::string> dimensions;
	BOOST_FOREACH( const CDMVariable & variable, sourceCdm.getVariables() )
	{
		const std::string & variableName = variable.getName();

		if ( variableName.substr(0, 11) == "projection_" )
			projectionVariable_ = variableName;

		if ( shouldTransferFromSourceToSink_(variableName) )
		{
			BOOST_FOREACH( const std::string & dimension, variable.getShape() )
			{
				if ( dimensions.find(dimension) == dimensions.end() )
				{
					CDMDimension dim = sourceCdm.getDimension(dimension);
					if ( dim.isUnlimited() )
					{
						size_t originalLength = dim.getLength();
						boost::shared_ptr<Data> d = sourceReader_->getData(dim.getName());

						boost::shared_array<double> times = d->asDouble();
						double lastTime = times[0] + (60 * 60 * wantedNumberOfHours);

						int count = 0;
						for ( int i = 0; i < d->size(); ++ i )
							if ( times[i] <= lastTime )
								++ count;

						wantedNumberOfTimeSteps_ = std::min(originalLength, (size_t) count);
						dim.setLength(wantedNumberOfTimeSteps_);
					}
					cdm_->addDimension(dim);
				}
				dimensions.insert(dimension);
			}
			cdm_->addVariable(variable);
			BOOST_FOREACH( const CDMAttribute & attribute, sourceCdm.getAttributes(variableName) )
				cdm_->addAttribute(variableName, attribute);
		}
	}

	// Add extra dimensions from configuration
	const ParameterTranslation::DimensionList & dimensionList = translation_.dimensionValues();
	BOOST_FOREACH(const ParameterTranslation::DimensionList::value_type & dim, dimensionList)
		cdm_->addDimension(CDMDimension(dim.first, dim.second.size()));
	// Create extra dimension variables, and add attributes
	BOOST_FOREACH(const ParameterTranslation::DimensionVariableList::value_type & dim, translation_.dimensionVariables())
	{
		cdm_->addVariable(CDMVariable(dim.first, CDM_FLOAT, std::vector<std::string>(1, dim.first)));
		BOOST_FOREACH(const CDMAttribute & attr, dim.second)
			cdm_->addAttribute(dim.first, attr);
	}

	BOOST_FOREACH( const ParameterTranslation::VariableList::value_type & var, translation_.extraVariables() )
	{
		cdm_->addVariable(var.first);
		BOOST_FOREACH( const MetNoFimex::CDMAttribute & attribute, var.second )
			cdm_->addAttribute(var.first.getName(), attribute);
	}


	/**
	 * Find spatial dimensions for data
	 */
	for ( int d = 0; d < int(MAX_WEATHER_DATA_ENTRIES_); ++d  )
	{
		const FimexDataSpecification & parameter = translation_.get(WeatherData(d));
		const std::string & cfName = parameter.cfName();

		xDimension_ = sourceCdm.getHorizontalXAxis(cfName);
		yDimension_ = sourceCdm.getHorizontalYAxis(cfName);
		if ( not xDimension_.empty() and not yDimension_.empty() )
			break;
		// Error checking is done when writing data
	}
	if ( xDimension_.empty() or yDimension_.empty() )
		throw std::runtime_error("Unable to find spatial dimensions for data");
}

GeneratedDataReader::~GeneratedDataReader()
{
}

void GeneratedDataReader::add(WeatherData weatherData, const type::Time & workingTime, const type::BlobContainer & data)
{
	const FimexDataSpecification & parameter = translation_.get(weatherData);
	const std::string & cfName = parameter.cfName();

	DataList & binaryData = data_[cfName];
	binaryData.resize(wantedNumberOfTimeSteps_);

	int idx = getValidTimeIndex_(parameter.getParametersValidTime(workingTime));
	if ( idx >= binaryData.size() )
		return;

	// Finding the data storage
	boost::shared_ptr<MetNoFimex::Data> & cdmData = binaryData[idx];
	if ( ! cdmData )
	{
		// Create it if it has not happened yet
		int dimensionSize = data.size();
		if ( parameter.hasExtraDimension() )
		{
			const CDMDimension & dim = cdm_->getDimension(parameter.extraDimensionName());
			dimensionSize *= dim.getLength();
		}
		cdmData = MetNoFimex::createData(CDM_FLOAT, dimensionSize);
	}
	unsigned bufferIndex = getOutputBufferIndex_(parameter) * data.size(); // Find correct place in data storage
	// ...and copy in data
	std::copy(data.begin(), data.end(), cdmData->asFloat().get() + bufferIndex);

	// Register variable if it has not happened yet
	if ( parameters_.find(cfName) == parameters_.end() )
	{
		std::vector<std::string> shape;
		shape.push_back(xDimension_);
		shape.push_back(yDimension_);
		if ( parameter.hasExtraDimension() )
			shape.push_back(parameter.extraDimensionName());
		shape.push_back("time");
		CDMVariable variable(cfName, CDM_FLOAT, shape);
		cdm_->addVariable(variable);
		BOOST_FOREACH( const MetNoFimex::CDMAttribute & attribute, parameter.attributes() )
			cdm_->addAttribute(cfName, attribute);
		cdm_->addAttribute(cfName, MetNoFimex::CDMAttribute("grid_mapping", projectionVariable_));

		parameters_.insert(cfName);
	}
}

namespace
{
size_t getSliceSize(const CDM& cdm, const CDMVariable& variable) {
	size_t sliceSize = 1;
	std::vector<std::string> shape = variable.getShape();

	for (std::vector<std::string>::const_iterator dimIt = shape.begin(); dimIt != shape.end(); ++dimIt) {
	    const CDMDimension& dim = cdm.getDimension(*dimIt);
	    if (!dim.isUnlimited()) {
	        sliceSize *= dim.getLength();
	    }
	}
	return sliceSize;
}
}

boost::shared_ptr<MetNoFimex::Data> GeneratedDataReader::getDataSlice(const std::string& varName, size_t unLimDimPos)
{
	if ( shouldTransferFromSourceToSink_(varName) )
		return sourceReader_->getDataSlice(varName, unLimDimPos);

	DataMap::const_iterator findParameter = data_.find(varName);
	if ( findParameter != data_.end() )
	{
		boost::shared_ptr<MetNoFimex::Data> ret = findParameter->second.at(unLimDimPos);
		if ( ! ret )
			ret = MetNoFimex::createData(CDM_FLOAT,
					getSliceSize(* cdm_, cdm_->getVariable(varName)), -32767);
					//std::numeric_limits<float>::quiet_NaN());
		return ret;
	}

	const ParameterTranslation::DimensionList::const_iterator extraDim = translation_.dimensionValues().find(varName);
	if ( extraDim != translation_.dimensionValues().end() )
	{
		boost::shared_ptr<MetNoFimex::Data> ret = MetNoFimex::createData(CDM_FLOAT, extraDim->second.size());
		boost::shared_array<float> data = ret->asFloat();
		std::copy(extraDim->second.begin(), extraDim->second.end(), data.get());
		return ret;
	}

	throw std::runtime_error(varName + ": no such variable");
}


bool GeneratedDataReader::shouldTransferFromSourceToSink_(const std::string & variable) const
{
	const std::set<std::string> & transferVariables = translation_.directTransferVariables();

	if ( transferVariables.find(variable) != transferVariables.end() )
		return true;

	if ( variable == projectionVariable_ )
		return true;


	const CDM::DimVec & dimensions = sourceReader_->getCDM().getDimensions();
	for ( CDM::DimVec::const_iterator it = dimensions.begin(); it != dimensions.end(); ++ it )
		if ( it->getName() == variable )
			return true;

//	CDMAttribute axisAttribute;
//	if ( sourceReader_->getCDM().getAttribute(variable, "axis", axisAttribute) )
//	{
//		const std::string & axis = axisAttribute.getStringValue();
//		if ( axis == "X" or axis == "Y" or axis == "T" )
//			return true;
//	}
	return false;
}

int GeneratedDataReader::getValidTimeIndex_(const type::Time & validTime) const
{
	double wantedTime = (double) get_seconds_since_epoch(validTime);

	boost::shared_ptr<MetNoFimex::Data> timesData = sourceReader_->getData("time");
	boost::shared_array<double> times = timesData->asDouble();

	int size = timesData->size();
	for ( int i = 0; i < size; ++ i )
		if ( times[i] == wantedTime )
			return i;

	throw std::runtime_error(string_from_local_date_time(validTime) + ": unable to find in index");
}

unsigned GeneratedDataReader::getOutputBufferIndex_(const FimexDataSpecification & parameter)
{
	if ( parameter.hasExtraDimension() )
	{
		const std::set<float> & dimensionValues = translation_.dimensionValues(parameter.extraDimensionName());

		std::set<float>::const_iterator find = dimensionValues.find(parameter.extraDimensionValue());
		if ( find == dimensionValues.end() )
		{
			std::ostringstream errorMessage;
			errorMessage << "Unable to find value " << parameter.extraDimensionValue() << " for dimension " << parameter.extraDimensionName();
			throw std::runtime_error(errorMessage.str());
		}
		return std::distance(dimensionValues.begin(), find);
	}
	return 0;
}

}
}
