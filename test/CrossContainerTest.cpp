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
#include <container/CrossContainer.h>
#include <algorithm>

using namespace std;

typedef CrossContainer<int> IntCrossContainer;

class CrossContainerTest : public testing::Test
{
protected:
	virtual void SetUp()
	{
		contents = new IntCrossContainer::Container;
		for ( int i = 10; i; -- i )
		{
			IntCrossContainer::InternalContainer f;
			for ( int j = 10; j; -- j )
				f.push_back((i*10) - j);
			contents->push_back(f);
		}

		cc = new IntCrossContainer(* contents, 3);
		/*
		 * Contents now are
		 * 90 91 ... 99
		 * 80 81 ... 89
		 * etc..
		 */

		begin = cc->begin();
	}

	virtual void TearDown()
	{
		delete cc;
		delete contents;
	}

	IntCrossContainer::Container * contents;
	IntCrossContainer * cc;
	IntCrossContainer::const_iterator begin;
};

TEST_F(CrossContainerTest, testIterate)
{
	int expected = 93;

	for ( IntCrossContainer::const_iterator it = begin; it != cc->end(); ++ it )
	{
		ASSERT_EQ(expected, * it);
		expected -= 10;
	}
}

TEST_F(CrossContainerTest, testRandomAccessAlgorithm)
{
	sort(cc->begin(), cc->end());

	int expected = 3;
	for ( IntCrossContainer::const_iterator it = begin; it != cc->end(); ++ it )
	{
		ASSERT_EQ(expected, * it);
		expected += 10;
	}

	// Check that we have not modified any other data
	IntCrossContainer cc2(* contents, 2);
	expected = 92;
	for ( IntCrossContainer::const_iterator it = cc2.begin(); it != cc2.end(); ++ it )
	{
		ASSERT_EQ(expected, * it);
		expected -= 10;
	}
}

TEST_F(CrossContainerTest, dereference)
{
	ASSERT_EQ(93, * begin );
}

TEST_F(CrossContainerTest, accessMember)
{
	// To be added
}

TEST_F(CrossContainerTest, constConversion)
{
	IntCrossContainer::const_iterator cit = begin;
}

TEST_F(CrossContainerTest, arrayAccess)
{
	ASSERT_EQ(73, begin[2]);
}

TEST_F(CrossContainerTest, measureDistance)
{
	IntCrossContainer::const_iterator start = begin + 3;
	IntCrossContainer::const_iterator stop = cc->end() - 4;
	ASSERT_EQ(3, stop - start);
}

TEST_F(CrossContainerTest, measureDistanceFromEnd)
{
	IntCrossContainer::const_iterator stop = cc->end();
	ASSERT_EQ(3, stop - (begin + 7));
}

TEST_F(CrossContainerTest, measureDistanceFromBegin)
{
	ASSERT_EQ(7, (begin +7) - begin);
}

TEST_F(CrossContainerTest, measureDistanceFromBeginToEnd)
{
	ASSERT_EQ(10, cc->end() - cc->begin());
}

TEST_F(CrossContainerTest, preIncrement)
{
	ASSERT_EQ(83, * (++begin));
}

TEST_F(CrossContainerTest, postIncrement)
{
	EXPECT_EQ(93, *(begin ++));
	ASSERT_EQ(83, * begin);
}

TEST_F(CrossContainerTest, preDecrement)
{
	++ begin;
	ASSERT_EQ(93, * (--begin));
}

TEST_F(CrossContainerTest, postDecrement)
{
	++ begin;
	ASSERT_EQ(83, * (begin--));
	ASSERT_EQ(93, * begin);
}

TEST_F(CrossContainerTest, add)
{
	IntCrossContainer::const_iterator newPos = begin + 3;
	ASSERT_EQ(63, * newPos);
}

TEST_F(CrossContainerTest, subtract)
{
	begin = begin + 5;
	IntCrossContainer::const_iterator newPos = begin - 3;
	ASSERT_EQ(73, * newPos);
}

TEST_F(CrossContainerTest, inplaceAdd)
{
	begin += 4;
	ASSERT_EQ(53, * begin);
}

TEST_F(CrossContainerTest, inplaceSubtract)
{
	begin += 8;
	begin -= 5;
	ASSERT_EQ(63, * begin);
}

TEST_F(CrossContainerTest, equal)
{
	EXPECT_TRUE(begin == cc->begin());
	EXPECT_FALSE(begin == cc->end());
	EXPECT_FALSE(++ begin == cc->begin());
}

TEST_F(CrossContainerTest, notEqual)
{
	EXPECT_FALSE(begin != cc->begin());
	EXPECT_TRUE(begin != cc->end());
	EXPECT_TRUE(++ begin != cc->begin());
}

TEST_F(CrossContainerTest, less)
{
	EXPECT_TRUE(begin < cc->end());
	EXPECT_FALSE(cc->end() < cc->begin());
	EXPECT_TRUE(cc->begin() < ++ cc->begin());
}
