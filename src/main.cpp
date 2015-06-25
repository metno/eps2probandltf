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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "generator/ProbabilityGeneratorControl.h"
#include <data_access/fimex/FimexSource.h>
#include <data_access/fimex/FimexSink.h>
#include <configuration/ProbabilityGenerationConfiguration.h>
#include "localtime.h"
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/rollingfileappender.h>
#include <log4cxx/patternlayout.h>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <map>
#include <vector>

using namespace std;


int main(int argc, char ** argv)
{
	ProbabilityGenerationConfiguration conf(argc, argv);

	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("pgen_probability"));
	logger->setLevel(log4cxx::Level::toLevel(boost::to_upper_copy(conf.logging().logLevel), log4cxx::Level::getAll()));
	if ( conf.logging().logFile.empty() )
		log4cxx::BasicConfigurator::configure();
	else
		logger->addAppender(new log4cxx::RollingFileAppender(new log4cxx::PatternLayout("%p %d: %m\n"), "test.log"));

	LOG4CXX_DEBUG(logger, "Starting probabilityForecast");

    try
	{
		{
			data_access::fimex::FimexSource source(conf.source());
			data_access::fimex::FimexSink sink(conf.sink(), conf.control().numberOfHoursToGenerate, source);

			ProbabilityGeneratorControl generator(source, sink, conf.control());
			generator.work();

			sink.flush();
		}
	}
	catch ( std::exception & e )
	{
		LOG4CXX_FATAL(logger, e.what());
		return 1;
	}
	LOG4CXX_DEBUG(logger, "Stopping probabilityForecast");
}
