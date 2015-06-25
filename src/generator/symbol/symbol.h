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


#ifndef SYMBOL_H_
#define SYMBOL_H_

#include <dataTypes.h>

namespace weatherSymbol
{
/**
 * Select a symbol for each point, along with its probability. Also get all
 * symbols for the given cloud cover and precipitation.
 *
 * @todo make this into two separate methods
 *
 * @param cloudCover Cloud cover samples
 * @param precipitation Precipitation samples
 * @param symbolOut Selected symbol for each point
 * @param certaintyOut Selected symbol certainty for each point
 * @param precipFraction Fraction of samples that must have precip to get precip in symbol
 */
type::BlobCollectionPtr getSymbolCertainty(
		type::BlobCollection & cloudCover,
		type::BlobCollection & precipitation,
		type::BlobContainer * symbolOut,
		type::BlobContainer * certaintyOut,
      float precipFraction=0.5);
}

#endif /* SYMBOL_H_ */
