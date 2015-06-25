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


#ifndef TEMPERATUREINTERNAL_H_
#define TEMPERATUREINTERNAL_H_

#include <dataTypes.h>
#include <cmath>

void getTemperatureDeviationProbability(
			float deviation,
			type::BlobCollection & baseData,
			const type::BlobContainer & selectedData,
			type::BlobContainer & probabilityOut);


template <typename ConstRandomAccessIterator>
float getTemperatureDeviationProbability(
		float acceptableDeviation,
		ConstRandomAccessIterator baseDataBegin, ConstRandomAccessIterator baseDataEnd,
		float selectedValue
		)
{
	unsigned nearCount = 0;
	for ( ConstRandomAccessIterator it = baseDataBegin; it != baseDataEnd; ++ it )
	{
		float deviation = std::abs(float(* it) - selectedValue);
		if ( deviation <=  acceptableDeviation )
			++ nearCount;
	}
	return (nearCount / float(baseDataEnd - baseDataBegin)) * 100.0;
}


#endif /* TEMPERATUREINTERNAL_H_ */
