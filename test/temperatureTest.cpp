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
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301, USA
 */

#include <gtest/gtest.h>

#include <generator/temperature/temperatureInternal.h>
#include <generator/temperature/TemperatureProbabilityGenerator.h>
#include <generator/symbol/SymbolGenerator.h>
#include <container/CrossContainer.h>
#include <generator/algorithm.h>


class TemperatureProbabilitiesTest : public testing::Test
{
	type::BlobCollection * setupTemperatureSamples()
	{
		type::BlobCollection * ret = new type::BlobCollection(5, type::BlobContainer());
		type::BlobCollection & s = * ret;
		s[0].push_back(3);
		s[1].push_back(3);
		s[2].push_back(3);
		s[3].push_back(3);
		s[4].push_back(3);

		s[0].push_back(4);
		s[1].push_back(9);
		s[2].push_back(6.5);
		s[3].push_back(5);
		s[4].push_back(9);

		s[0].push_back(7);
		s[1].push_back(2);//
		s[2].push_back(0);
		s[3].push_back(3);
		s[4].push_back(1);

		s[0].push_back(3);
		s[1].push_back(4);
		s[2].push_back(-1);
		s[3].push_back(-5);
		s[4].push_back(4);

		return ret;
	}

	type::BlobCollection * setupDewPointTemperatureSamples()
	{
		type::BlobCollection * ret = new type::BlobCollection(5, type::BlobContainer());
		type::BlobCollection & s = * ret;
		s[0].push_back(100);//
		s[1].push_back(1000);
		s[2].push_back(30);
		s[3].push_back(300);
		s[4].push_back(3);

		s[0].push_back(101);
		s[1].push_back(102);
		s[2].push_back(100);//
		s[3].push_back(44);
		s[4].push_back(155);

		s[0].push_back(1147);
		s[1].push_back(100);//
		s[2].push_back(1110);
		s[3].push_back(1253);
		s[4].push_back(199);

		s[0].push_back(13);
		s[1].push_back(14);
		s[2].push_back(100);//
		s[3].push_back(-5);
		s[4].push_back(49);

		return ret;
	}

	type::BlobCollection * setupWeatherSymbols()
	{
		using namespace weather_symbol;

		type::BlobCollection * ret = new type::BlobCollection(5);
		(*ret)[0].push_back(PartlyCloud);
		(*ret)[1].push_back(PartlyCloud);
		(*ret)[2].push_back(PartlyCloud);
		(*ret)[3].push_back(PartlyCloud);
		(*ret)[4].push_back(PartlyCloud);

		(*ret)[0].push_back(Cloud);
		(*ret)[1].push_back(Sun);
		(*ret)[2].push_back(LightRain);
		(*ret)[3].push_back(Rain);
		(*ret)[4].push_back(Rain);

		(*ret)[0].push_back(LightRain);
		(*ret)[1].push_back(Sun);
		(*ret)[2].push_back(Sun);
		(*ret)[3].push_back(PartlyCloud);
		(*ret)[4].push_back(PartlyCloud);

		(*ret)[0].push_back(PartlyCloud);
		(*ret)[1].push_back(Cloud);
		(*ret)[2].push_back(LightRainSun);
		(*ret)[3].push_back(Rain);
		(*ret)[4].push_back(Rain);

		return ret;
	}

public:
	void SetUp()
	{
		temperature = new type::BlobContainer;
		temperature->push_back(5);
		temperature->push_back(8);
		temperature->push_back(1);
		temperature->push_back(0);


		temperatureSamples = setupTemperatureSamples();
		dewPointTemperatureSamples = setupDewPointTemperatureSamples();
		allSymbols = setupWeatherSymbols();

		selectedSymbols = new type::BlobContainer;
		// setup allSymbols
		for ( unsigned i = 0; i < (*allSymbols)[0].size(); ++ i )
		{
			CrossContainer<float> symbolSamples(* allSymbols, i);
			selectedSymbols->push_back(weatherSymbol::selectSymbol(symbolSamples.begin(), symbolSamples.end()));
		}
	}

	void TearDown()
	{
		delete temperatureSamples;
		delete dewPointTemperatureSamples;
		delete temperature;
		delete selectedSymbols;
		delete allSymbols;

	}

protected:
	/// ensemble members
	type::BlobCollection * temperatureSamples;
	type::BlobCollection * dewPointTemperatureSamples;

