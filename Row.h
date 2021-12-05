#pragma once

#include <span>
#include <cassert>

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
		TRowIterator(CValue* pVal, int Stride):
			m_pValue(pVal),
			m_Stride(Stride)
		{

		}

	private:
		friend class boost::iterator_core_access;

		void increment() { m_pValue += m_Stride;  }

		void decrement() { m_pValue -= m_Stride; }

		bool equal(TRowIterator<TRowData> const& other) const
		{
			assert(this->m_Stride == other.m_Stride);
			return this->m_pValue == other.m_pValue;
		}

		void advance(int n) { m_pValue += n*m_Stride; };

		ptrdiff_t distance_to(TRowIterator<TRowData> other) const
		{
			assert(this->m_Stride == other.m_Stride);
			return (other.m_pValue - this->m_pValue)/m_Stride;
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

	private:
		friend class boost::iterator_core_access;

		void increment() { m_pValue += m_Stride; }

		void decrement() { m_pValue -= m_Stride; }

		bool equal(TRowIterator<TRowData> const& other) const
		{
			assert(this->m_Stride == other.m_Stride);
			return this->m_pValue == other.m_pValue;
		}

		void advance(int n) { m_pValue += n * m_Stride; };

		ptrdiff_t distance_to(TRowIterator<TRowData> other) const
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

		}
		
		std::span<CValue> Values()
		{
			return m_Data.Values();
		}

		TRowRange<TRowData> Vals()
		{
			TRowIterator<TRowData> iBeg{ ValueAt(m_Position), m_Stride};
			auto iEnd = iBeg;
			std::advance(iEnd, m_Stride * m_Length);
			return std::ranges::subrange(iBeg, iEnd );
		}

		TRowRange<TRowData> RVals() 
		{
			TRowIterator<TRowData> iEnd = { ValueAt(m_Position), - m_Stride };
			auto iBeg = iEnd;
			std::advance(iBeg, - m_Stride * m_Length);
			return std::ranges::subrange(iBeg, iEnd);
		}

		TConstRowRange<TRowData> Vals() const
		{
			TConstRowIterator<TRowData> iBeg = { ValueAt(m_Position), m_Stride };
			auto iEnd = iBeg;
			std::advance(iEnd, m_Stride * m_Length);
			return subrange(iBeg, iEnd);
		}

		TConstRowRange<TRowData> RVals() const
		{
			TConstRowIterator<TRowData> iEnd = { ValueAt(m_Position), -m_Stride };
			auto iBeg = iEnd;
			std::advance(iBeg, -m_Stride * m_Length);
			return subrange(iBeg, iEnd);
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

	private:

		int GetValueIndex(CPosition pos) const
		{
			return pos.x + pos.y * m_Stride;
		}

		const CValue* ValueAt(CPosition pos) const
		{
			int i = GetValueIndex(pos);
			return Values()[i];
		}

		CValue* ValueAt(CPosition pos) 
		{
			int i = GetValueIndex(pos);
			return &Values()[i];
		}


		int m_Stride = 0;  // -1, +1, -W, +W
		int m_Length = 0;
		CPosition m_Position;		
		TRowData m_Data;
	};


}