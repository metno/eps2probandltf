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

#include "localtime.h"
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/time_zone_base.hpp>
#include <boost/regex.hpp>
#include <string>
#include <sstream>

using namespace std;
using namespace boost::local_time;


namespace
{
const boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
}

type::Time get_time(long long secondsSinceEpoch)
{
	static const time_zone_ptr timeZone(new posix_time_zone("+00"));

	boost::posix_time::hours hours(secondsSinceEpoch / 3600);
	boost::posix_time::seconds seconds(secondsSinceEpoch % 3600);

	return type::Time(epoch, timeZone) + hours + seconds;
}

long long get_seconds_since_epoch(const type::Time & t)
{
	type::Time epoch_with_time_zone(epoch, t.zone());
	type::Duration timeSinceEpoch = t - epoch_with_time_zone;

	long long ret = timeSinceEpoch.hours() * 60 * 60;
	ret += timeSinceEpoch.seconds();

	return ret;
}

type::Time local_time_from_string(const std::string & localTime)
{
	string re = "^(\\d{4}-...?-\\d\\d)[ Tt](\\d\\d:\\d\\d:\\d\\d)(.*)$";
	boost::regex r(re);
	boost::smatch match;
	if ( ! boost::regex_match(localTime, match, r) )
		throw std::logic_error("Invalid syntax for time: " + localTime);

	string fullTime = match[1] + " " + match[2];
	boost::posix_time::ptime timePart = boost::posix_time::time_from_string(fullTime);
	string zonePart = match[3];
	if ( zonePart.empty() )
		zonePart = "+00";
	else if ( zonePart == "z" or zonePart == "Z" )
		zonePart = "+00";

	time_zone_ptr timeZone(new posix_time_zone(zonePart));

	timePart -= timeZone->base_utc_offset();

	type::Time t(timePart, timeZone);

	return t;
}


std::string string_from_local_date_time(const type::Time & t)
{
	ostringstream ret;
	ret << t.local_time() << t.zone()->to_posix_string();

	return ret.str();
}
