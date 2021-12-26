#pragma once
#include <ranges>
#include <optional>

#include "Utils.h"

#include "Row.h"
#include "Grid.h"

#include "Result.h"

#include "Line.h"

namespace Solver
{
	using Row::CRow;
	using Grid::CValue;

	using std::optional;
	using namespace std::ranges;
	using namespace Result;

	using Utils::IntSize;

	template <typename TIt>
	inline CVoid Mark(TIt From, TIt To, CValue val)
	{
		for (TIt it = From; it != To; ++it)
		{
			if (*it != CValue::None && *it != val)
				return ErrorCode::AlreadySpecified;
			*it = val;
		}
		return ErrorCode::NoError;
	}




	template <view TValues, view TNumbers, view TLefts>
	requires 
		std::is_same_v<range_value_t<TValues>, CValue> &&
		std::integral<range_value_t<TNumbers>>
	inline CVoid SolveOnePart(TValues vs, TNumbers nums, TLefts Lefts)
	{
		using namespace std::ranges;
		auto iBeg = std::begin(vs);
		auto iEnd = std::end(vs);

		CVoid res;

		for (int i = 0; i < IntSize(nums); ++i)
		{

			bool IsLast = i == nums.size() - 1;
			int n = nums[i];
			int& l = Lefts[i];

			if (std::distance(iBeg, iEnd) < n)
				return ErrorCode::LineTooSmall;

			// find X in first n values
			auto itS = std::find(iBeg, iBeg + n, Grid::CValue::Cross);
			if (itS != iBeg + n)
			{
				res = Mark(iBeg, itS, CValue::Cross);
				if (!res) return res;
				iBeg = itS;
				continue;
			}

			l = std::distance(begin(vs), iBeg);

			// find possible blacks after n squares
			auto iBlackEnd = std::find_if(iBeg + n, iEnd, Not{ CValue::Black });

			if (iBlackEnd != iEnd)
			{
				// check that there are not too many blacks after the block
				std::reverse_iterator itRBN{ iBeg + n };
				std::reverse_iterator itRBeg{ iBeg };
				auto itR = std::find_if(itRBN, itRBeg, Not{ CValue::Black });
				if (std::distance(itR, itRBN) > n)
					return Result::ErrorCode::TooManyBlacks;
				iBeg = iBlackEnd + 1;
			}
			else  // blacks are up to end
			{
				if (!IsLast)
					return Result::ErrorCode::LineTooSmall;
			}
		}

		return ErrorCode::NoError;
	}


	template <view TValues, view TNumbers, view TLefts, view TRights>
	inline Result::CResult<CLine> PrepareLineInfo(TValues vs, TNumbers nums, TLefts Lefts, TRights Rights)
	{
		CLine l;

		auto blacks = FindRuns(begin(vs), end(vs), CValue::Black);
		for (auto b : blacks)
		{
			l.Blacks.push_back(b);
		}

		l.Crosses = FindRuns(begin(vs), end(vs), CValue::Cross);

		for (int i = 0; i < IntSize(nums); ++i)
		{
			
			int left = Lefts[i];
			int reversedright = Rights[Rights.size() - 1 - i];
			int right = size(vs) - reversedright;
			int n = nums[i];
			l.Numbers.push_back(CNumber{ n, {left, right} });
		}
		return l;
	}

	template <typename TIterator>
	CVoid EvaluateLine(TIterator ValsBegin, TIterator ValsEnd, CLine& l)
	{
		return ErrorCode::NoError;
	}

	template <typename TRow>
	inline CVoid SolveRow(TRow &Input)
	{

		int NumSize = Input.Numbers().size();
		if (NumSize == 0)
			return Mark(Input.Vals().begin(), Input.Vals().end(), Grid::CValue::Cross);
		std::vector<int> Lefts(NumSize), Rights(NumSize);

		auto vLefts = subrange(Lefts);
		auto vRRights = subrange(Rights.rbegin(), Rights.rend());

		CVoid r = SolveOnePart(Input.Vals(), Input.Numbers(), vLefts);
		if (!r) return r;

		r = SolveOnePart(Input.RVals(), Input.RNumbers(), vRRights);
		if (!r) return r;

		auto rLine = PrepareLineInfo(Input.Vals(), Input.Numbers(), vLefts, subrange(Rights));
		if (!rLine) return rLine.Code();

		return EvaluateLine(Input.Vals().begin(), Input.Vals().end(), rLine.Result());
	}
}