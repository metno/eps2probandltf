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

template <typename T>
CrossContainer<T>::CrossContainer(typename CrossContainer<T>::Container & collection, int useIndex)
	: collection_(collection), useIndex_(useIndex)
{}

template <typename T>
T & CrossContainer<T>::operator [] (int index)
{
	return collection_.at(index).at(useIndex_);
}

template <typename T>
const T & CrossContainer<T>::operator [] (int index) const
{
	return collection_.at(index).at(useIndex_);
}

template <typename T, typename R>
CrossContainerIteratorBase<T,R>::CrossContainerIteratorBase(typename CrossContainer<T>::Container & toIterateOver,
		int index, int iterateIndex)
	: toIterate_(& toIterateOver)
	, constIndex_(index)
	, toIterateIndex_(iterateIndex)
{}

template <typename T, typename R>
CrossContainerIteratorBase<T,R> CrossContainerIteratorBase<T,R>::operator ++ (int)
{
	CrossContainerIteratorBase<T,R> ret(*this);
	++ toIterateIndex_;
	return ret;
}
template <typename T, typename R>
CrossContainerIteratorBase<T,R> & CrossContainerIteratorBase<T,R>::operator ++ ()
{
	++ toIterateIndex_;
	return * this;
}

template <typename T, typename R>
CrossContainerIteratorBase<T,R> CrossContainerIteratorBase<T,R>::operator -- (int)
{
	CrossContainerIteratorBase<T,R> ret(*this);
	-- toIterateIndex_;
	return ret;
}
template <typename T, typename R>
CrossContainerIteratorBase<T,R> & CrossContainerIteratorBase<T,R>::operator -- ()
{
	-- toIterateIndex_;
	return * this;
}
template <typename T, typename R>
R& CrossContainerIteratorBase<T,R>::operator * ()
{
	return toIterate_->at(toIterateIndex_).at(constIndex_);
}

template <typename T, typename R>
R& CrossContainerIteratorBase<T,R>::operator [] ( int index )
{
	return (*toIterate_).at(toIterateIndex_ + index).at(constIndex_);
}


template <typename T, typename R>
int CrossContainerIteratorBase<T,R>::operator - (const CrossContainerIteratorBase<T,R> & other) const
{
	return
	(toIterateIndex_ - other.toIterateIndex_) +
	(toIterate_ - other.toIterate_);
}

template <typename T, typename R>
CrossContainerIteratorBase<T,R> CrossContainerIteratorBase<T,R>::operator + (int amount) const
{
	CrossContainerIteratorBase<T,R> ret(*this);
	ret += amount;
	return ret;
}

template <typename T, typename R>
bool CrossContainerIteratorBase<T,R>::operator == (const CrossContainerIteratorBase<T,R> & other) const
{
	return constIndex_ == other.constIndex_
	and toIterateIndex_ == other.toIterateIndex_
	and toIterate_ == other.toIterate_;
}

template <typename T, typename R>
bool CrossContainerIteratorBase<T,R>::operator != (const CrossContainerIteratorBase<T,R> & other) const
{
	return ! (*this == other);
}

template <typename T, typename R>
bool CrossContainerIteratorBase<T,R>::operator < (const CrossContainerIteratorBase & other) const
{
	if ( toIterate_ != other.toIterate_)
		return toIterate_ < other.toIterate_;
	if ( constIndex_ != other.constIndex_ )
		return constIndex_ < other.constIndex_;
	return toIterateIndex_ < other.toIterateIndex_;
}

template <typename T>
typename CrossContainer<T>::iterator CrossContainer<T>::begin()
{
	return iterator(collection_, useIndex_, 0);
}

template <typename T>
typename CrossContainer<T>::iterator CrossContainer<T>::end()
{
	return iterator(collection_, useIndex_, collection_.size());
}

template <typename T>
typename CrossContainer<T>::const_iterator CrossContainer<T>::begin() const
{
	return const_iterator(collection_, useIndex_, 0);
}

template <typename T>
typename CrossContainer<T>::const_iterator CrossContainer<T>::end() const
{
	return const_iterator(collection_, useIndex_, collection_.size());
}
