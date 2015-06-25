/*
    pgen_probability

    Copyright (C) 2011 met.no

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


#ifndef SINK_H_
#define SINK_H_

#include <dataTypes.h>
#include <WeatherData.h>
#include <boost/noncopyable.hpp>

namespace data_access
{


class Sink : boost::noncopyable
{
public:
	virtual ~Sink() {}

	virtual void write(WeatherData weatherData, const type::Time & validTime, const type::BlobContainer & data) =0;

	virtual void flush() =0;
};

}



#endif /* SINK_H_ */
