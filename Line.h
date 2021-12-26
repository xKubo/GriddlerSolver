#pragma once

#include "Grid.h"

#include <iterator>
#include <vector>

namespace Solver
{

	template <typename T>
	struct Not
	{
		Not() = default;

		Not(T v) : m_Value(v)
		{

		}

		bool operator()(T v) const
		{
			return v != m_Value;
		}

	private:
		T m_Value;
	};

	using Grid::CValue;

	struct CInterval
	{
		CInterval(int Left, int Right) :
			m_Left(Left),
			m_Right(Right)
		{}

		CInterval() = default;

		int left() const
		{
			return m_Left;
		}

		int right() const
		{
			return m_Right;
		}

	private:
		int m_Left = 0, m_Right = 0;
	};

	using CIntervals = std::vector<CInterval>;

	template <typename TIterator>
	inline CIntervals FindRuns(TIterator begin, TIterator end, CValue v)
	{
		TIterator it = begin;
		CIntervals res;
		for (;;)
		{
			auto itVal = std::find(it, end, v);
			if (itVal == end)
				return res;
			it = std::find_if(itVal, end, Not(v));
			res.push_back({itVal - begin, it - begin});
		}
	}

	struct CNumber
	{
		CNumber() = default;
		CNumber(int v, CInterval i) : Value(v), Num(i)
		{

		}
		int Value;
		CInterval Num;
		std::vector<int> Blacks;
	};
	using CNumbers = std::vector<CNumber>;

	struct CBlack
	{
		CBlack(CInterval i) :
			Black(i)
		{

		}

		CBlack() = default;
		CInterval Black;
		std::vector<int> Nums;
	};
	using CBlacks = std::vector<CBlack>;

	struct CLine
	{
		CIntervals Crosses;
		CNumbers Numbers;
		CBlacks Blacks;
	};



}
