#pragma once
#include <ranges>
#include <optional>

#include "Utils.h"

#include "Row.h"
#include "Grid.h"

#include "Result.h"

#include "Line.h"
#include "Changes.h"

#include "Diag.h"

namespace Solver
{
	using Row::CRow;
	using Grid::CValue;

	using std::optional;
	using namespace std::ranges;
	using namespace Result;

	using Utils::IntSize;

	template <typename TIt>
	inline CVoid Mark(TIt From, TIt To, CValue val, CChanges<TIt> &chs)
	{
		for (TIt it = From; it != To; ++it)
		{
			if (*it == CValue::None)
			{
				chs.Add(it);
				*it = val;
			}
			else if (*it == val)
			{
		
			}
			else
				return ErrorCode::AlreadySpecified;			
		}
		return ErrorCode::NoError;
	}

	template <typename TIt>
	inline CVoid Mark(TIt At, CValue val, CChanges<TIt>& chs)
	{
		return Mark(At, At + 1, val, chs);
	}


	template <view TValues, view TNumbers, view TLefts>
	requires 
		std::is_same_v<range_value_t<TValues>, CValue> &&
		std::integral<range_value_t<TNumbers>>
	inline CVoid SolveInOneDir(TValues vs, TNumbers nums, TLefts Lefts, auto& chs)
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
				res = Mark(iBeg, itS, CValue::Cross, chs);
				if (!res) return res;
				iBeg = itS;
				continue;
			}

			l = Utils::ToInt(std::distance(begin(vs), iBeg));

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


	template <view TValues, view TNumbers, view TLefts>
	inline Result::CResult<CLine> PrepareLineInfo(TValues vs, TNumbers nums, TLefts Lefts, view auto Rights, auto& chs)
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
			
			int Left = Lefts[i];
			int reversedright = Rights[Rights.size() - 1 - i];
			int Right = IntSize(vs) - reversedright;
			int n = nums[i];
			l.Numbers.push_back(CNumber{ n, {Left, Right} });
		}
		return l;
	}

	inline CInterval Intersect(CInterval i1, CInterval i2)
	{
		int l = std::max(i1.Left(), i2.Left());
		int r = std::min(i1.Right(), i2.Right());
		if (r < l)
			return { };
		return {l, r};
	}

	template <typename TValues>
	CVoid EvaluateLine(TValues Vals, CLine& line, auto& chs)
	{
		auto ValsBegin = begin(Vals);
		auto ValsEnd = end(Vals);
		int LineSize = Utils::ToInt(ValsEnd - ValsBegin);
		int MinVal = IntSize(Vals);
		for (int i = 0; i<IntSize(line.Numbers); ++i)
		{
			CNumber& n = line.Numbers[i];

			MinVal = std::min(n.Value, MinVal);
			
			int l = n.Interval.Left();
			int r = n.Interval.Right();
			int sz = n.Interval.Size();
			if (sz < n.Value)
				return ErrorCode::LineTooSmall;



			if (sz == n.Value)  // exact fit - we can make cross before and after black - if possible
			{
				if (l != 0)
				{
					auto res = Mark(ValsBegin + l - 1, CValue::Cross, chs);
					if (!res) return res;
				}

				if (r < LineSize - 1)
				{
					auto res = Mark(ValsBegin + r, CValue::Cross, chs);
					if (!res) return res;
				}
			}
			else if (sz < 2 * n.Value)  // left and right overlap - there are some black squares
			{
				int blackstart = l + sz - n.Value;
				int blacksize = 2 * n.Value - sz;
				auto res = Mark(ValsBegin + blackstart, ValsBegin + blackstart + blacksize, CValue::Black, chs);
				if (!res) return res;
			}
			else
			{
				// left and right do not overlap
			}

			// assign to each num its possible blacks
			for (int bi = 0; bi < IntSize(line.Blacks); ++bi)
			{
				auto& b = line.Blacks[bi];
				if (!Intersect(b.Interval, n.Interval).IsEmpty())
				{
					b.Nums.push_back(i);
					n.Blacks.push_back(bi);
				}
			}
		}

		// if black has only one num, we can adjust l, r for that number
		for (int bi = 0; bi < IntSize(line.Blacks); ++bi)
		{
			CBlack& b = line.Blacks[bi];
			if (b.Nums.size() != 1)
				continue;

			CNumber& n = line.Numbers[b.Nums[0]];
			int l = n.Interval.Left();
			l = std::max(l, b.Interval.Right() - n.Value);
			int r = n.Interval.Right();
			r = std::min(r, b.Interval.Left() + n.Value);
			n.Interval = CInterval{l, r};
		}


		// if there are 2 crosses closer than minval - we can connect them
		int LastCrossAt = 0;
		for (const auto& c : line.Crosses)
		{
			if (c.Left() - LastCrossAt < MinVal)
			{
				auto r = Mark(begin(Vals) + LastCrossAt, begin(Vals) + c.Left(), CValue::Cross, chs);
				if (!r) return r.Code();
			}
				
			LastCrossAt = c.Right();			
		}

		// we have to find all possible coverages for blacks
		// we try to infer limitations of the number positions

		// grid solver state - 
		// remember all guesses (either some field X/O, or black belongs to this number)
		// run iterations in parallel - if contradiction found, discard, and mark the opposite in real solution




		return ErrorCode::NoError;
	}

	struct CContext
	{
		CContext()
		{
			m_Diag = Diag::CreateConsoleDiag("");
		}

		Diag::CDiag& Diag()
		{
			return m_Diag;
		}
	private:
		Diag::CDiag m_Diag;
	};

	template <typename TRow>
	inline CVoid SolveRow(TRow &Input, CContext& c)
	{

		int NumSize = IntSize(Input.Numbers());
		if (NumSize == 0)
		{
			CChanges chs(Input.Vals().begin());
			return Mark(Input.Vals().begin(), Input.Vals().end(), Grid::CValue::Cross, chs);
		}
		std::vector<int> Lefts(NumSize), Rights(NumSize);

		auto vLefts = subrange(Lefts);
		auto vRRights = subrange(Rights.rbegin(), Rights.rend());

		for (;;)
		{
			c.Diag().fmtI("{}", Input);
			CChanges chs(Input.Vals().begin());

			CVoid r = SolveInOneDir(Input.Vals(), Input.Numbers(), vLefts, chs);
			if (!r) return r;

			r = SolveInOneDir(Input.RVals(), Input.RNumbers(), vRRights, chs);   // BUG: chs should be reversed, too ... 
			if (!r) return r;

			auto rLine = PrepareLineInfo(Input.Vals(), Input.Numbers(), vLefts, subrange(Rights), chs);
			if (!rLine) return rLine.Code();


			CLine& line = rLine.Result();

			for (;;)
			{
				
				c.Diag().fmtI("{}", line);
				int ChangeCount = chs.Count();

				auto rEval = EvaluateLine(Input.Vals(), line, chs);
				if (!rEval) return rEval.Code();
				
				if (chs.Count() == ChangeCount)
					break;
			}

			if (chs.IsEmpty())
				return ErrorCode::NoError;

		}

	}
}