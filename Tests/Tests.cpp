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

struct CTestCase
{
	std::string In;
	std::vector<int> Nums;
	std::string Expected;
};

std::string sVals = "OXB";

std::vector<CValue> LoadFromString(const std::string& str)
{
	Grid::CValues vs;
	for (char c : str)
	{
		auto pos = sVals.find(c);
		Check(pos != -1, "Invalid value char: ", c);
		vs.push_back(static_cast<CValue>(pos));
	}
	return vs;
}

std::string SaveToString(std::span<CValue> vals)
{
	std::string res;
	for (CValue v : vals)
	{
		int pos = static_cast<int>(v);
		Check(pos < IntSize(sVals), "Unexpected value: ", pos);
		res.push_back(sVals[pos]);
	}
	return res;
}

void TestRow(const CTestCase& tc)
{
	std::vector<CValue> Input = LoadFromString(tc.In), Expected;

	if (tc.Expected != "E")
		Expected = LoadFromString(tc.Expected);

	Row::CStringRowData d{ tc.Nums, Input };

	TStringRow sr{ d, 1, Utils::IntSize(Input), {0,0} };
	Result::CVoid r = SolveRow(sr);
	if (tc.Expected == "E")
	{
		BOOST_CHECK(!r);
		return;
	}

	BOOST_CHECK(r);
	Check(Input.size() == Expected.size(), "Input and expected values must have same size: ", Input.size(), "!=", Expected.size());
	std::string vals = SaveToString(sr.Values());
	BOOST_CHECK_EQUAL(tc.Expected, vals);
}

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
