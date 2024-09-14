#pragma once

#include "Grid.h"

#include <iterator>
#include <vector>
#include "Utils.h"

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

		int Left() const
		{
			return m_Left;
		}

		int Right() const
		{
			return m_Right;
		}

		int Size() const
		{
			return m_Right - m_Left;
		}

		bool IsEmpty() const
		{
			return m_Left == m_Right;
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
			res.push_back(Solver::CInterval(Utils::ToInt(itVal - begin), Utils::ToInt(it - begin)));
		}
	}

	struct CNumber
	{
		CNumber() = default;
		CNumber(int v, CInterval i) : Value(v), Interval(i)
		{

		}
		int Value;
		CInterval Interval;
		std::vector<int> Blacks;
	};
	using CNumbers = std::vector<CNumber>;

	struct CBlack
	{
		CBlack(CInterval i) :
			Interval(i)
		{

		}

		CBlack(CInterval i, std::vector<int> Ns) :
			Interval(i), Nums(Ns)
		{

		}

		CBlack() = default;
		CInterval Interval;
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


template<>
struct std::formatter<Solver::CInterval, char>
{

	template<class ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	template<class FmtContext>
	FmtContext::iterator format(Solver::CInterval i, FmtContext& ctx) const
	{
		return std::format_to(ctx.out(), "<{},{})", i.Left(), i.Right());
	}
};


template<>
struct std::formatter<Solver::CNumber, char>
{

	template<class ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	template<class FmtContext>
	FmtContext::iterator format(const Solver::CNumber& n, FmtContext& ctx) const
	{
		return std::format_to(ctx.out(), "#{}:{}, Bs={}", n.Value, n.Interval, n.Blacks);
	}
};


template<>
struct std::formatter<Solver::CBlack, char>
{

	template<class ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	template<class FmtContext>
	FmtContext::iterator format(const Solver::CBlack& b, FmtContext& ctx) const
	{
		return std::format_to(ctx.out(), "B:{}, #s={}", b.Interval, b.Nums);
	}
};

template<>
struct std::formatter<Solver::CLine, char>
{

	template<class ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	template<class FmtContext>
	FmtContext::iterator format(const Solver::CLine &l, FmtContext& ctx) const
	{
		return std::format_to(ctx.out(), "B={}, X={}, Ns={})", l.Blacks, l.Crosses, l.Numbers);
	}
};
