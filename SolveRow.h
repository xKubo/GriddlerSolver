#pragma once
#include <ranges>
#include <optional>

#include "Utils.h"

#include "Row.h"
#include "Grid.h"

#include "Result.h"

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

	template <view TValues, view TNumbers, view TLefts, view TRights>
	requires 
		std::is_same_v<range_value_t<TValues>, CValue> &&
		std::integral<range_value_t<TNumbers>>
	inline CVoid SolveOnePart(TValues vs, TNumbers nums, TLefts Lefts, TRights Rights)
	{
		using namespace std;
		using namespace std::ranges;
		auto iBeg = std::begin(vs);
		auto iEnd = std::end(vs);

		CVoid res;

		for (int i = 0; i < IntSize(nums); ++i)
		{
			bool IsLast = i == nums.size() - 1;
			int n = nums[i];
			optional<int>& l = Lefts[i];
			optional<int>& r = Rights[i];

			if (std::distance(iBeg, iEnd) < n)
				return ErrorCode::LineTooSmall;

			// find X in first n values
			auto itS = std::find(iBeg, iBeg + n, Grid::CValue::Cross);
			if (itS != iBeg + n)
			{
				res = Mark(iBeg, itS, CValue::Cross);
				if (!res) return res;
				iBeg = itS;
				if (std::distance(iBeg, iEnd) < n)
					return ErrorCode::LineTooSmall;
			}

			// find and mark possible blacks
			auto iBlackEnd = std::find(iBeg + n, iEnd, Grid::CValue::Black);
			if (iBlackEnd != iEnd)		 // we found some blacks after the number
			{
				// mark blacks after the block
				auto iBlackBeg = std::find(iBeg, iBlackEnd, Grid::CValue::Black);
				res = Mark(iBlackBeg, iBlackEnd, Grid::CValue::Cross);
				if (!res) return res;

				// mark crosses at the beginning
				auto iMin = iBlackEnd - n;
				res = Mark(iBeg, iMin, Grid::CValue::Cross);
				if (!res) return res;				

				l = iMin - std::begin(vs);
			}
			else
				l = iBeg - std::begin(vs);

			

			// if we are not at the end move one field up
			if (iBlackEnd == iEnd)
			{
				if (IsLast)
					return ErrorCode::NoError;
				else
					return ErrorCode::LineTooSmall;				
			}
			
			++iBlackEnd;

			r = iBlackEnd - std::begin(vs);

			iBeg = iBlackEnd;
		}

		return ErrorCode::NoError;
	}

	template <view TValues, view TNumbers, view TLefts, view TRights>
	inline CVoid EvaluateLeftsAndRights(TValues vs, TNumbers nums, TLefts Lefts, TRights Rights)
	{
		CVoid res;
		for (int i = 0; i < IntSize(nums); ++i)
		{
			optional<int>& left = Lefts[i];
			optional<int>& right = Rights[Rights.size() - 1 - i];

			int n = nums[i];

			if (left && right)
			{
				int l = *left;
				int r = size(vs) - *right;		// the right value must be reversed - because it was generated from the other direction
				auto iBeg = begin(vs) + l;
				auto iEnd = begin(vs) + r;

				int d = r - l;

				int BlackFields = 2 * n - d;
				auto iStart = iBeg + n - d;
				if (BlackFields > 0)
				{
					res = Mark(iStart, iStart + BlackFields, CValue::Black);
					if (!res) return res;
				}
			}

			

			if (right)
			{
				int r = size(vs) - *right;
				int EndOfCrosses;
				bool IsLast = i == IntSize(nums) - 1;
				
				if (IsLast)
					EndOfCrosses = IntSize(vs);
				else
				{
					if (!Lefts[i+1])
						continue;
					EndOfCrosses = *Lefts[i+1];
				}

				res = Mark(begin(vs) + r, begin(vs) + EndOfCrosses, CValue::Cross);
				if (!res) return res;
			}

		}
		return Result::ErrorCode::NoError;
	}

	template <typename TRow>
	inline CVoid SolveRow(TRow &Input)
	{
		int NumSize = Input.Numbers().size();
		if (NumSize == 0)
			return Mark(Input.Vals().begin(), Input.Vals().end(), Grid::CValue::Cross);
		std::vector<std::optional<int>> Lefts(NumSize), Rights(NumSize);

		auto vLefts = subrange(Lefts);
		auto vRRights = subrange(Rights.rbegin(), Rights.rend());

		CVoid r = SolveOnePart(Input.Vals(), Input.Numbers(), vLefts, vRRights);
		if (!r) return r;
		r = SolveOnePart(Input.RVals(), Input.RNumbers(), vRRights, vLefts);
		if (!r) return r;
		r = EvaluateLeftsAndRights(Input.Vals(), Input.Numbers(), vLefts, subrange(Rights));
		if (!r) return r;
		return ErrorCode::NoError;

	}
}