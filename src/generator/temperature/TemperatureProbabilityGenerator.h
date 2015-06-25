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

#ifndef TemperatureProbabilityGenerator_H_
#define TemperatureProbabilityGenerator_H_

#include <generator/percentile/PercentileGenerator.h>

/**
 * Generates probability values related to temperature
 */
class TemperatureProbabilityGenerator : public PercentileGenerator
{
public:
	/**
	 * @arg selectedSymbols selected weather symbols to use
	 * @arg all symbols to use, unsorted
	 * @arg precipitationBase all precipitation data to use, unsorted
	 */
	TemperatureProbabilityGenerator(
			const type::BlobContainer & selectedSymbols,
			type::BlobCollection & allSymbols,
			type::BlobCollection & temperatureBase
			);
	~TemperatureProbabilityGenerator();

	/**
	 * Get probable temperature values.
	 *
	 * The returned values are the median of all input values with the same
	 * weather symbol group (rain or no rain).
	 */
	const type::BlobContainer & getTemperature() const;

	/**
	 * Probability estimate of temperature being within +/- 1.5 degrees of selected value.
	 *
	 * Calculation is made by counting all samples within the ensemble,
	 * regardless of weather symbol
	 */
	void getTemperatureProbability1p5(type::BlobContainer & out);

	/**
	 * Probability estimate of temperature being within +/- 2.5 degrees of selected value.
	 *
	 * Calculation is made by counting all samples within the ensemble,
	 * regardless of weather symbol
	 */
	void getTemperatureProbability2p5(type::BlobContainer & out);

private:
	const type::BlobContainer & selectedSymbols_;
	type::BlobCollection & allSymbols_;

	type::BlobContainer temperature_;
};

#endif /* TemperatureProbabilityGenerator_H_ */
