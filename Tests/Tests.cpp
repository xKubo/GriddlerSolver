// Tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"

#include "../Row.h"
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

struct CTestCase
{
	std::string In;
	std::vector<int> Nums;
	std::string Expected;
};

std::vector<CValue> LoadFromString(const std::string& Vals)
{
	return {};
}

std::string SaveToString(std::span<CValue> vals)
{
	return {};
}

void TestRow(const CTestCase& tc)
{
	std::vector<CValue> Input = LoadFromString(tc.In), Expected = LoadFromString(tc.Expected);

	Row::CStringRowData d{ tc.Nums, Input };

	Check(Input.size() == Expected.size(), "Input and expected values must have same size: ", Input.size(), "!=", Expected.size());
	TStringRow sr{ d, 1, Utils::IntSize(Input)};
	SolveRow(sr);
	
	std::string Result = SaveToString(d.Values());
	BOOST_CHECK_EQUAL(tc.Expected, Result);
}

BOOST_AUTO_TEST_SUITE(Basics)

BOOST_AUTO_TEST_CASE(TwoBlack)
{
	TestRow({
		"OO",
		{2},
		"BB",
	});
	BOOST_REQUIRE(1 == 1);
}

BOOST_AUTO_TEST_SUITE_END()