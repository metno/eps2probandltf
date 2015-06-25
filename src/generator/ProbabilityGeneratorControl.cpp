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

#include "ProbabilityGeneratorControl.h"
#include "ProbabilityGenerator.h"
#include <data_access/Source.h>
#include <localtime.h>
#include <log4cxx/logger.h>
#include <sstream>
#include <functional>

using namespace std;


ProbabilityGeneratorControl::ProbabilityGeneratorControl(
		data_access::Source & dataSource,
		data_access::Sink & dataSink,
		const ProbabilityGenerationConfiguration::GeneratorControl & conf)
	: source_(dataSource), sink_(dataSink), conf_(conf)
{}

ProbabilityGeneratorControl::~ProbabilityGeneratorControl()
{}

void ProbabilityGeneratorControl::work()
{
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("pgen_probability.control"));

	const type::TimeListPtr validTimes = source_.getValidTimes();

	LOG4CXX_DEBUG(logger, "Found valid time list");

	if ( validTimes->empty() )
		throw std::runtime_error("No data found");

	type::Duration hoursToGenerate(conf_.numberOfHoursToGenerate,0,0,0);
	type::Time lastGenerateTime = (*validTimes)[0] + hoursToGenerate;

	for ( type::TimeList::const_iterator it = validTimes->begin(); it != validTimes->end() and * it < lastGenerateTime; ++ it )
	{
		const type::Time & time = * it;//(*validTimes)[timeStep];

		const std::string timeString = string_from_local_date_time(time);
		LOG4CXX_INFO(logger, "Starting work on valid time " + timeString);

		ProbabilityGenerator generator(source_, sink_, conf_, time );
		try
		{
			generator.work();
		}
		catch( std::out_of_range & )
		{
			LOG4CXX_WARN(logger, "Insufficient data to generate probabilities for time " + string_from_local_date_time(time));
		}
		catch( std::exception & e )
		{
		}

		LOG4CXX_INFO(logger, timeString + " done");
	}
}
