/*
 probabilityForecast

 Copyright (C) 2008 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: wdb@met.no

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

#include "TemperatureProbabilityGenerator.h"
#include "temperatureInternal.h"
#include <generator/algorithm.h>
#include <stdexcept>
#include <iterator>

using namespace std;

// Persentilene 10, 25, 75 og 90 for temperatur og d�gnlig nedb�r defineres
// ved henholdsvis de sorterte ensemblemedlemmene nr. 5, 13, 39 og 46.


TemperatureProbabilityGenerator::TemperatureProbabilityGenerator(
		const type::BlobContainer & selectedSymbols,
		type::BlobCollection & allSymbols,
		type::BlobCollection & temperatureBase)
: PercentileGenerator(temperatureBase), selectedSymbols_(selectedSymbols), allSymbols_(allSymbols)
{
	getProbableValues(selectedSymbols_, allSymbols_, baseData_, & temperature_);
}

TemperatureProbabilityGenerator::~TemperatureProbabilityGenerator()
{
}

const type::BlobContainer & TemperatureProbabilityGenerator::getTemperature() const
{
	return temperature_;
}

void TemperatureProbabilityGenerator::getTemperatureProbability1p5(
		type::BlobContainer & out)
{
	getTemperatureDeviationProbability(1.5, baseData_, temperature_, out);
}

void TemperatureProbabilityGenerator::getTemperatureProbability2p5(
		type::BlobContainer & out)
{
	getTemperatureDeviationProbability(2.5, baseData_, temperature_, out);
}