	/// the temperature we have decided on
	type::BlobContainer * temperature;

	type::BlobContainer * selectedSymbols;
	type::BlobCollection * allSymbols;
};

TEST_F(TemperatureProbabilitiesTest, getProbableTemperature)
{
	type::BlobContainer result;

//	std::cout << selectedSymbols->size()<<" - "<<allSymbols->size()<<" - "<<temperatureSamples->size()<<std::endl;

	getProbableValues(* selectedSymbols, * allSymbols, * temperatureSamples, & result);

	ASSERT_EQ(4u, result.size());
	EXPECT_FLOAT_EQ(3.0, result[0]);
	EXPECT_FLOAT_EQ(6.5, result[1]);
	EXPECT_FLOAT_EQ(1, result[2]);
	EXPECT_FLOAT_EQ(-0.5, result[3]);
}

TEST_F(TemperatureProbabilitiesTest, getTemperatureDeviationProbability)
{
	type::BlobContainer result;
	getTemperatureDeviationProbability(1.5, * temperatureSamples, * temperature, result);

	ASSERT_EQ(4u, result.size());
	EXPECT_EQ(0.0, result[0]);
	EXPECT_FLOAT_EQ(60, result[1]);
	EXPECT_FLOAT_EQ(60, result[2]);
	EXPECT_FLOAT_EQ(20, result[3]);
}

TEST_F(TemperatureProbabilitiesTest, getTemperatureDeviationProbabilityReal)
{
	float data[51] = {
		16.248,
		16.191,
		15.878,
		16.027,
		15.689,
		15.906,
		16.257,
		16.185,
		15.48,
		16.752,
		15.942,
		15.954,
		16.169,
		16.661,
		15.063,
		16.876,
		16.138,
		16.097,
		15.64,
		15.564,
		15.316,
		15.933,
		16.81,
		16.106,
		16.07,
		16.081,
		16.494,
		16.125,
		15.374,
		15.364,
		16.153,
		16.426,
		16.282,
		16.094,
		16.743,
		16.671,
		16.195,
		15.861,
		16.087,
		16.565,
		15.913,
		15.931,
		15.823,
		15.392,
		16.474,
		16.495,
		15.835,
		17.042,
		15.154,
		16.289,
		15.452
	};

	float probability = getTemperatureDeviationProbability(2, data, data +51, 16.076);

	ASSERT_EQ(100, probability);
}

TEST_F(TemperatureProbabilitiesTest, getProbableTemperature2)
{
	TemperatureProbabilityGenerator t(* selectedSymbols, * allSymbols, * temperatureSamples);
	const type::BlobContainer & result = t.getTemperature();

	ASSERT_EQ(4u, result.size());
	EXPECT_FLOAT_EQ(3.0, result[0]);
	EXPECT_FLOAT_EQ(6.5, result[1]);
	EXPECT_FLOAT_EQ(1, result[2]);
	EXPECT_FLOAT_EQ(-0.5, result[3]);
}

TEST_F(TemperatureProbabilitiesTest, getTemperatureDeviationProbability2)
{
	// Note: this test ignores the preselected values, given in the temperature variable.

	TemperatureProbabilityGenerator t(* selectedSymbols, * allSymbols, * temperatureSamples);
	type::BlobContainer result;
	t.getTemperatureProbability2p5(result);

	ASSERT_EQ(4u, result.size());
	EXPECT_EQ(100, result[0]);
	EXPECT_FLOAT_EQ(100, result[1]);
	EXPECT_FLOAT_EQ(80, result[2]);
	EXPECT_FLOAT_EQ(20, result[3]);
}

TEST_F(TemperatureProbabilitiesTest, preservesOriginalOrderingOfBaseData)
{
	TemperatureProbabilityGenerator t(* selectedSymbols, * allSymbols, * temperatureSamples);
	type::BlobContainer result;
	t.getTemperatureProbability1p5(result);

	ASSERT_EQ(4u, result.size());

	EXPECT_FLOAT_EQ(4, (*temperatureSamples)[0][1]);
	EXPECT_FLOAT_EQ(9, (*temperatureSamples)[1][1]);
	EXPECT_FLOAT_EQ(6.5, (*temperatureSamples)[2][1]);
	EXPECT_FLOAT_EQ(5, (*temperatureSamples)[3][1]);
	EXPECT_FLOAT_EQ(9, (*temperatureSamples)[4][1]);
}
