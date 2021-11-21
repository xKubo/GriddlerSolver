#pragma once

#include <span>
#include <vector>

#include "../Row.h"
#include "../Grid.h"

namespace Row
{
	using Grid::CValue;

	struct CStringRowData
	{
		CStringRowData() = default;

		CStringRowData(const std::vector<int>& Nums, const std::vector<CValue>& Vals) :
			m_Numbers(Nums),
			m_Values(Vals)
		{

		}

		std::span<const int> Numbers() const
		{
			return m_Numbers;
		}

		std::span<CValue> Values()
		{
			return m_Values;
		}

	private:
		std::vector<int> m_Numbers;
		std::vector<CValue> m_Values;
	};

	using TStringRow = Row::CRow<CStringRowData>;

}
