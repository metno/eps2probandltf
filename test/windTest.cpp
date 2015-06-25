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
#include <generator/wind/windInternal.h>
#include <generator/algorithm.h>
#include <generator/symbol/SymbolGenerator.h>

#include <iostream>


class WindTest : public testing::Test
{
public:
	void SetUp()
	{
		const int fieldCount = 4;
		fields = fieldCount;

		windU = new type::BlobCollection(samples);
		windV = new type::BlobCollection(samples);
		(*windU)[0].push_back(1);
		(*windU)[1].push_back(-1);
		(*windU)[2].push_back(1);
		(*windU)[3].push_back(-1);
		(*windU)[4].push_back(-1);

		(*windV)[0].push_back(-1);
		(*windV)[1].push_back(0);
		(*windV)[2].push_back(3);
		(*windV)[3].push_back(4);
		(*windV)[4].push_back(3);


		(*windU)[0].push_back(1);
		(*windU)[1].push_back(11);
		(*windU)[2].push_back(-2);
		(*windU)[3].push_back(3);
		(*windU)[4].push_back(2);

		(*windV)[0].push_back(1);
		(*windV)[1].push_back(0);
		(*windV)[2].push_back(2);
		(*windV)[3].push_back(-3.7);
		(*windV)[4].push_back(-2);


		(*windU)[0].push_back(7);
		(*windU)[1].push_back(-1);
		(*windU)[2].push_back(3);
		(*windU)[3].push_back(2);
		(*windU)[4].push_back(7);

		(*windV)[0].push_back(-7);
		(*windV)[1].push_back(1);
		(*windV)[2].push_back(2);
		(*windV)[3].push_back(9);
		(*windV)[4].push_back(-2);


		(*windU)[0].push_back(0);
		(*windU)[1].push_back(-2);
		(*windU)[2].push_back(-0.1);
		(*windU)[3].push_back(3);
		(*windU)[4].push_back(-5);

		(*windV)[0].push_back(-4);
		(*windV)[1].push_back(2);
		(*windV)[2].push_back(-1);
		(*windV)[3].push_back(5);
		(*windV)[4].push_back(2);
	}

	void TearDown()
	{
		delete windV;
		delete windU;
	}

protected:
	type::BlobCollection * windU;
	type::BlobCollection * windV;

	const type::BlobContainer * selectedSymbols;
	type::BlobCollection * allSymbols;

	static const int samples;
	int fields;
};
const int WindTest::samples = 5;

TEST_F(WindTest, DISABLED_selectWind)
{
	using namespace weather_symbol;

	type::BlobContainer selectedSymbols;
	type::BlobCollection allSymbols(5, type::BlobContainer());
	allSymbols[0].push_back(PartlyCloud);
	allSymbols[1].push_back(Cloud);
	allSymbols[2].push_back(LightRain);
	allSymbols[3].push_back(PartlyCloud);
	allSymbols[4].push_back(PartlyCloud);

	allSymbols[0].push_back(Sun);
	allSymbols[1].push_back(Sun);
	allSymbols[2].push_back(Cloud);
	allSymbols[3].push_back(PartlyCloud);
	allSymbols[4].push_back(LightRain);

	allSymbols[0].push_back(Sun);
	allSymbols[1].push_back(LightRainSun);
	allSymbols[2].push_back(PartlyCloud);
	allSymbols[3].push_back(Rain);
	allSymbols[4].push_back(PartlyCloud);

	allSymbols[0].push_back(Rain);
	allSymbols[1].push_back(PartlyCloud);
	allSymbols[2].push_back(Rain);
	allSymbols[3].push_back(PartlyCloud);
	allSymbols[4].push_back(Rain);

	// setup allSymbols
	for ( unsigned i = 0; i < allSymbols.size(); ++ i )
	{
		CrossContainer<float> symbolSamples(allSymbols, i);
		selectedSymbols.push_back(weatherSymbol::selectSymbol(symbolSamples.begin(), symbolSamples.end()));
	}

	type::WindContainer result;
	type::WindCollectionPtr wind = getWind(*windU, * windV);
	getProbableValues(selectedSymbols, allSymbols, * wind, & result, WindForceLess());

	//	selectWind(selectedSymbols, allSymbols, * wind, & result);

	ASSERT_EQ(4u, result.size());
	// Should be uncommented, but for some reason it does not compile..
//	EXPECT_EQ(Wind(1.618034,-1.618034), result[0]);
//	EXPECT_EQ(Wind(-2.68412,2.68412), result[1]);
//	EXPECT_EQ(Wind(7,-2), result[2]);
//	EXPECT_EQ(Wind(0,-4), result[3]);
}

