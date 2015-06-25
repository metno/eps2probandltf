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

#ifndef FIMEXSOURCE_H_
#define FIMEXSOURCE_H_

#include <data_access/Source.h>
#include "FileType.h"
#include "FimexDataSpecification.h"
#include "ParameterTranslation.h"
#include <configuration/ProbabilityGenerationConfiguration.h>
#include <boost/shared_array.hpp>
#include <string>

namespace MetNoFimex
{
class CDMReader;
}


namespace data_access
{
namespace fimex
{

class FimexSource: public Source
{
public:
	explicit FimexSource(const ProbabilityGenerationConfiguration::Source & options);
	virtual ~FimexSource();

	virtual type::TimeListPtr getValidTimes();

	virtual type::BlobCollectionPtr getData(WeatherData weatherData, const type::Time & t);

	const boost::shared_ptr<MetNoFimex::CDMReader> & getReader() const { return reader_; }

private:
	void init_(const std::string & file, const std::string & configFile, FileType fileType);

	boost::shared_array<float> getRawData_(const FimexDataSpecification & dataSpec, const type::Time & workingTime);
	type::BlobCollectionPtr extractData(const float * rawData, int gridSize, int ensembleCount);
	void scaleData_(const FimexDataSpecification & dataSpec, type::BlobCollection & blobs) const;

	std::string getTimeDimension_() const;

	boost::shared_ptr<MetNoFimex::CDMReader> reader_;
	type::TimeListPtr validTimes_;
	ParameterTranslation translator_;
};

}
}

#endif /* FIMEXSOURCE_H_ */
