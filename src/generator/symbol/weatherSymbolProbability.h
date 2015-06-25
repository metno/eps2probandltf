/*
 probabilityForecast

 Copyright (C) 2008 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: probabilityForecast@met.no

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


#ifndef WEATHERSYMBOLPROBABILITY_H_
#define WEATHERSYMBOLPROBABILITY_H_

#include <algorithm>
#include <weather_symbol/Code.h>

namespace weatherSymbol
{

/**
 * How much does two symbols influence each other when it comes to calculating
 * probabilities?
 */
float getProbabilityFactor(weather_symbol::Code base, weather_symbol::Code selected);

/**
 * How much does two symbols influence each other when it comes to calculating
 * probabilities?
 */
template <typename RandomAccessIterator>
float getProbabilityFactor(RandomAccessIterator start, RandomAccessIterator stop, weather_symbol::Code toShow, weather_symbol::Code chanceOf)
{
	float elements = stop - start;
	unsigned elementCount = std::count(start, stop, chanceOf);
	float ret = ((elementCount / elements) * getProbabilityFactor(toShow, chanceOf)) * 100.0;
	return ret;
}

/**
 * Get the probability of a given symbol, for the given samples
 */
template <typename RandomAccessIterator>
float getProbabilityOf(RandomAccessIterator start, RandomAccessIterator stop, weather_symbol::Code s)
{
	float sum = 0;
	sum += getProbabilityFactor(start, stop, s, weather_symbol::Sun);
	sum += getProbabilityFactor(start, stop, s, weather_symbol::LightCloud);
	sum += getProbabilityFactor(start, stop, s, weather_symbol::PartlyCloud);
	sum += getProbabilityFactor(start, stop, s, weather_symbol::Cloud);
	sum += getProbabilityFactor(start, stop, s, weather_symbol::LightRainSun);
	sum += getProbabilityFactor(start, stop, s, weather_symbol::LightRain);
	sum += getProbabilityFactor(start, stop, s, weather_symbol::Rain);
	return sum;
}

}

#endif /* WEATHERSYMBOLPROBABILITY_H_ */
