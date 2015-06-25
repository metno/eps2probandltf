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

#include "Wind.h"
#include <cmath>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <cmath>


Wind::Wind(float u, float v)
	: u_(u), v_(v)
{}

Wind::~Wind()
{}

float Wind::force() const
{
	return std::sqrt((u_*u_) + (v_*v_));
}

Wind::Direction Wind::direction() const
{
	const double radiansToDegrees = 57.29577951308232;
	float ret = (std::atan2(v_, u_) * radiansToDegrees) /*+ 180*/;
	return ret;
}


Wind::Direction::Direction(float dir)
	: dir_(dir)
{
	while ( dir_ <= -180 )
		dir_ += 360;
	while ( 180 < dir_ )
		dir_ -= 360;
}

float Wind::Direction::differenceTo(Wind::Direction d) const
{
	float a = dir_;
	float b = float(d);
	return Wind::Direction(a - b);
}

Wind Wind::scaledTo(float windForce) const
{
	float factor = windForce / force();

	return Wind(u_ * factor, v_ * factor);
}

struct makeWind : std::binary_function<float, float, Wind>
{
	Wind operator () (float u, float v) const
	{
		return Wind(u, v);
	}
};


type::WindForecastCollection getWind(const type::BlobForecastCollection & u, const type::BlobForecastCollection & v)
{
	using namespace type;

	type::WindCollectionPtr main = getWind(* u.data(), * v.data());

	return WindForecastCollection(main);
}

type::WindCollectionPtr getWind(const type::BlobCollection & u, const type::BlobCollection & v)
{
	using namespace type;

	WindCollectionPtr ret(new WindCollection(u.size()));

	BlobCollection::const_iterator iu = u.begin();
	BlobCollection::const_iterator iv = v.begin();
	WindCollection::iterator ir = ret->begin();
	while ( iu != u.end() )
	{
		std::transform(iu->begin(), iu->end(), iv->begin(), std::back_inserter(* ir), makeWind());
		++ iu;
		++ iv;
		++ ir;
	}

	return ret;
}
