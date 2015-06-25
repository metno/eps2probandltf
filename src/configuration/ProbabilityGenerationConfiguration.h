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

#ifndef PROBABILITYGENERATIONCONFIGURATION_H_
#define PROBABILITYGENERATIONCONFIGURATION_H_

#include <data_access/fimex/FileType.h>
#include <string>
#include <map>

class ProbabilityGenerationConfiguration
{
public:
	ProbabilityGenerationConfiguration(int argc, char ** argv);
	~ProbabilityGenerationConfiguration();

	struct DataAccessOptions
	{
		std::string fileName;
		std::string configFileName;
		data_access::fimex::FileType fileType;
		std::string parameterSpecificationFile;
	};
	typedef DataAccessOptions Source;
	typedef DataAccessOptions Sink;
	struct GeneratorControl
	{
		GeneratorControl();

		int numberOfHoursToGenerate;
		bool excludePercentiles;
		bool excludeSimplifiedProbabilities;
		std::map<std::string, bool> excludedParameters;
      float precipFraction;

		bool shouldGenerate(const std::string & what) const;
	};
	struct LoggingOptions
	{
		std::string logLevel;
		std::string logFile;
	};

	const Source & source() const { return source_; }
	const Sink & sink() const { return sink_; }
	const GeneratorControl & control() const { return control_; }
	const LoggingOptions & logging() const { return logging_; }

private:
	Source source_;
	Sink sink_;
	GeneratorControl control_;
	LoggingOptions logging_;

	std::string configurationFile_;
};

#endif /* PROBABILITYGENERATIONCONFIGURATION_H_ */
