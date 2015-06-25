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

#include "ProbabilityGenerator.h"
#include <dataTypes.h>
#include <localtime.h>
#include <data_access/Source.h>
#include <data_access/Sink.h>
#include <container/ccAlgorithm.h>
#include "symbol/symbol.h"
#include "temperature/TemperatureProbabilityGenerator.h"
#include "wind/generateWind.h"
#include "wind/Wind.h"
#include "precipitation/PrecipitationProbabilityGenerator.h"
#include "airPressure/AirPressureProbabilityGenerator.h"
#include <container/CrossContainer.h>
#include <log4cxx/logger.h>
#include <boost/assign/list_of.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>
#include <sstream>
#include <functional>
#include <algorithm>


ProbabilityGenerator::ProbabilityGenerator(
		data_access::Source & source,
		data_access::Sink & sink,
		const ProbabilityGenerationConfiguration::GeneratorControl & conf,
		const boost::local_time::local_date_time & validTime) :
	validTime_(validTime),
	source_(source),
	sink_(sink),
	conf_(conf)
{
}

#define HANDLE_EXCEPTIONS(f) \
	try { \
		f; \
	} catch (data_access::Source::DataNotAvailable & e ) { \
		LOG4CXX_WARN(logger, e.what()); \
	} catch (std::exception & e ) { \
		LOG4CXX_ERROR(logger, e.what()); \
		throw; \
	}

#define LOG_EXCEPTIONS(f) \
		try { \
			f; \
		} catch (data_access::Source::DataNotAvailable & e ) { \
			LOG4CXX_INFO(logger, e.what()); \
		} catch (std::exception & e ) { \
			LOG4CXX_WARN(logger, e.what()); \
		}

//#define HANDLE_EXCEPTIONS(f) f

ProbabilityGenerator::~ProbabilityGenerator()
{
}

void ProbabilityGenerator::work()
{
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("pgen_probability.generator"));

	type::BlobCollectionPtr symbols;
	if ( conf_.shouldGenerate("symbols") )
	{
		try
		{
			symbols = generateSymbols_();
		}
		catch ( std::exception & e )
		{
			// This gives an error for the last valid time in list of 18:00 data is missing for that day
			LOG4CXX_ERROR(logger, e.what() + std::string(" - unable to generate any probabilities for period "));
			return;
		}
	}

	if ( symbols )
	{
		if ( conf_.shouldGenerate("clouds") )
			HANDLE_EXCEPTIONS(generateClouds_(* symbols));
		if ( conf_.shouldGenerate("temperature") )
		{
			HANDLE_EXCEPTIONS(generateTemperature_(* symbols));
			HANDLE_EXCEPTIONS(generateTemperatureMin_(* symbols));
			HANDLE_EXCEPTIONS(generateTemperatureMax_(* symbols));
		}
		if ( conf_.shouldGenerate("wind") )
			HANDLE_EXCEPTIONS(generateWind_(* symbols));
		if ( conf_.shouldGenerate("precipitation") )
			HANDLE_EXCEPTIONS(generatePrecipitation_(* symbols));
		if ( conf_.shouldGenerate("pressure") )
			HANDLE_EXCEPTIONS(generateAirPressure_(* symbols));
		if ( conf_.shouldGenerate("humidity") )
			HANDLE_EXCEPTIONS(generateHumidity_(* symbols));
	}

	if ( conf_.shouldGenerate("waves") )
		HANDLE_EXCEPTIONS(generateWaves_());

	saveAll_();
}


