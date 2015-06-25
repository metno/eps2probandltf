/*
 pgen

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "ProbabilityGenerationConfiguration.h"
#include <boost/program_options.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <fstream>

namespace
{
std::ostream & version(std::ostream & out)
{
	return out << "eps2probandltf " << VERSION;
}

std::ostream & help(std::ostream & out, const boost::program_options::options_description & options)
{
	out << "This program calculates various probabilities, based on input for an ensemble file.";
	out << '\n';
	out << "Usage: eps2probandltf OPTIONS\n\n";
	out << "Options:\n";

	return out << options;
}

const std::string DEFAULT_PARAMETER_CONFIG = SYSCONFDIR"/eps2probandltf.parameters.xml";
const std::string DEFAULT_FELT_INPUT_CONFIG = SYSCONFDIR"/felt2nc_eps_yr.xml";
const std::string DEFAULT_OPTIONS_CONFIG = SYSCONFDIR"/eps2probandltf.conf";
}


ProbabilityGenerationConfiguration::ProbabilityGenerationConfiguration(int argc, char ** argv)
{
	using namespace boost::program_options;

	options_description options("Allowed options");
	options.add_options()
			("config", value(& configurationFile_)->default_value(DEFAULT_OPTIONS_CONFIG), "Read configuration from the given file. The default file will be read if it exists")
			("help", "Show help message")
			("version", "Show version information");

	options_description source("Data source");
	source.add_options()
			("source.file", value<std::string>(& source_.fileName), "Read data from this file")
			("source.type", value<data_access::fimex::FileType>(& source_.fileType), "Assume that file is of the given type (netcdf, felt or wdb)")
			("source.config", value<std::string>(& source_.configFileName), "Read source file configuration from this file");

	options_description sink("Data sink");
	sink.add_options()
			("sink.file", value<std::string>(& sink_.fileName), "Write data to this file")
			("sink.type", value<data_access::fimex::FileType>(& sink_.fileType), "Assume that file is of the given type (netcdf, felt or wdb)")
			("sink.config", value<std::string>(& sink_.configFileName), "Read sink file configuration from this file");

	options_description control("Generation control");
	control.add_options()
			( "timesteps", value<int>(), "Only generate data for the given number of hours.")
			( "excludePercentiles", bool_switch( & control_.excludePercentiles ), "Exclude generation of percentile data" )
			( "excludeSimplifiedProbabilities", bool_switch( & control_.excludeSimplifiedProbabilities ), "Exclude generation of simplified probabilities" )
			( "parameters_definitions", value<std::string>(),  "Read parameter specifications from the given file. If not given, "SYSCONFDIR"/eps2probandltf.parameters.xml will be used")
			( "exclude,x", value<std::vector<std::string> >(), "Do not create probabilities for the given data. Legal values are symbols, clouds, temperature, wind, precipitation, pressure, humidity, waves or all. Excluding symbols will also exclude many other probabilities.")
			( "include,i", value<std::vector<std::string> >(), "Override a previous call to --exclude." )
			( "precipFraction", value(& control_.precipFraction)->default_value(0.5), "Fraction of members needed to trigger precipitation in the symbol." )
			;

	options_description logging("Logging");
	logging.add_options()
			("log-level", value(& logging_.logLevel)->default_value("info"), "Use the given log level (off, fatal, error, warn, info or debug.")
			("log-file", value(& logging_.logFile), "Log to the given file, instead of stdout");
			;

	options.add(source).add(sink).add(control).add(logging);

    variables_map vm;

    store(parse_command_line(argc, argv, options), vm);
    notify(vm);

    options_description configOptions;
    configOptions.add(source).add(sink).add(control).add(logging);
    if ( not configurationFile_.empty() )
    {
    	boost::filesystem::path configFile(configurationFile_);
    	if ( ! exists(configFile) )
    	{
    		if ( configurationFile_ != DEFAULT_OPTIONS_CONFIG )
    			throw std::runtime_error(configurationFile_ + " does not exist");
    	}
    	else if ( is_directory(configFile) )
    	{
			throw std::runtime_error(configurationFile_ + " is a directory");
    	}
    	else
    	{
			std::ifstream configStream(configurationFile_.c_str());
			store(parse_config_file(configStream, configOptions), vm);
			notify(vm);
    	}
    }

    if ( vm.count("help") )
    {
    	help(std::cout, options);
    	exit(0);
    }
    if ( vm.count("version") )
    {
    	version(std::cout);
    	exit(0);
    }

    if ( vm.count("timesteps") )
    	control_.numberOfHoursToGenerate = vm["timesteps"].as<int>();
    else
    	control_.numberOfHoursToGenerate = std::numeric_limits<int>::max();

    if ( vm.count("parameters_definitions") )
    	sink_.parameterSpecificationFile = vm["parameters_definitions"].as<std::string>();
    else
    	sink_.parameterSpecificationFile = SYSCONFDIR"/eps2probandltf.parameters.xml";
    source_.parameterSpecificationFile = sink_.parameterSpecificationFile;

    if ( source_.fileType == data_access::fimex::Felt and source_.configFileName.empty() )
    	source_.configFileName = DEFAULT_FELT_INPUT_CONFIG;

    std::set<std::string> logLevels = boost::assign::list_of("off")("fatal")("error")("warn")("info")("debug");
    if ( logLevels.find(logging_.logLevel) == logLevels.end() )
    {
    	std::clog << logging_.logLevel << " is not a valid log level. Please enter a valid log level." << std::endl;
    	exit(1);
    }

    if ( vm.count("exclude") )
    {
    	std::vector<std::string> excludeList = vm["exclude"].as<std::vector<std::string> >();
    	for ( std::vector<std::string>::const_iterator it = excludeList.begin(); it != excludeList.end(); ++ it )
    	{
    		if ( * it == "all" )
    		{
    			for (std::map<std::string, bool>::iterator it = control_.excludedParameters.begin(); it != control_.excludedParameters.end(); ++ it )
    				it->second = true;
    		}
    		else
    		{
				std::map<std::string, bool>::iterator find = control_.excludedParameters.find(* it);
				if ( find == control_.excludedParameters.end() )
					throw std::runtime_error(* it + ": invalid exclusion");
				find->second = true;
    		}
    	}
    }
    if ( vm.count("include") )
    {
    	std::vector<std::string> includeList = vm["include"].as<std::vector<std::string> >();
    	for ( std::vector<std::string>::const_iterator it = includeList.begin(); it != includeList.end(); ++ it )
    	{
    		if ( * it == "all" )
    		{
    			for (std::map<std::string, bool>::iterator it = control_.excludedParameters.begin(); it != control_.excludedParameters.end(); ++ it )
    				it->second = false;
    		}
    		else
    		{
				std::map<std::string, bool>::iterator find = control_.excludedParameters.find(* it);
				if ( find == control_.excludedParameters.end() )
					throw std::runtime_error(* it + ": invalid exclusion");
				find->second = false;
    		}
    	}
    }
}

ProbabilityGenerationConfiguration::~ProbabilityGenerationConfiguration()
{
}

ProbabilityGenerationConfiguration::GeneratorControl::GeneratorControl()
{
	excludedParameters["symbols"] = false;
	excludedParameters["clouds"] = false;
	excludedParameters["temperature"] = false;
	excludedParameters["wind"] = false;
	excludedParameters["precipitation"] = false;
	excludedParameters["pressure"] = false;
	excludedParameters["waves"] = false;
	excludedParameters["humidity"] = false;
}

bool ProbabilityGenerationConfiguration::GeneratorControl::shouldGenerate(const std::string & what) const
{
	std::map<std::string, bool>::const_iterator find = excludedParameters.find(what);
	if ( find == excludedParameters.end() )
		throw std::runtime_error(what + " no such parameter is known");
	return not find->second;
}
