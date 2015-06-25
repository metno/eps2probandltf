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

#ifndef BASICPERCENTILEGENERATOR_H_
#define BASICPERCENTILEGENERATOR_H_

#include <vector>
#include <functional>

/**
 * Generate percentiles from a data set.
 */
template<typename T, typename SortFunction = std::less<T> >
class BasicPercentileGenerator
{
public:

	typedef T value_type ;
	typedef std::vector<value_type> Container;
	typedef std::vector<Container> Collection;

	/**
	 * @param baseData The data set to generate percentiles from.
	 */
	BasicPercentileGenerator(const Collection & baseData);
	virtual ~BasicPercentileGenerator();

	/**
	 * Get a field where each point consists of the value that separates
	 * the lower 10% from the upper 90% of all values.
	 */
	const Container & get10Percentile() const;

	/**
	 * Get a field where each point consists of the value that separates
	 * the lower 25% from the upper 75% of all values.
	 */
	const Container & get25Percentile() const;

	/**
	 * Get a field where each point consists of the value that separates
	 * the lower 50% from the upper 50% of all values.
	 */
	const Container & get50Percentile() const;


	/**
	 * Get a field where each point consists of the value that separates
	 * the lower 75% from the upper 25% of all values.
	 */
	const Container & get75Percentile() const;

	/**
	 * Get a field where each point consists of the value that separates
	 * the lower 90% from the upper 10% of all values.
	 */
	const Container & get90Percentile() const;

	/**
	 * Get the probability of total precipitation above the given amount.
	 *
	 * @param out returned values
	 * @param mm Precipitation, in millimeter
	 */
	void probabilityOfValueAbove(Container & out, float val) const;


protected:
//	const Container & getSortedIndex_(unsigned index) const;
	const Collection & getSortedData_() const;

	mutable Collection baseData_;

private:
	mutable Collection sortedData_;
};

#include "BasicPercentileGenerator.t.h"

#endif /* BASICPERCENTILEGENERATOR_H_ */