type::BlobCollectionPtr ProbabilityGenerator::generateSymbols_()
{
	const boost::posix_time::time_duration range(6,0,0);
	type::BlobCollectionPtr cloudCover = getCloudAverage_(range);
	type::BlobCollectionPtr precipitation = getPrecipitationSum_();

	type::BlobContainer yrSymbolProbability;
	type::BlobCollectionPtr allSymbols = weatherSymbol::getSymbolCertainty(
			* cloudCover, * precipitation,
			& selectedYrSymbol_, & yrSymbolProbability, conf_.precipFraction);
	if ( ! conf_.excludeSimplifiedProbabilities )
	{
		toSave_[ YrSymbol ] = selectedYrSymbol_;
		toSave_[ ProbabilityOfYrSymbol ] = yrSymbolProbability;
	}
	return allSymbols;
}

void ProbabilityGenerator::generateClouds_(type::BlobCollection & allSymbols)
{
	type::BlobForecastCollection cloudCover = getData_(CloudCover, validTime_);

	if ( ! conf_.excludeSimplifiedProbabilities )
	{
		SimpleProbabilityGenerator generator(selectedYrSymbol_, allSymbols, * cloudCover.data());
		toSave_[ CloudCover ] = generator.getSelected();
	}
	if ( not conf_.excludePercentiles )
	{
		PercentileGenerator cloudGenerator(* cloudCover.data());
		toSave_[ CloudCover10Percentile ] = cloudGenerator.get10Percentile();
		toSave_[ CloudCover25Percentile ] = cloudGenerator.get25Percentile();
		toSave_[ CloudCover50Percentile ] = cloudGenerator.get50Percentile();
		toSave_[ CloudCover75Percentile ] = cloudGenerator.get75Percentile();
		toSave_[ CloudCover90Percentile ] = cloudGenerator.get90Percentile();
	}

	generateSimple_(LowCloudCover, allSymbols);
	generateSimple_(MediumCloudCover, allSymbols);
	generateSimple_(HighCloudCover, allSymbols);
}

void ProbabilityGenerator::generateTemperature_(type::BlobCollection & allSymbols)
{
	type::BlobForecastCollection temperature = getData_(Temperature, validTime_);

	TemperatureProbabilityGenerator t(selectedYrSymbol_, allSymbols, * temperature.data());
	if ( ! conf_.excludeSimplifiedProbabilities )
	{
		toSave_[ Temperature ] = t.getTemperature();
	}
	if ( ! conf_.excludePercentiles )
	{
		toSave_[ Temperature10Percentile ] = t.get10Percentile();
		toSave_[ Temperature25Percentile ] = t.get25Percentile();
		toSave_[ Temperature50Percentile ] = t.get50Percentile();
		toSave_[ Temperature75Percentile ] = t.get75Percentile();
		toSave_[ Temperature90Percentile ] = t.get90Percentile();
	}
	if ( ! conf_.excludeSimplifiedProbabilities )
	{
		t.getTemperatureProbability1p5(toSave_[ TemperatureProbability1 ]);
		t.getTemperatureProbability2p5(toSave_[ TemperatureProbability2 ]);
	}
}

void ProbabilityGenerator::generateTemperatureMin_(type::BlobCollection & allSymbols)
{
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("pgen_probability.generator.TAN"));
	generateSimple_(TemperatureMin, allSymbols);

	// Ensure that the lowest value of TAN and regular temperature is set in data
	DataCollection::const_iterator temperature = toSave_.find(Temperature);
	if ( temperature == toSave_.end() )
	{
		LOG4CXX_WARN(logger, "Unable to find selected temperature - skipping correction of TAN");
		return;
	}
	DataCollection::iterator tan = toSave_.find(TemperatureMin);
	if ( tan == toSave_.end() )
		throw std::runtime_error("Internal error - unable to locate newly generated TAN values");
	if ( tan->second.size() != temperature->second.size() )
		throw std::runtime_error("Selected temperature grid size does not match TAN grid size");

	for ( int i = 0; i < tan->second.size(); ++ i )
	{
		tan->second[i] = std::min(tan->second[i], temperature->second[i]);
	}
}

