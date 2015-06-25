/*
 probabilityForecast

 Copyright (C) 2009 met.no

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


#ifndef PROBABILITYABOVETRESHOLD_H_
#define PROBABILITYABOVETRESHOLD_H_

#include <functional>
#include <algorithm>
#include <iterator>

template <typename InIter>
double chanceOfValueAbove(InIter first, InIter last, typename InIter::value_type value)
{
	double count = std::count_if(first, last, std::bind2nd(std::greater<typename InIter::value_type>(), value));
	return (count / std::distance(first, last)) * 100;
}

#endif /* PROBABILITYABOVETRESHOLD_H_ */
