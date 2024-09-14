#pragma once

#include <vector>

#include "Extents.h"
#include "Utils.h"

namespace Grid
{
	using CNumbers = std::vector<int>;
	using CNumbersVec = std::vector<CNumbers>;

	using Utils::IntSize;

	inline int FindMaxNumbersCount(const CNumbersVec& v)
	{
		auto it = std::max_element(v.begin(), v.end(), [](const CNumbers& n1, const CNumbers& n2) { return n1.size() < n2.size(); });
		return IntSize(*it);
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


	inline std::string sVals = "OXB";

	inline std::vector<CValue> LoadFromString(const std::string& str)
	{
		Grid::CValues vs;
		for (char c : str)
		{
			auto pos = sVals.find(c);
			Utils::Check(pos != -1, "Invalid value char: ", c);
			vs.push_back(static_cast<CValue>(pos));
		}
		return vs;
	}

	template <typename TValsRange>
	inline std::string SaveToString(const TValsRange &vals)
	{
		std::string res;
		for (const CValue &v : vals)
		{
			int pos = static_cast<int>(v);
			Utils::Check(pos < IntSize(sVals), "Unexpected value: ", pos);
			res.push_back(sVals[pos]);
		}
		return res;
	}

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

