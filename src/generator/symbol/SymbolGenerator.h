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

#ifndef SYMBOLGENERATOR_H_
#define SYMBOLGENERATOR_H_

#include <dataTypes.h>
#include "weatherSymbolProbability.h"
#include <weather_symbol/Factory.h>
#include <boost/static_assert.hpp>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <map>


namespace weatherSymbol
{

extern const weather_symbol::Factory weatherSymbolFactory;

/**
 * Based on the given cloud cover and precipitation data, select symbols for
 * each point and sample
 */
type::BlobCollectionPtr generateSymbols(type::BlobCollection & cloudCover, type::BlobCollection & precipitation);


namespace
{
bool hasPrecipitation(float weatherSymbolCodeAsFloat)
{
	return weatherSymbolFactory.interpretor()->hasPrecipitation(weather_symbol::Code(weatherSymbolCodeAsFloat));
}
}

/**
 * Select the most likely symbol from the given set.
 */
template <typename RandomAccessIterator>
weather_symbol::Code selectSymbol(RandomAccessIterator start, RandomAccessIterator stop, float precipFraction=0.5)
{
	typedef std::map<weather_symbol::Code, unsigned> SymbolProbability;
	SymbolProbability symbolProbability;

	for ( RandomAccessIterator it = start; it != stop; ++ it )
	{
		weather_symbol::Code s = (weather_symbol::Code) * it;
		++ symbolProbability[s];
	}

	bool isRain = false;
	int precipitationCount = std::count_if(start, stop, hasPrecipitation);
   int numMembers = stop - start + 1;
   if(numMembers > 0) {
      float fraction = float(precipitationCount) / numMembers;
      if (fraction >= precipFraction)
         isRain = true;
   }

	weather_symbol::Code ret = weather_symbol::Error;
	unsigned count = 0;
	for ( SymbolProbability::iterator find = symbolProbability.begin(); find != symbolProbability.end(); ++ find )
		if ( count < find->second and (hasPrecipitation(find->first) == isRain) )
		{
			ret = find->first;
			count = find->second;
		}

	return ret;
}


/**
 * Estimate probability of a given symbol
 */
typedef std::map<weather_symbol::Code, double> SymbolProbability;
template <typename RandomAccessIterator>
void getSymbolProbability(RandomAccessIterator start, RandomAccessIterator stop, SymbolProbability * out)
{
	for ( RandomAccessIterator it = start; it != stop; ++ it )
	{
		weather_symbol::Code s = (weather_symbol::Code) * it;
		++ (*out)[s];
	}

	const int elements = stop - start;
	for ( SymbolProbability::iterator it = out->begin(); it != out->end(); ++ it )
		it->second /= elements;
}

/**
 * Generate symbols for the given iterator ranges
 */
template<typename InputIterator, typename OutputIterator>
void getSymbols(
		InputIterator cloudCoverBegin, InputIterator cloudCoverEnd,
		InputIterator precipitationBegin,
		OutputIterator out)
{
	while ( cloudCoverBegin != cloudCoverEnd )
	{
		* out = (float) weatherSymbolFactory.getSymbol((* cloudCoverBegin) * 100, * precipitationBegin);
		++ cloudCoverBegin;
		++ precipitationBegin;
		++ out;
	}
}


}

#endif /* SYMBOLGENERATOR_H_ */
