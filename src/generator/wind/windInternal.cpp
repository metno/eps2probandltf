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

#include "windInternal.h"
#include <generator/algorithm.h>
#include <container/ccAlgorithm.h>
#include <functional>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>


namespace
{
class almost_equal_force : public std::binary_function<Wind, Wind, bool>
{
public:
	almost_equal_force(float delta) : delta_(delta) {}
	bool operator () (Wind a, Wind b) const
	{
		return std::fabs(a.force() - b.force()) <= delta_;
	}
private:
	float delta_;
};

template <typename ConstRandomAccessIterator>
unsigned countAbsDifferenceForce(
		ConstRandomAccessIterator start, ConstRandomAccessIterator stop,
		Wind value, unsigned absDiff)
{
	return std::count_if(start, stop, std::bind2nd(almost_equal_force(absDiff), value));
}

class almost_equal_direction : public std::binary_function<Wind, Wind, bool>
{
public:
	almost_equal_direction(float delta) : delta_(std::fabs(delta)) {}
	bool operator () (Wind a, Wind b) const
	{
		return std::fabs(a.direction().differenceTo(b.direction())) <= delta_;

//		float aDir = a.direction();
//		float bDir = b.direction();
//		return std::fabs(aDir - bDir) <= delta_;
	}
private:
	float delta_;
};


template <typename ConstRandomAccessIterator>
unsigned countAbsDifferenceDirection(ConstRandomAccessIterator start, ConstRandomAccessIterator stop, Wind value, float absDiff)
{
	return std::count_if(start, stop, std::bind2nd(almost_equal_direction(absDiff), value));
}

}

float getForceProbability(CrossContainer<Wind> & point, const Wind & wind)
{
    unsigned deviation = wind.force() < 12 ? 2 : 3;

    float count = std::count_if(point.begin(), point.end(),
    		std::bind2nd(almost_equal_force(deviation), wind));
//    float count = countAbsDifferenceForce(point.begin(), point.end(), wind, deviation);
    return (count / point.size()) * 100.0;
}

float getDirectionProbability(CrossContainer<Wind> & point, const Wind & wind)
{
	float force = wind.force();
	if ( force < 3 )
		return 100;

    float deviation = force < 12 ? 45 : 22.5;
    float count = countAbsDifferenceDirection(point.begin(), point.end(), wind, deviation);
    return (count / point.size()) * 100.0;
}

float getOverallProbability(CrossContainer<Wind> & point, const Wind & wind)
{
	return getForceProbability(point, wind);
}


void getWindProbability(
		const type::WindContainer & selectedWind,
		type::WindCollection & windSamples,
		type::BlobContainer * probabilityOut)
{
	for ( unsigned i = 0; i < selectedWind.size(); ++ i )
	{
		CrossContainer<Wind> point(windSamples, i);
		float probability = getOverallProbability(point, selectedWind[i]);
		probabilityOut->push_back(probability);
	}
}
