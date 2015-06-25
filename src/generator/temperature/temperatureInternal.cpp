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

#include "temperatureInternal.h"
#include <container/CrossContainer.h>
#include <container/ccAlgorithm.h>
#include <generator/algorithm.h>
#include <functional>

#include <iostream>

void getTemperatureDeviationProbability(
			float deviation,
			type::BlobCollection & baseData,
			const type::BlobContainer & selectedData,
			type::BlobContainer & probabilityOut)
{
	probabilityOut.reserve(baseData[0].size());
	for ( unsigned i = 0; i < baseData[0].size(); ++i )
    {
		CrossContainer<float> point(baseData, i);
        float probability = getTemperatureDeviationProbability(deviation, point.begin(), point.end(), selectedData[i]);
        probabilityOut.push_back(probability);
    }
}
