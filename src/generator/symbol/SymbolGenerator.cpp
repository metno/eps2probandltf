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

#include "SymbolGenerator.h"
#include <container/CrossContainer.h>

namespace weatherSymbol
{
const weather_symbol::Factory weatherSymbolFactory(6);


/**
 * Generate symbols, based on the given cloud- andprecipitation data
 */
type::BlobCollectionPtr generateSymbols(type::BlobCollection & cloudCover, type::BlobCollection & precipitation)
{
	// Return value:
	type::BlobCollectionPtr ret = type::getBlobCollection(cloudCover.size(), cloudCover[0].size());

	(new type::BlobCollection(cloudCover.size()));
	for( type::BlobCollection::iterator it = ret->begin(); it != ret->end(); ++ it )
		it->resize(cloudCover[0].size());

	for ( unsigned i = 0; i < cloudCover[0].size(); ++ i )
	{
		CrossContainer<float> cloud(cloudCover, i);
		CrossContainer<float> precip(precipitation, i);
		CrossContainer<float> symbols(* ret, i);

		for ( unsigned j = 0; j < cloud.size(); ++ j )
		{
			weather_symbol::Code code = weatherSymbolFactory.getSymbol(cloud[j] * 100, precip[j]);
			symbols[j] = float(code);
		}

	}

	return ret;
}

}
