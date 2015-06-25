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

#include <gtest/gtest.h>
#include <generator/symbol/symbol.h>
#include <generator/symbol/SymbolGenerator.h>
#include <container/CrossContainer.h>
#include "dataTypes.h"

using namespace type;

class SymbolGenerateTest : public testing::Test
{
protected:
	void SetUp()
	{
		cloudCover = new BlobCollection;
		int cover = 0;
		for ( size_t i = 0; i < steps; ++ i )
		{
			cloudCover->push_back(BlobContainer());
			for ( size_t j = 0; j < gridSize; ++ j)
				cloudCover->back().push_back(((cover ++ % 9) / 8.0));
		}
//		{ 0,    12.5, 25,   37.5 },  // cloud 0,0,1,1
//		{ 50,   62.5, 75,   87.5 },  // cloud 2,2,2,3
//		{ 100,  0,    12.5, 25   },  // cloud 3,0,0,1
//		{ 37.5, 50,   62.5, 75   },  // cloud 1,2,2,2
//		{ 87.5, 100,  0,    12.5 }   // cloud 3,3,0,0

		precipitation = new BlobCollection;
		float precip = 0;
		for ( size_t i = 0; i < steps; ++ i )
		{
			precipitation->push_back(BlobContainer());
			for ( size_t j = 0; j < gridSize; ++ j)
				precipitation->back().push_back(precip ++ / 0.2);
		}

	}

	void TearDown()
	{
		delete cloudCover;
		delete precipitation;
	}

	static const size_t gridSize;
	static const size_t steps;

	BlobCollection * cloudCover;
	BlobCollection * precipitation;
};
const size_t SymbolGenerateTest::gridSize = 4;
const size_t SymbolGenerateTest::steps = 5;


TEST_F(SymbolGenerateTest, generateSymbol)
{
	// Result
	BlobCollectionPtr result = weatherSymbol::generateSymbols(* cloudCover, * precipitation);
	ASSERT_EQ(steps, result->size());

	using namespace weather_symbol;
	const BlobCollection & bc = * result;
	EXPECT_EQ(Sun,          bc[0][0]);
	EXPECT_EQ(RainSun, bc[0][1]);
	EXPECT_EQ(RainSun, bc[0][2]);
	EXPECT_EQ(RainSun, bc[0][3]);
	EXPECT_EQ(RainSun,    bc[1][0]);
	EXPECT_EQ(RainSun,    bc[1][1]);
	EXPECT_EQ(RainSun,    bc[1][2]);
	EXPECT_EQ(Rain,    bc[1][3]);
	EXPECT_EQ(Rain,    bc[2][0]);
	EXPECT_EQ(RainSun, bc[2][1]);
	EXPECT_EQ(RainSun, bc[2][2]);
	EXPECT_EQ(RainSun, bc[2][3]);
	EXPECT_EQ(RainSun, bc[3][0]);
	EXPECT_EQ(RainSun,    bc[3][1]);
	EXPECT_EQ(RainSun,    bc[3][2]);
	EXPECT_EQ(RainSun,    bc[3][3]);
	EXPECT_EQ(Rain,    bc[4][0]);
	EXPECT_EQ(Rain,    bc[4][1]);
	EXPECT_EQ(RainSun, bc[4][2]);
	EXPECT_EQ(RainSun, bc[4][3]);
}

TEST_F(SymbolGenerateTest, getProbabilities)
{
	// Result
	BlobCollectionPtr result = weatherSymbol::generateSymbols(* cloudCover, * precipitation);
	CrossContainer<float> cc(*result, 0);

	weatherSymbol::SymbolProbability sp;
	weatherSymbol::getSymbolProbability(cc.begin(), cc.end(), & sp);

	EXPECT_EQ(3u, sp.size());
	EXPECT_EQ(double(0.2), sp[weather_symbol::Sun]);
	EXPECT_EQ(double(0.4), sp[weather_symbol::RainSun]);
	EXPECT_EQ(double(0.4), sp[weather_symbol::Rain]);

	// Remove this test
}

TEST_F(SymbolGenerateTest, getSymbolUncertainty)
{
	BlobContainer symbol;
	BlobContainer certainty;

	weatherSymbol::getSymbolCertainty(* cloudCover, * precipitation, & symbol, & certainty);

	ASSERT_EQ(gridSize, symbol.size());
	ASSERT_EQ(gridSize, certainty.size());

	using namespace weather_symbol;
	EXPECT_EQ(Rain, (Code) symbol[0]);
	EXPECT_FLOAT_EQ(40, certainty[0]);
}

TEST(SymbolSelectionTest, generic)
{
	using namespace weather_symbol;
	Code s[5] = {Sun, LightRainSun, Sun, Rain, Sun};

	Code chosen = weatherSymbol::selectSymbol(s, s +5);
	EXPECT_EQ(Sun, chosen);
}

TEST(SymbolSelectionTest, mostFrequentInWrongGroup)
{
	using namespace weather_symbol;
	Code s[11] = {
			Sun, Sun, Sun, Cloud, Cloud, LightCloud, // 6
			LightRainSun, Rain, Rain, Rain, Rain // 5
	};

	Code chosen = weatherSymbol::selectSymbol(s, s + 11);
	EXPECT_EQ(Sun, chosen);
}


TEST(SymbolCertaintyTest, generic)
{
	using namespace weather_symbol;
	Code s[5] = {Sun, PartlyCloud, Sun, Rain, Sun};

	Code chosen = weatherSymbol::selectSymbol(s, s +5);
	EXPECT_EQ(Sun, chosen);

	float certainty = weatherSymbol::getProbabilityOf(s, s + 5, chosen);
	EXPECT_FLOAT_EQ(60, certainty);
}
