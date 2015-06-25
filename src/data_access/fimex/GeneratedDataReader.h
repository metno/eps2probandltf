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

#ifndef GENERATEDDATAREADER_H_
#define GENERATEDDATAREADER_H_

#include <fimex/CDMReader.h>
#include "ParameterTranslation.h"
#include <dataTypes.h>
#include <WeatherData.h>
#include <set>

namespace MetNoFimex
{
class CDM;
class CDMDimension;
class CDMVariable;
}



namespace data_access
{
namespace fimex
{

class GeneratedDataReader: public MetNoFimex::CDMReader
{
public:
	GeneratedDataReader(int wantedNumberOfHours, const std::string & translationFile, const boost::shared_ptr<MetNoFimex::CDMReader> & sourceReader);
	virtual ~GeneratedDataReader();

	void add(WeatherData weatherData, const type::Time & workingTime, const type::BlobContainer & data);

	virtual boost::shared_ptr<MetNoFimex::Data> getDataSlice(const std::string& varName, size_t unLimDimPos);

private:
	bool shouldTransferFromSourceToSink_(const std::string & variable) const;
	int getValidTimeIndex_(const type::Time & validTime) const;
	unsigned getOutputBufferIndex_(const FimexDataSpecification & parameter);

	boost::shared_ptr<MetNoFimex::CDMReader> sourceReader_;

	ParameterTranslation translation_;
	std::set<std::string> parameters_;

	int wantedNumberOfTimeSteps_;
	std::string projectionVariable_;

	std::string xDimension_;
	std::string yDimension_;

	typedef std::vector<boost::shared_ptr<MetNoFimex::Data> > DataList;
	typedef	std::map<std::string, DataList> DataMap;
	DataMap data_;
};

}
}

#endif /* GENERATEDDATAREADER_H_ */
