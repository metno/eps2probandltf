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

#ifndef PROBABILITYGENERATORCONTROL_H_
#define PROBABILITYGENERATORCONTROL_H_

#include <configuration/ProbabilityGenerationConfiguration.h>
#include <boost/date_time/local_time/local_time.hpp>
#include <string>
#include <vector>

namespace data_access
{
class Source;
class Sink;
}

/**
 * Creates and stores probability values for all time steps of a reference
 * time.
 */
class ProbabilityGeneratorControl
{
public:
	ProbabilityGeneratorControl(
			data_access::Source & dataSource,
			data_access::Sink & dataSink,
			const ProbabilityGenerationConfiguration::GeneratorControl & conf);
	~ProbabilityGeneratorControl();

	/**
	 * Find all validTimes, and invoke ProbabilityGenerator for each of those
	 */
	void work();

private:
	data_access::Source & source_;
	data_access::Sink & sink_;
	const ProbabilityGenerationConfiguration::GeneratorControl & conf_;
};

#endif /* PROBABILITYGENERATORCONTROL_H_ */
