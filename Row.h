#pragma once

#include <span>
#include <cassert>
#include <format>
#include <boost/iterator/iterator_facade.hpp>

#include "Grid.h"

namespace Row
{
	using Grid::CValue;

	template <typename TRowData>
	struct CRow;

	template <typename TRowData>
	struct TRowIterator : public boost::iterator_facade<
		TRowIterator<TRowData>,
		CValue,
		boost::random_access_traversal_tag>
	{
		TRowIterator() = default;
		TRowIterator(CValue* pVal, int Stride) :
			m_pValue(pVal),
			m_Stride(Stride)
		{

		}

		int Stride() const
		{
			return m_Stride;
		}

		CValue* Value() const
		{
			return m_pValue;
		}

	private:
		friend class boost::iterator_core_access;

		void increment() { m_pValue += m_Stride; }

		void decrement() { m_pValue -= m_Stride; }

		bool equal(TRowIterator<TRowData> const& other) const
		{
			assert(this->m_Stride == other.m_Stride);
			return this->m_pValue == other.m_pValue;
		}

		void advance(size_t n) { m_pValue += n * m_Stride; };

		ptrdiff_t distance_to(TRowIterator<TRowData> other) const
		{
			assert(this->m_Stride == other.m_Stride);
			return (other.m_pValue - this->m_pValue) / m_Stride;
		}

		CValue& dereference() const { return *m_pValue; }

		CValue* m_pValue = nullptr;
		int m_Stride = 0;
	};

	template <typename TRowData>
	struct TConstRowIterator : public boost::iterator_facade<
		TConstRowIterator<TRowData>,
		const CValue,
		boost::random_access_traversal_tag>
	{
		TConstRowIterator() = default;
		TConstRowIterator(const CValue* pVal, int Stride) :
			m_pValue(pVal),
			m_Stride(Stride)
		{

		}

		TConstRowIterator(TRowIterator<TRowData> i) :
			m_pValue(i.Value()),
			m_Stride(i.Stride())
		{

		}

	private:
		friend class boost::iterator_core_access;

		void increment() { m_pValue += m_Stride; }

		void decrement() { m_pValue -= m_Stride; }

		bool equal(const TConstRowIterator<TRowData>& other) const
		{
			assert(this->m_Stride == other.m_Stride);
			return this->m_pValue == other.m_pValue;
		}

		void advance(size_t n) { m_pValue += n * m_Stride; };

		ptrdiff_t distance_to(TConstRowIterator<TRowData> other) const
		{
			assert(this->m_Stride == other.m_Stride);
			return (other.m_pValue - this->m_pValue) / m_Stride;
		}

		const CValue& dereference() const { return *m_pValue; }

		const CValue* m_pValue = nullptr;
		int m_Stride = 0;
	};

	template <typename TRowData>
	using TRowRange = std::ranges::subrange<TRowIterator<TRowData>, TRowIterator<TRowData>>;

	template <typename TRowData>
	using TConstRowRange = std::ranges::subrange<TConstRowIterator<TRowData>, TConstRowIterator<TRowData>>;

	struct CPosition
	{
		int x = 0, y = 0;
	};


	enum struct Direction : char
	{
		Unknown = 'X',
		Left = 'L',
		Right = 'R',
		Up = 'U',
		Down = 'D',
	};

	template <typename TRowData>
	struct CRow
	{
		CRow() = default;

		CRow(TRowData d, int Stride, int Length, CPosition p) :
			m_Data(d),
			m_Stride(Stride),
			m_Length(Length),
			m_Position(p)
		{
			Utils::Check(m_Stride != 0, "invalid stride", m_Stride);
		}

		std::span<CValue> Values()
		{
			return m_Data.Values();
		}

		std::span<const CValue> Values() const
		{
			return m_Data.Values();
		}

