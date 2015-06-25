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

#include <gtest/gtest.h>
#include <generator/symbol/SymbolGenerator.h>
#include <generator/algorithm.h>
#include <boost/assign/std/vector.hpp>
#include <vector>
#include <functional>
#include <iostream>

using namespace boost::assign;
using namespace weather_symbol;
using namespace weatherSymbol;

TEST(RealWorldTemperatureTest, DISABLED_getTemperature)
{
	std::vector<float> clouds;
	clouds += 1, 1, 1, 1, 1, 1, 1, 0.976562, 1, 0.671875, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.664062, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.90625, 0.96875, 1, 1, 1;
	ASSERT_EQ(51, clouds.size());

	std::vector<float> precipitation;
	precipitation += 0.0325584, 0.0290642, 0.0298176, 0.03018, 0.0375786, 0.0285835, 0.029892, 0.0324974, 0.0351772, 0.0309105, 0.0289764, 0.0272427, 0.0343246, 0.037014, 0.0340996, 0.0367622, 0.0257282, 0.0323429, 0.0311432, 0.0337029, 0.0332813, 0.0324154, 0.0313988, 0.0369225, 0.032196, 0.0271873, 0.031229, 0.0394745, 0.023901, 0.0361481, 0.0295677, 0.0300007, 0.0340099, 0.0377426, 0.0325565, 0.0275288, 0.0361252, 0.0362244, 0.0309563, 0.0286388, 0.0374107, 0.0301838, 0.0266342, 0.0312996, 0.032423, 0.0295162, 0.0318756, 0.0307407, 0.0316315, 0.0353737, 0.0314159;
	ASSERT_EQ(51, precipitation.size());
//	for ( std::vector<float>::iterator it = precipitation.begin(); it != precipitation.end(); ++ it )
//		* it /= 100;

	std::vector<float> symbols;
	getSymbols(clouds.begin(), clouds.end(), precipitation.begin(), std::back_inserter(symbols));
	ASSERT_EQ(51, symbols.size());

	weather_symbol::Code s = selectSymbol(symbols.begin(), symbols.end());
	EXPECT_EQ(Rain, s);

	float probability = getProbabilityOf(symbols.begin(), symbols.end(), s);
	EXPECT_EQ(0.67, probability);

//	for ( std::vector<float>::const_iterator it = symbols.begin(); it != symbols.end(); ++ it )
//		std::cout << * it << std::endl;

	std::vector<float> temperatures;
	temperatures += -10.866, -8.62, -7.5, -8.928, -9.423, -5.75599999999997, -9.14299999999997, -8.90499999999997, -9.43099999999998, -9.87, -6.39699999999999, -9.51399999999995, -4.56, -7.77499999999998, -8.63799999999998, -10.391, -9.31799999999998, -9.95299999999997, -5.06599999999997, -9.78899999999999, -6.81599999999997, -6.81099999999998, -10.76, -5.99299999999999, -9.30699999999996, -7.63399999999996, -6.84799999999996, -6.94099999999997, -9.39599999999996, -7.82999999999998, -9.33199999999999, -11.353, -7.53699999999998, -8.541, -10.597, -7.69199999999995, -8.483, -10.067, -10.03, -3.60999999999996, -9.56599999999997, -9.322, -9.88999999999999, -8.31999999999999, -9.92999999999995, -6.22499999999997, -10.179, -7.79599999999999, -8.15899999999999, -7.61099999999999, -8.14699999999999;
	ASSERT_EQ(51, temperatures.size());

	float temperature = getProbableValue<float>(s, symbols.begin(), symbols.end(), temperatures.begin(), std::less<float>());

	EXPECT_EQ(-8.638, temperature);
}