void ProbabilityGenerator::generateTemperatureMax_(type::BlobCollection & allSymbols)
{
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("pgen_probability.generator.TAX"));
	generateSimple_(TemperatureMax, allSymbols);

	// Ensure that the highest value of TAX and regular temperature is set in data
	DataCollection::const_iterator temperature = toSave_.find(Temperature);
	if ( temperature == toSave_.end() )
	{
		LOG4CXX_WARN(logger, "Unable to find selected temperature - skipping correction of TAX");
		return;
	}
	DataCollection::iterator tax = toSave_.find(TemperatureMax);
	if ( tax == toSave_.end() )
		throw std::runtime_error("Internal error - unable to locate newly generated TAX values");
	if ( tax->second.size() != temperature->second.size() )
		throw std::runtime_error("Selected temperature grid size does not match TAX grid size");

	for ( int i = 0; i < tax->second.size(); ++ i )
	{
		tax->second[i] = std::max(tax->second[i], temperature->second[i]);
	}
}



namespace
{
typedef BasicPercentileGenerator<Wind, WindForceLess> WindPercentileGenerator;
void transferForce(type::BlobContainer & to, const WindPercentileGenerator::Container & from)
{
	BOOST_FOREACH(const Wind & w, from)
		to.push_back(w.force());
}
}

void ProbabilityGenerator::generateWind_(type::BlobCollection & allSymbols)
{
	type::BlobForecastCollection windU = getData_(WindU, validTime_);
	type::BlobForecastCollection windV = getData_(WindV, validTime_);

	type::WindForecastCollection wind = getWind(windU, windV);

	if ( not conf_.excludeSimplifiedProbabilities )
		getWindProbabilities(
				selectedYrSymbol_, allSymbols, * wind.data(),
				& toSave_[ WindU ], & toSave_[ WindV ], & toSave_[ WindProbability ]);

	if ( not conf_.excludePercentiles )
	{
		WindPercentileGenerator generator(* wind.data());

		transferForce(toSave_[Wind10Percentile], generator.get10Percentile());
		transferForce(toSave_[Wind25Percentile], generator.get25Percentile());
		transferForce(toSave_[Wind50Percentile], generator.get50Percentile());
		transferForce(toSave_[Wind75Percentile], generator.get75Percentile());
		transferForce(toSave_[Wind90Percentile], generator.get90Percentile());
	}
}

inline float metreFromMillimetre(const float millimetre)
{
	return millimetre / 1000;
}

void ProbabilityGenerator::generatePrecipitation_(type::BlobCollection & allSymbols)
{
	type::BlobCollectionPtr precipitation = getPrecipitationSum_();

	PrecipitationProbabilityGenerator p(selectedYrSymbol_, allSymbols, * precipitation);

	if ( ! conf_.excludeSimplifiedProbabilities )
		toSave_[ PrecipitationOut ] = p.getPrecipitation();

	if ( ! conf_.excludePercentiles )
	{
		toSave_[ Precipitation10Percentile ] = p.get10Percentile();
		toSave_[ Precipitation25Percentile ] = p.get25Percentile();
		toSave_[ Precipitation50Percentile ] = p.get50Percentile();
		toSave_[ Precipitation75Percentile ] = p.get75Percentile();
		toSave_[ Precipitation90Percentile ] = p.get90Percentile();

		// We include probability of precipitation abov X mm in this aswell
		p.probabilityOfValueAbove(toSave_[ PrecipitationAbove0_1mm ], 0.1);
		p.probabilityOfValueAbove(toSave_[ PrecipitationAbove0_2mm ], 0.2);
		p.probabilityOfValueAbove(toSave_[ PrecipitationAbove0_5mm ], 0.5);
		p.probabilityOfValueAbove(toSave_[ PrecipitationAbove1_0mm ], 1);
		p.probabilityOfValueAbove(toSave_[ PrecipitationAbove2_0mm ], 2);
		p.probabilityOfValueAbove(toSave_[ PrecipitationAbove5_0mm ], 5);
	}
}