TEST_F(WindTest, getForceProbability)
{
	type::WindCollectionPtr wind = getWind(*windU, * windV);
	CrossContainer<Wind> point0(* wind, 0);
	EXPECT_FLOAT_EQ(60, getForceProbability(point0, Wind(-3, 2)));

	CrossContainer<Wind> point1(* wind, 1);
	EXPECT_FLOAT_EQ(20, getForceProbability(point1, Wind(0, -13.9)));

	CrossContainer<Wind> point2(* wind, 2);
	EXPECT_FLOAT_EQ(40, getForceProbability(point2, Wind(7, -3)));

	CrossContainer<Wind> point3(* wind, 2);
	EXPECT_FLOAT_EQ(40, getForceProbability(point3, Wind(7, -3)));
}

TEST_F(WindTest, getDirectionProbability)
{
	type::WindCollectionPtr wind = getWind(*windU, * windV);
	CrossContainer<Wind> point0(* wind, 0);
	EXPECT_FLOAT_EQ(100, getDirectionProbability(point0, Wind(0, 2)));

	CrossContainer<Wind> point1(* wind, 1);
	EXPECT_FLOAT_EQ(0, getDirectionProbability(point1, Wind(0, -13.9)));

	CrossContainer<Wind> point2(* wind, 2);
	EXPECT_FLOAT_EQ(40, getDirectionProbability(point2, Wind(7, -3)));

	CrossContainer<Wind> point3(* wind, 2);
	EXPECT_FLOAT_EQ(40, getDirectionProbability(point3, Wind(7, -3)));
}

TEST_F(WindTest, getOverallProbability)
{
	type::WindCollectionPtr wind = getWind(*windU, * windV);
	CrossContainer<Wind> point0(* wind, 0);
	EXPECT_FLOAT_EQ(60, getOverallProbability(point0, Wind(-3, 2)));

	CrossContainer<Wind> point1(* wind, 1);
	EXPECT_FLOAT_EQ(20, getOverallProbability(point1, Wind(0, -13.9)));

	CrossContainer<Wind> point2(* wind, 2);
	EXPECT_FLOAT_EQ(40, getOverallProbability(point2, Wind(7, -3)));

	CrossContainer<Wind> point3(* wind, 2);
	EXPECT_FLOAT_EQ(40, getOverallProbability(point3, Wind(7, -3)));
}


TEST(MedianTest, oddNumberOfSamples)
{
	type::BlobContainer bc;
	bc.push_back(3);
	bc.push_back(4);
	bc.push_back(4);
	bc.push_back(9);
	bc.push_back(385);

	median<float, type::BlobContainer::const_iterator> m;
	float result = m(bc.begin(), bc.end());

	ASSERT_EQ(4.0, result);
}

TEST(MedianTest, evenNumberOfSamples)
{
	type::BlobContainer bc;
	bc.push_back(4);
	bc.push_back(4);
	bc.push_back(9);
	bc.push_back(385);

	median<float, type::BlobContainer::const_iterator> m;
	float result = m(bc.begin(), bc.end());

	ASSERT_EQ(6.5, result);
}

TEST(WindDirectionTest, north)
{
	Wind w(0,1);
	ASSERT_FLOAT_EQ(90, w.direction());
}

TEST(WindDirectionTest, south)
{
	Wind w(0,-1);
	ASSERT_FLOAT_EQ(-90, w.direction());
}

TEST(WindDirectionTest, east)
{
	Wind w(1,0);
	ASSERT_FLOAT_EQ(0, w.direction());
}

TEST(WindDirectionTest, west)
{
	Wind w(-1,0);
	ASSERT_FLOAT_EQ(180, w.direction());
}

TEST(WindDirectionTest, northEast)
{
	Wind w(1,1);
	ASSERT_FLOAT_EQ(45, w.direction());
}

TEST(WindDirectionTest, northWest)
{
	Wind w(-1,1);
	ASSERT_FLOAT_EQ(135, w.direction());
}

TEST(WindDirectionTest, southEast)
{
	Wind w(1,-1);
	ASSERT_FLOAT_EQ(-45, w.direction());
}

TEST(WindDirectionTest, southWest)
{
	Wind w(-1,-1);
	ASSERT_FLOAT_EQ(-135, w.direction());
}

TEST(WindDirectionTest, regularDirectionDifference)
{
	Wind::Direction a(0);
	Wind::Direction b(90);
	ASSERT_FLOAT_EQ(-90, a.differenceTo(b));
	ASSERT_FLOAT_EQ(90, b.differenceTo(a));
}

TEST(WindDirectionTest, directionDifferenceCrosses360degrees)
{
	Wind::Direction a(0);
	Wind::Direction b(190);
	ASSERT_FLOAT_EQ(170, a.differenceTo(b));
	ASSERT_FLOAT_EQ(-170, b.differenceTo(a));
}

TEST(WindDirectionTest, bothSidesOf180Degrees)
{
	Wind::Direction a(170);
	Wind::Direction b(-170);
	ASSERT_FLOAT_EQ(-20, a.differenceTo(b));
	ASSERT_FLOAT_EQ(20, b.differenceTo(a));
}
