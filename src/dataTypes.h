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


#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <boost/date_time/local_time/local_date_time.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace type
{

typedef boost::posix_time::time_duration Duration;

typedef boost::local_time::local_date_time Time;

typedef std::vector<Time> TimeList;

typedef boost::shared_ptr<TimeList> TimeListPtr;


typedef float BlobValue;

/// Storage for a single field from the database
typedef std::vector<BlobValue> BlobContainer;

typedef boost::shared_ptr<BlobContainer> BlobContainerPtr;

/**
 * Storage for a collection of fields from the database, eg. all ensemble
 * members for a given parameter
 */
typedef std::vector<BlobContainer> BlobCollection;

#define BLOB_CONTAINER(Type) std::vector<Type>
#define BLOB_COLLECTION(Type) std::vector<BLOB_CONTAINER(Type) >

typedef boost::shared_ptr<BlobCollection> BlobCollectionPtr;


template<class T>
struct AdjustTowardReference
{
	const float scale_;

	AdjustTowardReference(float scale) : scale_(scale) {}

	T operator () (T main, T reference)
	{
		return main + (scale_ * (reference - main));
	}
};

/// Get a BlobCollection with all field values set to zero
inline BlobCollectionPtr getBlobCollection(unsigned noOfBlobs, unsigned blobSize)
{
	BlobCollectionPtr ret(new BlobCollection(noOfBlobs));
	for( BlobCollection::iterator it = ret->begin(); it != ret->end(); ++ it )
		it->resize(blobSize);

	return ret;
}

/// Get a copy of the given BlobCollection, but with all field values set to zero
inline BlobCollectionPtr getEqualSizeBlobCollection(const BlobCollection & c)
{
	return getBlobCollection(c.size(), c[0].size());
}

/// Get the size of the contained BlobContainers
inline unsigned xSize(const BlobCollection & c)
{
	return c.at(0).size();
}

}


#endif /* DATATYPES_H_ */
