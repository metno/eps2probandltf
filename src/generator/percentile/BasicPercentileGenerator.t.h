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

//#include "BasicPercentileGenerator.h"
#include <container/CrossContainer.h>
#include <container/ccAlgorithm.h>
#include <generator/algorithms/probabilityAboveTreshold.h>
#include <functional>
#include <iterator>

template<typename T, typename SortFunction>
BasicPercentileGenerator<T, SortFunction>::BasicPercentileGenerator(const Collection & baseData)
	: baseData_(baseData)
{
}

template<typename T, typename SortFunction>
BasicPercentileGenerator<T, SortFunction>::~BasicPercentileGenerator()
{}

template<typename T, typename SortFunction>
const typename BasicPercentileGenerator<T, SortFunction>::Container & BasicPercentileGenerator<T, SortFunction>::get10Percentile() const
{
	// Gives idx 4 for size 51 ensemble
	typename BasicPercentileGenerator<T, SortFunction>::Container::size_type i = (type::BlobContainer::size_type) (baseData_.size() * 0.1) -1;
	return getSortedData_().at(i);
}

template<typename T, typename SortFunction>
const typename BasicPercentileGenerator<T, SortFunction>::Container & BasicPercentileGenerator<T, SortFunction>::get25Percentile() const
{
	// Gives idx 12 for size 51 ensemble
	typename BasicPercentileGenerator<T, SortFunction>::Container::size_type i = (typename BasicPercentileGenerator<T, SortFunction>::Container::size_type) (baseData_.size() * 0.25);
	return getSortedData_().at(i);
}

template<typename T, typename SortFunction>
const typename BasicPercentileGenerator<T, SortFunction>::Container & BasicPercentileGenerator<T, SortFunction>::get50Percentile() const
{
	typename BasicPercentileGenerator<T, SortFunction>::Container::size_type i = (typename BasicPercentileGenerator<T, SortFunction>::Container::size_type) (baseData_.size() / 2 );
	return getSortedData_().at(i);
}


template<typename T, typename SortFunction>
const typename BasicPercentileGenerator<T, SortFunction>::Container & BasicPercentileGenerator<T, SortFunction>::get75Percentile() const
{
	// Gives idx 38 for size 51 ensemble
	typename BasicPercentileGenerator<T, SortFunction>::Container::size_type i = (typename BasicPercentileGenerator<T, SortFunction>::Container::size_type) (baseData_.size() * 0.75);
	return getSortedData_().at(i);
}

template<typename T, typename SortFunction>
const typename BasicPercentileGenerator<T, SortFunction>::Container & BasicPercentileGenerator<T, SortFunction>::get90Percentile() const
{
	// Gives idx 46 for size 51 ensemble
	typename BasicPercentileGenerator<T, SortFunction>::Container::size_type i = (typename BasicPercentileGenerator<T, SortFunction>::Container::size_type) (baseData_.size() * 0.9);
	return getSortedData_().at(i);
}

template<typename T, typename SortFunction>
void BasicPercentileGenerator<T, SortFunction>::probabilityOfValueAbove(typename BasicPercentileGenerator<T, SortFunction>::Container & out, float val) const
{
	typename BasicPercentileGenerator<T, SortFunction>::Container tmpStore;

	for ( int i = 0; i < baseData_[0].size(); ++ i )
	{
		CrossContainer<float> point(baseData_, i);
		double probability = chanceOfValueAbove(point.begin(), point.end(), val);
		tmpStore.push_back(probability);
	}
	out.swap(tmpStore);
}

//const typename BasicPercentileGenerator<T, SortFunction>::Container & BasicPercentileGenerator<T, SortFunction>::getSortedIndex_(unsigned index) const
//{
//	return getSortedData_().at(index);
//}

template<typename T, typename SortFunction>
const typename BasicPercentileGenerator<T, SortFunction>::Collection & BasicPercentileGenerator<T, SortFunction>::getSortedData_() const
{
	if ( sortedData_.empty() )
	{
		sortedData_ = baseData_;
		algorithm::sort(sortedData_, SortFunction());
	}
	return sortedData_;
}
