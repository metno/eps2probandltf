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


#ifndef WEATHERDATA_H_
#define WEATHERDATA_H_


enum WeatherData
{
	CloudCover,
	YrSymbol,
	ProbabilityOfYrSymbol,
	Temperature,
	TemperatureMin,
	TemperatureMax,
	Temperature10Percentile,
	Temperature25Percentile,
	Temperature50Percentile,
	Temperature75Percentile,
	Temperature90Percentile,
	TemperatureProbability1,
	TemperatureProbability2,
	WindU,
	WindV,
	WindProbability,
	Wind10Percentile,
	Wind25Percentile,
	Wind50Percentile,
	Wind75Percentile,
	Wind90Percentile,
	PrecipitationInFrom,
	PrecipitationInTo,
	PrecipitationOut,
	Precipitation10Percentile,
	Precipitation25Percentile,
	Precipitation50Percentile,
	Precipitation75Percentile,
	Precipitation90Percentile,
	PrecipitationAbove0_1mm,
	PrecipitationAbove0_2mm,
	PrecipitationAbove0_5mm,
	PrecipitationAbove1_0mm,
	PrecipitationAbove2_0mm,
	PrecipitationAbove5_0mm,
	AirPressure,
	RelativeHumidity,
	DewPointTemperature,
	LowCloudCover,
	MediumCloudCover,
	HighCloudCover,
	CloudCover10Percentile,
	CloudCover25Percentile,
	CloudCover50Percentile,
	CloudCover75Percentile,
	CloudCover90Percentile,
	SignificantWaveHeight,
	SignificantWaveHeight10Percentile,
	SignificantWaveHeight25Percentile,
	SignificantWaveHeight50Percentile,
	SignificantWaveHeight75Percentile,
	SignificantWaveHeight90Percentile,
	MAX_WEATHER_DATA_ENTRIES_
};


#endif /* WEATHERDATA_H_ */
