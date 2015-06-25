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


#ifndef GENERATEWIND_H_
#define GENERATEWIND_H_

#include "Wind.h"
#include <dataTypes.h>

/**
 * Get likely wind, along with a probability estimate
 *
 * @param selectedSymbols weather symbols to present
 * @param allSymbols all weather symbol samples
 * @param wind all wind samples
 * @param windUOut Return parameter - U component of wind
 * @param windVOut Return parameter - V component of wind
 * @param windProbabilityOut Return parameter - probability estimate.
 */
void getWindProbabilities(
		const type::BlobContainer & selectedSymbols,
		type::BlobCollection & allSymbols,
		type::WindCollection & wind,
		type::BlobContainer * windUOut,
		type::BlobContainer * windVOut,
		type::BlobContainer * windProbabilityOut);


#endif /* GENERATEWIND_H_ */
