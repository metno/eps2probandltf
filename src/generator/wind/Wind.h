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

#ifndef WIND_H_
#define WIND_H_

#include <dataTypes.h>
#include <ForecastCollection.h>
#include <cmath>
#include <ostream>


/**
 * A single representation of U- and V components of wind, along with various
 * functions to do wind math
 */
class Wind
{
public:
	Wind(float u, float v);
	~Wind();

	/// Wind direction, in degrees
	class Direction
	{
	public:
		Direction(float dir);

		float differenceTo(Direction d) const;

		operator float () const { return dir_; }
	private:
		float dir_;
	};

	/// wind force
	float force() const;

	/// wind direction, in degrees
	Direction direction() const;

	float u() const { return u_; }
	float v() const { return v_; }

	Wind scaledTo(float windForce) const;

private:
	float u_;
	float v_;
};

namespace type
{
typedef std::vector<Wind> WindContainer;
typedef boost::shared_ptr<WindContainer> WindContainerPtr;
typedef std::vector<WindContainer> WindCollection;
typedef boost::shared_ptr<WindCollection> WindCollectionPtr;

class WindForecastCollection : public ForecastCollection<Wind>
{
public:
	WindForecastCollection(ValueCollectionPtr mainData) :
			ForecastCollection<Wind>(mainData)
	{}
};
}


/// Create a collection of Wind objects, based on U- and V data.
type::WindForecastCollection getWind(const type::BlobForecastCollection & u, const type::BlobForecastCollection & v);

type::WindCollectionPtr getWind(const type::BlobCollection & u, const type::BlobCollection & v);


/**
 * Ordering wind, based on force alone
 */
struct WindForceLess
{
	bool operator () (Wind a, Wind b) const
	{
		return a.force() < b.force();
	}
};


inline bool operator == (const Wind & a, const Wind & b)
{
	return std::abs(a.u()-b.u()) < 0.0001 and std::abs(a.v()-b.v()) < 0.0001;
}

inline std::ostream & operator << (std::ostream & s, const Wind & w)
{
	return s << "Wind(" << w.u() << ',' << w.v() << ')';
}

#endif /* WIND_H_ */