		TRowRange<TRowData> Vals()
		{
			TRowIterator<TRowData> iBeg{ ValueAt(m_Position), m_Stride };
			auto iEnd = iBeg;
			std::advance(iEnd, Utils::ToInt(m_Stride * m_Length));
			return std::ranges::subrange(iBeg, iEnd);
		}

		TRowRange<TRowData> RVals()
		{
			TRowIterator<TRowData> iEnd = { ValueAt(m_Position), -m_Stride };
			auto iBeg = iEnd;
			std::advance(iBeg, -m_Stride * m_Length);
			return std::ranges::subrange(iBeg, iEnd);
		}

		TConstRowRange<TRowData> Vals() const
		{
			TConstRowIterator<TRowData> iBeg = { ValueAt(m_Position), m_Stride };
			auto iEnd = iBeg;
			std::advance(iEnd, m_Stride * m_Length);
			return std::ranges::subrange(iBeg, iEnd);
		}

		TConstRowRange<TRowData> RVals() const
		{
			TConstRowIterator<TRowData> iEnd = { ValueAt(m_Position), -m_Stride };
			auto iBeg = iEnd;
			std::advance(iBeg, -m_Stride * m_Length);
			return std::ranges::subrange(iBeg, iEnd);
		}

		std::span<const int> Numbers() const
		{
			return m_Data.Numbers();
		}

		auto RNumbers() const
		{
			auto spn = m_Data.Numbers();
			return std::ranges::subrange(spn.rbegin(), spn.rend());
		}

		int Length() const
		{
			return m_Length;
		}

		const TRowData& RowData() const
		{
			return m_Data;
		}

		TRowData& RowData()
		{
			return m_Data;
		}

		const CPosition& Position() const
		{
			return m_Position;
		}

		Direction dir() const
		{
			if (m_Stride < -1)
				return Direction::Up;
			if (m_Stride == -1)
				return Direction::Left;
			if (m_Stride > 1)
				return Direction::Down;
			if (m_Stride == 1)
				return Direction::Right;
			return Direction::Unknown;
		}

	private:

		int GetValueIndex(CPosition pos) const
		{
			return pos.x + pos.y * m_Stride;
		}

		const CValue* ValueAt(CPosition pos) const
		{
			int i = GetValueIndex(pos);
			return Values().data() + i; 
		}

		CValue* ValueAt(CPosition pos)
		{
			int i = GetValueIndex(pos);
			return Values().data() + i;
		}


		int m_Stride = 0;  // -1, +1, -W, +W
		int m_Length = 0;
		CPosition m_Position;
		TRowData m_Data;
	};


}


template<>
struct std::formatter<Row::CPosition, char>
{

	template<class ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	template<class FmtContext>
	FmtContext::iterator format(Row::CPosition pos, FmtContext& ctx) const
	{
		return std::format_to(ctx.out(), "[{}, {}]", pos.x, pos.y);
	}
};


template <typename E> requires std::is_enum_v<E>
inline std::underlying_type_t<E> operator+(E e)
{
	return static_cast<std::underlying_type_t<E>>(e);
}

template<typename T>
struct std::formatter<Row::CRow<T>, char>
{

	template<class ParseContext>
	constexpr ParseContext::iterator parse(ParseContext& ctx)
	{
		return ctx.begin();
	}

	template<class FmtContext>
	FmtContext::iterator format(const Row::CRow<T> &r, FmtContext& ctx) const
	{
		using namespace Utils;
		static_assert(std::is_same_v<std::underlying_type_t<Row::Direction>, char>);
		char d = static_cast<char> (r.dir());
		std::string Vals = Grid::SaveToString(r.Vals());
		std::string Nums(Utils::IntSize(Vals), ' ');
		std::ranges::generate(Nums, [Counter = 0]() mutable {return 48 + Counter++ % 10; });
		std::format_to(ctx.out(), "{}{}:{}\r\n{}\r\n{}\r\n{}", d, r.Position(), r.Length(), r.Numbers(), Nums, Vals);

		return ctx.out();
	}
};
