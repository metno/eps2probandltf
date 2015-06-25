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


#ifndef CCALGORITHM_H_
#define CCALGORITHM_H_

#include <dataTypes.h>
#include "CrossContainer.h"
#include <algorithm>


namespace algorithm
{
template<typename T, typename SortFunction>
void sort(BLOB_COLLECTION(T) & toSort, SortFunction sortFunction = std::less<T>())
{
	if ( toSort.empty() )
		return;

	for ( unsigned i = 0; i < toSort[0].size(); ++ i )
	{
		CrossContainer<T> cc(toSort, i);
		std::sort(cc.begin(), cc.end(), sortFunction);
	}
}

/**
 * Perform action F on each element in the given BlobContainer
 */
template <class F>
void for_each(type::BlobCollection & container, F f)
{
	for ( unsigned i = 0; i < container[0].size(); ++ i )
	{
		CrossContainer<float> cc(container, i);
		std::for_each(cc.begin(), cc.end(), f);
	}
}

/**
 * similar to std::transform, but works on a blobCollection
 */
template <class F>
void transform(const type::BlobCollection & in, type::BlobCollection & out, F f)
{
	if ( in.empty() )
	{
		out.clear();
		return;
	}

	const type::BlobCollection::size_type internalSize = in[0].size();
	out.resize(in.size(), type::BlobContainer(internalSize));

	for ( unsigned i = 0; i < in[0].size(); ++ i )
	{
		const CrossContainer<float> ccIn(const_cast<type::BlobCollection &>(in), i);
		CrossContainer<float> ccOut(out, i);
		std::transform(ccIn.begin(), ccIn.end(), ccOut.begin(), f);
	}
}

/**
 * similar to std::transform, but works on a blobCollection
 */
template <class F>
void transform(const type::BlobCollection & in1, const type::BlobCollection & in2, type::BlobCollection & out, F f)
{
	if ( in1.empty() )
	{
		out.clear();
		return;
	}

	const type::BlobCollection::size_type internalSize = in1[0].size();
	out.resize(in1.size(), type::BlobContainer(internalSize));

	for ( unsigned i = 0; i < in1[0].size(); ++ i )
	{
		const CrossContainer<float> ccIn1(const_cast<type::BlobCollection &>(in1), i);
		const CrossContainer<float> ccIn2(const_cast<type::BlobCollection &>(in2), i);
		CrossContainer<float> ccOut(out, i);

		std::transform(ccIn1.begin(), ccIn1.end(), ccIn2.begin(), ccOut.begin(), f);
	}
}

/**
 * fills out with the result of having called f(in[x]), for all legal values
 * of x
 */
template <class Collection, class Container, typename F>
void agregate(Collection & in, Container & out, F f)
{
	typedef typename Container::value_type value_type;

	if ( in.empty() )
		return;

	for ( unsigned i = 0; i < in[0].size(); ++ i )
	{
		const CrossContainer<value_type> ccIn(in, i);
		out.push_back(f(ccIn.begin(), ccIn.end()));
	}
}

}



#endif /* CCALGORITHM_H_ */
