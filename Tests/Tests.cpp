// Tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"

#include "StringRow.h"
#include "../Grid.h"
#include "../SolveRow.h"
#include "../Utils.h"

#define BOOST_TEST_MODULE GriddlerSolver
#include <boost/test/included/unit_test.hpp>

using Grid::CValue;
using Row::TStringRow;
using Solver::SolveRow;
using Utils::Check;
using Utils::IntSize;
using Grid::LoadFromString;
using Grid::SaveToString;

struct CTestCase
{
	std::string In;
	std::vector<int> Nums;
	std::string Expected;
};



void TestRow(const CTestCase& tc)
{
	std::vector<CValue> Input = LoadFromString(tc.In), Expected;
	Row::CStringRowData d{ tc.Nums, Input };
	TStringRow sr{ d, 1, Utils::IntSize(Input), {0,0} };
	Solver::CContext c;
	Result::CVoid r = SolveRow(sr, c);

	if (tc.Expected == "E")
	{		
		BOOST_CHECK(!r);
		return;
	}

	Expected = LoadFromString(tc.Expected);
	BOOST_CHECK(r);
	Check(Input.size() == Expected.size(), "Input and expected values must have same size: ", Input.size(), "!=", Expected.size());
	std::string vals = SaveToString(sr.Values());
	BOOST_CHECK_EQUAL(tc.Expected, vals);
}

BOOST_AUTO_TEST_SUITE(Formats)

BOOST_AUTO_TEST_CASE(TestPosition)
{
	Row::CPosition pos{ 2,3 };
	auto s = std::format("{}", pos);
	BOOST_REQUIRE_EQUAL(s, "[2, 3]");
}

BOOST_AUTO_TEST_CASE(TestInterval)
{
	Solver::CInterval i{ 0,5 };
	auto s = std::format("{}", i);
	BOOST_REQUIRE_EQUAL(s, "<0, 5)");
}

BOOST_AUTO_TEST_CASE(TestRow)
{
	auto values = LoadFromString("OO");
	Row::CStringRowData d{ {2}, values };
	TStringRow sr{ d, 1, Utils::IntSize(d.Values()), {0,0} };
	const TStringRow& cr = sr;
	std::string s = std::format("{}", cr);
	BOOST_REQUIRE_EQUAL(s, "R[0, 0]:2\r\n01\r\nOO\r\n");
}

BOOST_AUTO_TEST_CASE(TestLine)
{
	Solver::CLine l;
	l.Crosses = { {1,2}, {3,2} };
	l.Numbers = { {2, {1,3}}, {3, {4,7}} };
	l.Blacks = { {{1,2}, {1, 2, 3}}, {{1,2}, {1, 2, 3}} };
	std::string s = std::format("{}", l);
	BOOST_REQUIRE(!s.empty());
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Basics)

BOOST_AUTO_TEST_CASE(AllBlacks)
{
	TestRow({
		"OO",
		{2},
		"BB",
		});
}

BOOST_AUTO_TEST_CASE(AllCrosses)
{
	TestRow({
		"OO",
		{},
		"XX",
		});
}

BOOST_AUTO_TEST_CASE(TooFewInfo)
{
	TestRow({
		"OOOO",
		{2},
		"OOOO",
	});
}

BOOST_AUTO_TEST_CASE(SomeBlacks)
{
	TestRow({
		"OOOOO",
		{3},
		"OOBOO",
		});
}

BOOST_AUTO_TEST_CASE(BlackAtTheEnd)
{
	TestRow({
		"OOOBB",
		{3},
		"XXBBB",
		});
}



BOOST_AUTO_TEST_CASE(TwoBlacks)
{
	TestRow({
		"OBOBB",
		{1, 2},
		"XBXBB",
		});
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Advanced)

BOOST_AUTO_TEST_CASE(ConnectCrossesThatAreTooClose)
{
	TestRow({
		"OXOOXOOXOOOOOO",
		{3},
		"XXXXXXXXOOOOOO",
		});
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Contradictions)

BOOST_AUTO_TEST_CASE(TooBigNumber)
{
	TestRow({
		"OO",
		{3},
		"E",
		});
}


BOOST_AUTO_TEST_SUITE_END()
