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

#include "FimexSink.h"
#include "FimexSource.h"
#include "GeneratedDataReader.h"
#include <fimex/CDM.h>
#include <fimex/NetCDF_CDMWriter.h>

namespace data_access
{
namespace fimex
{

FimexSink::FimexSink(const ProbabilityGenerationConfiguration::Sink & conf, int numberOfHours, const FimexSource & source) :
		outputFile_(conf.fileName), reader_(new GeneratedDataReader(numberOfHours, conf.parameterSpecificationFile, source.getReader()))
{
}

FimexSink::~FimexSink()
{
}

void FimexSink::write(WeatherData weatherData, const type::Time & validTime, const type::BlobContainer & data)
{
	static_cast<GeneratedDataReader *>(reader_.get())->add(weatherData, validTime, data);
}

void FimexSink::flush()
{
	MetNoFimex::NetCDF_CDMWriter writer(reader_, outputFile_);
}

}
}
