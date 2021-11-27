#pragma once
#include <ranges>

#include "Row.h"
#include "Grid.h"

#include "Result.h"

namespace Solver
{
	using Row::CRow;
	using Grid::CValue;

	using namespace std::ranges;
	using namespace Result;


	template <view TValues, view TNumbers, view TPositions>
	requires 
		std::is_same_v<range_value_t<TValues>, CValue> &&
		std::integral<range_value_t<TNumbers>>
	CVoid SolveOnePart(TValues vs, TNumbers nums, TPositions p)
	{
		using namespace std;
		using namespace std::ranges;
		auto iBeg = begin(vs);
		auto iEnd = end(vs);


		CVoid res;

		for (int n : nums)
		{
			if (std::distance(iBeg, iEnd) < n)
				return ErrorCode::LineTooSmall;

			auto itS = std::ranges::find(iBeg, iBeg + n, Grid::CValue::Cross);
			if (itS != iBeg + n)
			{
				res = Mark(iBeg, itS, CValue::Cross);
				if (!res) return res;
				continue;
			}



		}

	}


	template <typename TRowData>
	CVoid SolveRow(CRow<TRowData> Input)
	{
		int NumSize = Input.Numbers().size();
		std::vector<int> Lefts(NumSize), Rights(NumSize);
		auto r = SolveOnePart(subrange(Input.Values()), subrange(Input.Numbers()), subrange(Lefts));
		if (!r) return r;
		return ErrorCode::NoError;
		for (int i = 0; i<Input.Length(); ++i)
		{
			CValue& val = Input.Values()[i];
			int Num = Input.Numbers()[i];
		}
	}
}