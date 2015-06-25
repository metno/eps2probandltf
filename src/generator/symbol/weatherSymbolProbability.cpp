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

#include "weatherSymbolProbability.h"
#include <boost/assign/list_of.hpp>
#include <set>
#include <map>
#include <stdexcept>

namespace weatherSymbol
{

float getProbabilityFactor(weather_symbol::Code base, weather_symbol::Code selected)
{
	typedef std::set<weather_symbol::Code> SymbolSet;
	typedef std::map<weather_symbol::Code, SymbolSet> NearbySymbolMap;

	static NearbySymbolMap nearby;
	if ( nearby.empty() )
	{
		using namespace weather_symbol;
		nearby[Sun] = boost::assign::list_of(Sun)(LightCloud); // 1
		nearby[LightCloud] = boost::assign::list_of(Sun)(LightCloud)(PartlyCloud); // 2
		nearby[PartlyCloud] = boost::assign::list_of(LightCloud)(PartlyCloud)(Cloud); // 3
		nearby[Cloud] = boost::assign::list_of(PartlyCloud)(Cloud); // 4
		nearby[DrizzleSun] = boost::assign::list_of(DrizzleSun)(LightRainSun)(Drizzle); // 40
		nearby[LightRainSun] = boost::assign::list_of(DrizzleSun)(LightRainSun)(RainSun)(LightRain); // 5
		nearby[RainSun] = boost::assign::list_of(LightRainSun)(RainSun)(Rain); // 41
		nearby[Drizzle] = boost::assign::list_of(DrizzleSun)(Drizzle)(LightRain); // 46
		nearby[LightRain] = boost::assign::list_of(LightRainSun)(Drizzle)(LightRain)(Rain); // 9
		nearby[Rain] = boost::assign::list_of(RainSun)(LightRain)(Rain); // 10
	}

	NearbySymbolMap::const_iterator find = nearby.find(base);
	if ( find == nearby.end() )
		throw std::logic_error("Internal error");

	const SymbolSet & s = find->second;
	if ( s.find(selected) != s.end() )
		return 1;
	return 0;
}


}
