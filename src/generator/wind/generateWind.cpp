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

#include "generateWind.h"
#include "windInternal.h"
#include <generator/algorithm.h>

void getWindProbabilities(
		const type::BlobContainer & selectedSymbols,
		type::BlobCollection & allSymbols,
		type::WindCollection & wind,
		type::BlobContainer * windUOut,
		type::BlobContainer * windVOut,
		type::BlobContainer * windProbabilityOut)
{
	type::WindContainer selectedWind;

	getProbableValues(selectedSymbols, allSymbols, wind, & selectedWind, WindForceLess());

	for ( type::WindContainer::const_iterator it = selectedWind.begin(); it != selectedWind.end(); ++ it )
	{
		windUOut->push_back(it->u());
		windVOut->push_back(it->v());
	}

	getWindProbability(selectedWind, wind, windProbabilityOut);
}
