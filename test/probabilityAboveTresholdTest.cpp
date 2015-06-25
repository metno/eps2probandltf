/*
 probabilityForecast

 Copyright (C) 2009 met.no

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
#include <generator/algorithms/probabilityAboveTreshold.h>
#include <boost/assign/list_of.hpp>

TEST(ProbabilityAboveTest, oddNumberOfSamples)
{
	std::vector<int> vals = boost::assign::list_of(1)(2)(5)(7)(9);

	ASSERT_DOUBLE_EQ(40, chanceOfValueAbove(vals.begin(), vals.end(), 6));
}

TEST(ProbabilityAboveTest, evenNumberOfSamples)
{
	std::vector<int> vals = boost::assign::list_of(1)(2)(5)(7);

	ASSERT_DOUBLE_EQ(25, chanceOfValueAbove(vals.begin(), vals.end(), 6));
}

TEST(ProbabilityAboveTest, allMembersGreater)
{
	std::vector<int> vals = boost::assign::list_of(1)(2)(5)(7)(9);

	ASSERT_DOUBLE_EQ(100, chanceOfValueAbove(vals.begin(), vals.end(), 0));
}

TEST(ProbabilityAboveTest, noMembersGreater)
{
	std::vector<int> vals = boost::assign::list_of(1)(2)(5)(7)(9);

	ASSERT_DOUBLE_EQ(0, chanceOfValueAbove(vals.begin(), vals.end(), 9));
}

TEST(ProbabilityAboveTest, severalValuesAtThreshold)
{
	std::vector<int> vals = boost::assign::list_of(1)(5)(5)(5)(9);
	ASSERT_DOUBLE_EQ(20, chanceOfValueAbove(vals.begin(), vals.end(), 5));
}
