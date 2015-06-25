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

#ifndef CROSSCONTAINER_H_
#define CROSSCONTAINER_H_

#include <boost/noncopyable.hpp>
#include <vector>
#include <iterator>

template <typename T, typename R> class CrossContainerIteratorBase;

/**
 * An array of arrays providing access to the nth element in each array. The
 * value of n is given in the constructor.
 *
 * Data may be accessed by array subscript, or by using the random-access
 * iterator provided by this class.
 *
 * @bugs Creating iterators from const versions of this class does not work
 * properly
 *
 * @warning All contained containers must be of the same size, and no
 * detection of this is done.
 */
template <typename T>
class CrossContainer : boost::noncopyable
{
public:

	typedef T value_type;
	typedef std::vector<value_type> InternalContainer;
	typedef std::vector<InternalContainer> Container;

	CrossContainer(Container & collection, int useIndex);

	typedef CrossContainerIteratorBase<value_type, value_type> iterator;
	typedef CrossContainerIteratorBase<value_type, const value_type> const_iterator;

	value_type & operator [] (int index);
	const value_type & operator [] (int index) const;

	typedef typename Container::size_type size_type;
	size_type size() const { return collection_.size(); }
	bool empty() const { return collection_.empty(); }

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
private:
	Container & collection_;
	const int useIndex_;
};

template <typename T, typename R>
class CrossContainerIteratorBase : public std::iterator<std::random_access_iterator_tag, T, int>
{
public:
	CrossContainerIteratorBase()
		: toIterate_(0), constIndex_(0), toIterateIndex_(0)
	{}

	R& operator * ();
	R* operator -> () { return & operator*(); }

	operator CrossContainerIteratorBase<T, const R>()
	{
		return CrossContainerIteratorBase<T, const R>(* toIterate_, constIndex_, toIterateIndex_);
	}

	R& operator [] ( int index );
	int operator - (const CrossContainerIteratorBase<T,R> & other) const;
	CrossContainerIteratorBase operator ++ (int);
	CrossContainerIteratorBase & operator ++ ();
	CrossContainerIteratorBase operator -- (int);
	CrossContainerIteratorBase & operator -- ();
	CrossContainerIteratorBase & operator += (int amount) { toIterateIndex_ += amount; return * this; }
	CrossContainerIteratorBase operator + (int amount) const;
	CrossContainerIteratorBase & operator -= (int amount) { toIterateIndex_ -= amount; return * this; }
	CrossContainerIteratorBase operator - (int amount) const { return operator+(-amount); }

	bool operator == (const CrossContainerIteratorBase & other) const;
	bool operator != (const CrossContainerIteratorBase & other) const;
	bool operator < (const CrossContainerIteratorBase & other) const;



private:
	typedef CrossContainer<T> Container;
	CrossContainerIteratorBase(typename Container::Container & toIterateOver,
			int index, int iterateIndex);
	// these friend declarations allow us to keep constructor private
	friend class CrossContainer<T>;
	friend class CrossContainerIteratorBase<T, T>;

	typename Container::Container * toIterate_;
	int constIndex_;
	int toIterateIndex_;
};


#include "bits/CrossContainer.t.h"

#endif /* CROSSCONTAINER_H_ */
