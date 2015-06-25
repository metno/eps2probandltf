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


#ifndef WINDINTERNAL_H_
#define WINDINTERNAL_H_

#include <container/CrossContainer.h>
#include "Wind.h"

/**
 * Get a probability estimate of how certiain the given wind force is.
 *
 * @param point sample data
 * @param wind the selected data
 * @return probability, in range [0-1]
 */
float getForceProbability(CrossContainer<Wind> & point, const Wind & wind);

/**
 * Get a probability estimate of how certain the wind direction is.
 *
 * @param point sample data
 * @param wind the selected data
 * @return probability, in range [0-1]
 */
float getDirectionProbability(CrossContainer<Wind> & point, const Wind & wind);

/**
 * Get a probability estimate of how certain the wind data is.
 *
 * @param point sample data
 * @param wind the selected data
 * @return probability, in range [0-1]
 */
float getOverallProbability(CrossContainer<Wind> & point, const Wind & wind);

/**
 * Get a probability estimate of how certain the wind data is, for an entire field.
 *
 * @param selectedWind the selected data
 * @param windSamples all data sets
 * @param probabilityOut return value
 */
void getWindProbability(
		const type::WindContainer & selectedWind,
		type::WindCollection & windSamples,
		type::BlobContainer * probabilityOut);

#endif /* WINDINTERNAL_H_ */
