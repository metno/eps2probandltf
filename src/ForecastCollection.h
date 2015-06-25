/*
    pgen-probability

    Copyright (C) 2009 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: post@met.no

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


#ifndef FORECASTCOLLECTION_H_
#define FORECASTCOLLECTION_H_

#include "dataTypes.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace type
{

/**
 * Convenience class to gather both regular data and reference data
 */
template<class ValueType>
class ForecastCollection
{
public:
	typedef ValueType value_type;
	typedef std::vector<value_type> ValueContainer;
	typedef boost::shared_ptr<ValueContainer> ValueContainerPtr;
	typedef std::vector<ValueContainer> ValueCollection;
	typedef boost::shared_ptr<ValueCollection> ValueCollectionPtr;

	ForecastCollection(ValueCollectionPtr data) :
		data_(data)
	{}

	virtual ~ForecastCollection() {}

	ValueCollectionPtr & data() { return data_; }
	const ValueCollectionPtr & data() const { return data_; }

private:
	ValueCollectionPtr data_;
};

class BlobForecastCollection : public ForecastCollection<BlobValue>
{
public:
	BlobForecastCollection(ValueCollectionPtr mainData) :
			ForecastCollection<BlobValue>(mainData)
	{}
};

}

#endif /* FORECASTCOLLECTION_H_ */

