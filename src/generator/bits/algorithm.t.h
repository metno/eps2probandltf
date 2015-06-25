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

#include <container/CrossContainer.h>
#include <generator/wind/Wind.h>
#include <generator/symbol/SymbolGenerator.h>
#include <boost/assign/list_of.hpp>
#include <set>
#include <iostream>
#include <sstream>
#include <stdexcept>


/**
 * Median value. Note that the given data set needs to be sorted for this to work.
 */
template <typename Return, typename RandomAccessIterator>
struct median : std::binary_function<RandomAccessIterator, RandomAccessIterator, Return>
{
	Return operator() (RandomAccessIterator start, RandomAccessIterator stop)
	{
	//	return median<Return>(start, stop, internal::self<float>());
		if ( start == stop )
			return * start;

		// ER DETTE RETT?
		// 2 elementer: distance = 1 - skulle ha v�rt 2

		int elementCount = stop - start;
		if ( elementCount % 2 )
			return start[elementCount / 2];
		else // no single value in middle - returning average of two center values
			return (start[elementCount / 2] + start[(elementCount / 2) -1]) / 2;
	}
};

template <typename RandomAccessIterator>
struct median<Wind,RandomAccessIterator> : std::binary_function<RandomAccessIterator, RandomAccessIterator, Wind>
{
	Wind operator() (RandomAccessIterator start, RandomAccessIterator stop)
	{
		//	return median<Return>(start, stop, internal::self<float>());
		if ( start == stop )
			return * start;

		int elementCount = stop - start;
		start += elementCount / 2;
		if ( elementCount % 2 )
			return * start;
		else // no single value in middle - returning average wind speed, and direction for lowest wind force
		{
			Wind a = * start --;
			Wind b = * start;
			double medianForce = (a.force() + b.force()) / 2.0;

			Wind ret = b.scaledTo(medianForce);

			return ret;
		}
	}
};

inline bool symbolInfluencesSelection(weather_symbol::Code selectedSymbol, weather_symbol::Code query)
{
	return weatherSymbol::getProbabilityFactor(selectedSymbol, query) != 0;
}

template <class ReturnType, class SymbolIterator, class DataIterator, typename Cmp>
ReturnType getProbableValue(
		weather_symbol::Code selectedSymbol,
		SymbolIterator allSymbolsBegin, SymbolIterator allSymbolsEnd,
		DataIterator dataBegin,
		Cmp cmp)
{
	using namespace weatherSymbol;

	std::vector<ReturnType> relevant;
	while ( allSymbolsBegin != allSymbolsEnd )
	{
		weather_symbol::Code toConsider = (weather_symbol::Code) * allSymbolsBegin;
		bool symbolIsRelevant = symbolInfluencesSelection(selectedSymbol, toConsider);
		if ( symbolIsRelevant )
		{
			const ReturnType & relevantValue = * dataBegin;
			relevant.push_back(relevantValue);
		}

		++ allSymbolsBegin;
		++ dataBegin;
	}

	if ( relevant.empty() )
		throw std::runtime_error("Internal error - no symbols in selected category");

	std::sort(relevant.begin(), relevant.end(), cmp);

	ReturnType ret = median<ReturnType, typename std::vector<ReturnType>::const_iterator>()(relevant.begin(), relevant.end());

	return ret;
}

/**
 * Generic algorithm for selecting a probable single value for dataIn, based
 * also on what weather symbols have been selected.
 */
template <class Collection, class Container, typename Cmp>
void getProbableValues(
		const type::BlobContainer & selectedSymbols,
		type::BlobCollection & allSymbols,
		Collection & dataIn,
		Container * dataOut,
		Cmp cmp)
{
	typedef typename Container::value_type Data;

	unsigned size = allSymbols[0].size();
	for ( unsigned i = 0; i < size; ++ i )
	{
		CrossContainer<float> pSymbol(allSymbols, i);
		CrossContainer<Data> pdata(dataIn, i);

		if ( pSymbol.size() != pdata.size() )
		{
			std::ostringstream errMsg;
			errMsg << "Symbols size: " << pSymbol.size() << " != " << pdata.size() << "(base data size)";
			throw std::runtime_error(errMsg.str());
		}

		Data d = getProbableValue<Data>(
				weather_symbol::Code(selectedSymbols.at(i)),
				pSymbol.begin(), pSymbol.end(), pdata.begin(), cmp);
		dataOut->push_back(d);
	}

}

template <class Collection, class Container>
void getProbableValues(
		const type::BlobContainer & selectedSymbols,
		type::BlobCollection & allSymbols,
		Collection & dataIn,
		Container * dataOut)
{
	getProbableValues(selectedSymbols, allSymbols, dataIn, dataOut, std::less<typename Container::value_type>() );
}