void ProbabilityGenerator::generateAirPressure_(type::BlobCollection & allSymbols)
{
	generateSimple_(AirPressure, allSymbols);
}

void ProbabilityGenerator::generateHumidity_(type::BlobCollection & allSymbols)
{
	generateSimple_(RelativeHumidity, allSymbols);
}

void ProbabilityGenerator::generateWaves_()
{
	type::BlobForecastCollection waves = getData_(SignificantWaveHeight, validTime_);
	PercentileGenerator generator(* waves.data());

	if ( ! conf_.excludePercentiles )
	{
		toSave_[ SignificantWaveHeight10Percentile ] = generator.get10Percentile();
		toSave_[ SignificantWaveHeight25Percentile ] = generator.get25Percentile();
		toSave_[ SignificantWaveHeight50Percentile ] = generator.get50Percentile();
		toSave_[ SignificantWaveHeight75Percentile ] = generator.get75Percentile();
		toSave_[ SignificantWaveHeight90Percentile ] = generator.get90Percentile();
	}
}

void ProbabilityGenerator::generateSimple_(WeatherData phenomenon, type::BlobCollection & allSymbols)
{
	if ( conf_.excludeSimplifiedProbabilities )
		return;

	type::BlobForecastCollection d = getData_(phenomenon, validTime_);
	SimpleProbabilityGenerator generator(selectedYrSymbol_, allSymbols, * d.data());
	toSave_[phenomenon] = generator.getSelected();
}


void ProbabilityGenerator::saveAll_()
{
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("pgen_probability.generator"));

	for ( DataCollection::const_iterator it = toSave_.begin(); it != toSave_.end(); ++ it )
	{
		try
		{
			sink_.write(it->first, validTime_, it->second);
		}
		catch ( std::exception & e )
		{
			LOG4CXX_ERROR(logger, e.what());
		}
	}
}

type::BlobForecastCollection ProbabilityGenerator::getData_(WeatherData d, const boost::local_time::local_date_time & validTime)
{
	type::BlobForecastCollection ret = source_.getData(d, validTime);
	if ( ret.data()->empty() )
	{
		std::ostringstream errMsg;
		errMsg << "Unable to get data for " << d << " at valid time " << string_from_local_date_time(validTime);
		throw std::runtime_error(errMsg.str());
	}

	return ret;
}


type::BlobCollectionPtr ProbabilityGenerator::getPrecipitationSum_()
{
	type::BlobForecastCollection precipitationA = getData_(PrecipitationInFrom, validTime_);
	type::BlobForecastCollection precipitation = getData_(PrecipitationInTo, validTime_);

	int subzeroCount = 0;

	for ( type::BlobCollection::size_type i = 0; i < precipitation.data()->size(); ++ i)
		for ( type::BlobCollection::size_type j = 0; j < (*precipitation.data())[i].size(); ++ j )
		{
			float newVal = precipitation.data()->at(i).at(j) - precipitationA.data()->at(i).at(j);
			if ( newVal < 0 )
			{
				if ( newVal < -0.001 )
					++ subzeroCount;
				newVal = 0;
			}
			(*precipitation.data())[i][j] = newVal;
		}

	return precipitation.data();
}

type::BlobCollectionPtr ProbabilityGenerator::getCloudAverage_(const boost::posix_time::time_duration & range)
{
	type::BlobForecastCollection cloudCoverA = getData_(CloudCover, validTime_);

	boost::local_time::local_date_time nextValidTime = validTime_ + range;
	type::BlobForecastCollection cloudCover = getData_(CloudCover, nextValidTime);

	for ( type::BlobCollection::size_type i = 0 ; i < cloudCover.data()->size(); ++ i )
		for ( type::BlobCollection::size_type j = 0; j < (*cloudCover.data())[i].size(); ++ j )
			(*cloudCover.data())[i][j] = ((*cloudCover.data())[i][j] + (*cloudCoverA.data())[i][j]) / 2;

	return cloudCover.data();
}
