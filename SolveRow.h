#pragma once

#include "Row.h"
#include "Grid.h"

namespace Solver
{
	using Row::CRow;
	using Grid::CValue;

	template <typename TRowData>
	void SolveRow(CRow<TRowData> Input)
	{
		return;
		for (int i = 0; i<Input.Length(); ++i)
		{
			CValue& val = Input.Values()[i];
			int Num = Input.Numbers()[i];
		}
	}
}