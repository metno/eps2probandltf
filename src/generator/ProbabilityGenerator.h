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

#ifndef PROBABILITYGENERATOR_H_
#define PROBABILITYGENERATOR_H_

#include <boost/noncopyable.hpp>
#include "ForecastCollection.h"
#include <WeatherData.h>
#include <dataTypes.h>
#include <configuration/ProbabilityGenerationConfiguration.h>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <map>

class DataSpecificationCollection;

namespace data_access
{
class Source;
class Sink;
}

/**
 * Generates probability values for various meteorological parameters, for a
 * single time step.
 */
class ProbabilityGenerator : boost::noncopyable
{
public:
	/**
	 * @param conn Database connection for fetching and storing data
	 * @param referenceTime reference time of data to be processed
	 * @param validtime for data to be processed
	 */
	ProbabilityGenerator(
			data_access::Source & source,
			data_access::Sink & sink,
			const ProbabilityGenerationConfiguration::GeneratorControl & conf,
			const boost::local_time::local_date_time & validTime);
	virtual ~ProbabilityGenerator();

	/**
	 * Fetch data from database, process it, and store data back into database.
	 */
	void work();

private:
	type::BlobCollectionPtr generateSymbols_();
	void generateClouds_(type::BlobCollection & allSymbols);
	void generateTemperature_(type::BlobCollection & allSymbols);
	void generateTemperatureMin_(type::BlobCollection & allSymbols);
	void generateTemperatureMax_(type::BlobCollection & allSymbols);
	void generateWind_(type::BlobCollection & allSymbols);
	void generatePrecipitation_(type::BlobCollection & allSymbols);
	void generateAirPressure_(type::BlobCollection & allSymbols);
	void generateHumidity_(type::BlobCollection & allSymbols);
	void generateWaves_();

	void generateSimple_(WeatherData phenomenon, type::BlobCollection & allSymbols);

	void saveAll_();

	type::BlobForecastCollection getData_(WeatherData d, const boost::local_time::local_date_time & validTime);

	type::BlobCollectionPtr getPrecipitationSum_();
	type::BlobCollectionPtr getCloudAverage_(const boost::posix_time::time_duration & range);

	type::BlobContainer selectedYrSymbol_;

	typedef std::map<WeatherData, type::BlobContainer> DataCollection;
	DataCollection toSave_;

	type::Time validTime_;

	data_access::Source & source_;
	data_access::Sink & sink_;

	const ProbabilityGenerationConfiguration::GeneratorControl & conf_;
};

#endif /* PROBABILITYGENERATOR_H_ */
