#pragma once

#include <vector>

#include "Extents.h"

namespace Grid
{
	using CNumbers = std::vector<int>;
	using CNumbersVec = std::vector<CNumbers>;


	inline int FindMaxNumbersCount(const CNumbersVec& v)
	{
		auto it = std::max_element(v.begin(), v.end(), [](const CNumbers& n1, const CNumbers& n2) { return n1.size() < n2.size(); });
		return it->size();
	}

	using Extents::CExtents;

	enum struct CValue
	{
		None,
		Cross,
		Black,
		Error,
	};

	using CValues = std::vector<CValue>;



	struct CGrid
	{
		CGrid() = default;
		CGrid(const CExtents& e, const std::vector<CNumbers>& Vertical, const std::vector<CNumbers>& Horizontal, const CValues& Vals) :
			m_Vertical(Vertical),
			m_Horizontal(Horizontal),
			m_Values(Vals),
			m_Extents(e)
		{

		}

		CExtents Extents() const
		{
			return m_Extents;
		}

		const std::vector<CNumbers>& Vertical() const
		{
			return m_Vertical;
		}

		const std::vector<CNumbers>& Horizontal() const
		{
			return m_Horizontal;
		}

		CValues& Values()
		{
			return m_Values;
		}

		const CValues& Values() const
		{
			return m_Values;
		}

	private:
		CExtents m_Extents;
		std::vector<CNumbers> m_Vertical, m_Horizontal;
		CValues m_Values;
	};


	CGrid LoadFromStream(std::istream& i);
}

