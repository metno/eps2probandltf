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


#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <dataTypes.h>
#include <generator/symbol/weatherSymbolProbability.h>

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
		Cmp cmp);

/**
 * Generic algorithm for selecting a probable single value for dataIn, based
 * also on what weather symbols have been selected.
 */
template <class Collection, class Container>
void getProbableValues(
		const type::BlobContainer & selectedSymbols,
		type::BlobCollection & allSymbols,
		Collection & dataIn,
		Container * dataOut);

#include "bits/algorithm.t.h"

#endif /* ALGORITHM_H_ */
