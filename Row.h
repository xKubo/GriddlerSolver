#pragma once

#include <span>

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
		TRowIterator(CRow<TRowData>& row, int Index):
			m_pRow(&row),
			m_Index(Index)
		{

		}

	private:
		friend class boost::iterator_core_access;

		void increment() { ++m_Index;  }

		void decrement() { --m_Index; }

		bool equal(TRowIterator<TRowData> const& other) const
		{
			return this->m_Index == other.m_Index;
		}

		void advance(int n) { m_Index += n; };

		ptrdiff_t distance_to(TRowIterator<TRowData> other) const
		{
			return other.m_Index - this->m_Index;
		}

		CValue& dereference() const { return m_pRow->Values(m_Index); }

		CRow<TRowData>* m_pRow = nullptr;
		int m_Index = 0;
	};

	template <typename TRowData>
	struct TConstRowIterator : public boost::iterator_facade<
		TConstRowIterator<TRowData>,
		const CValue,
		boost::random_access_traversal_tag>
	{
		TConstRowIterator() = default;
		TConstRowIterator(const CRow<TRowData>& row, int Index) :
			m_pRow(&row),
			m_Index(Index)
		{

		}

	private:
		friend class boost::iterator_core_access;

		void increment() { ++m_Index; }

		void decrement() { --m_Index; }

		bool equal(TConstRowIterator<TRowData> const& other) const
		{
			return this->m_Index == other.m_Index;
		}

		void advance(int n) { m_Index += n; };

		ptrdiff_t distance_to(TConstRowIterator<TRowData> other) const
		{
			return other.m_Index - this->m_Index;
		}

		const CValue& dereference() const { return m_pRow->Values(m_Index); }

		const CRow<TRowData>* m_pRow = nullptr;
		int m_Index = 0;
	};

	template <typename TRowData>
	using TRowRange = std::ranges::subrange<TRowIterator<TRowData>, TRowIterator<TRowData>>;

	template <typename TRowData>
	using TConstRowRange = std::ranges::subrange<TConstRowIterator<TRowData>, TConstRowIterator<TRowData>>;

	struct CNumbers
	{
		int Stride; // -1, +1
		
	};

	template <typename TRowData>
	struct CRow
	{
		CRow() = default;

		CRow(TRowData d, int Stride, int Length) :
			m_Data(d),
			m_Stride(Stride),
			m_Length(Length)
		{

		}

	private:
		int m_Stride = 0;  // -1, +1, -W, +W
		int m_Length = 0;

		TRowIterator<TRowData> begin() const
		{
			return { *this, 0 };
		}

		TRowIterator<TRowData> end() const
		{

		}
		
		TRowData m_Data;
	};


}