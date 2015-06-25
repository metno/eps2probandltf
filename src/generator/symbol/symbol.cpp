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

#include "symbol.h"
#include "SymbolGenerator.h"
#include <dataTypes.h>
#include <container/CrossContainer.h>


namespace weatherSymbol
{

type::BlobCollectionPtr getSymbolCertainty(type::BlobCollection & cloudCover, type::BlobCollection & precipitation,
		type::BlobContainer * symbolOut, type::BlobContainer * certaintyOut, float precipFraction)
{
	symbolOut->reserve(cloudCover[0].size());
	certaintyOut->reserve(cloudCover[0].size());

	type::BlobCollectionPtr symbols = type::getEqualSizeBlobCollection(cloudCover);

	for ( unsigned i = 0; i < cloudCover[0].size(); ++ i )
	{
		CrossContainer<float> cloud(cloudCover, i);
		CrossContainer<float> precip(precipitation, i);
		CrossContainer<float> cc(* symbols, i);
		getSymbols(cloud.begin(), cloud.end(), precip.begin(), cc.begin());

		weather_symbol::Code s = selectSymbol(cc.begin(), cc.end(), precipFraction);

		symbolOut->push_back(s);
		float probability = getProbabilityOf(cc.begin(), cc.end(), s);
		certaintyOut->push_back(probability);
	}

	return symbols;
}

}
