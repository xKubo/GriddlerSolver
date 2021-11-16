#include "stdafx.h"
#include "Grid.h"
#include "Utils.h"
#include <string>
#include <numeric>


namespace Grid
{
	using namespace Utils;

	CGrid LoadFromStream(std::istream& i)
	{
		auto Exts = GetNextNumsLine(i);
		Check(Exts.size() == 2, "Exts");
		CExtents e;
		e.W = Exts[0];
		e.H = Exts[1];

		auto s = GetLine(i);
		Check(s.empty(), "One Line empty after WxH. Contains:", s);

		std::vector<CNumbers> VertNums, HorzNums;
		for (;;)
		{
			auto s = GetLine(i);
			if (s.empty())
				break;
			VertNums.push_back(SplitStringIntoNums(s));
		}

		Check(!VertNums.empty(), "At least one Vertnum");

		for (;;)
		{
			auto s = GetLine(i);
			if (s.empty())
				break;
			HorzNums.push_back(SplitStringIntoNums(s));
		}

		Check(!HorzNums.empty(), "At least one Horznum");

		int VertSum = 0;
		for (const auto& nums : VertNums)
		{
			VertSum += std::accumulate(nums.begin(), nums.end(), 0);
		}

		int HorzSum = 0;
		for (const auto& nums : HorzNums)
		{
			HorzSum += std::accumulate(nums.begin(), nums.end(), 0);
		}

		Check(HorzSum == VertSum, "Num sums differ: HSum/VSum: ", HorzSum, " ", VertSum);

		CValues vs(e.W * e.H );
		return CGrid{ e, VertNums, HorzNums, vs };
	}

}
