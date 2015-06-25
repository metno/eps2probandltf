/*
    pgen-probability

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


#ifndef SOURCE_H_
#define SOURCE_H_

#include <WeatherData.h>
#include <dataTypes.h>
#include <boost/noncopyable.hpp>
#include <exception>

namespace data_access
{

class Source : boost::noncopyable
{
public:
	virtual ~Source();

	virtual type::TimeListPtr getValidTimes() =0;

	virtual type::BlobCollectionPtr getData(WeatherData weatherData, const type::Time & t) =0;

	class DataNotAvailable : public std::exception
	{
	public:
		explicit DataNotAvailable(const std::string & itemName);
		~DataNotAvailable() throw();

		virtual const char* what() const throw();

	private:
		std::string message_;
	};
};

}

#endif /* SOURCE_H_ */
