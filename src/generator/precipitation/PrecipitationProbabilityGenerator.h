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

#ifndef PRECIPITATIONPROBABILITYGENERATOR_H_
#define PRECIPITATIONPROBABILITYGENERATOR_H_

#include <generator/percentile/PercentileGenerator.h>


/**
 * Generates probability values for precipitation.
 */
class PrecipitationProbabilityGenerator : public PercentileGenerator
{
public:
	/**
	 * @arg selectedSymbols selected weather symbols to use
	 * @arg all symbols to use, unsorted
	 * @arg precipitationBase all precipitation data to use, unsorted
	 */
	PrecipitationProbabilityGenerator(
			const type::BlobContainer & selectedSymbols,
			type::BlobCollection & allSymbols,
			type::BlobCollection & precipitationBase);
	~PrecipitationProbabilityGenerator();

	/**
	 * Get probable precipitation values.
	 *
	 * The returned values are the median of all input values with the same
	 * weather symbol group (rain or no rain).
	 *
	 * @return a field of all probable precipitation values
	 */
	const type::BlobContainer & getPrecipitation() const;

private:
	const type::BlobContainer & selectedSymbols_;
	type::BlobCollection & allSymbols_;

	type::BlobContainer precip_;
};

#endif /* PRECIPITATIONPROBABILITYGENERATOR_H_ */
