#include "stdafx.h"
#include "Grid.h"
#include "Utils.h"

#include <string>
#include <numeric>

#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>


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

		std::vector<CNumbers> VertNumsTranslated, HorzNums, VertNums;
		for (;;)
		{
			auto s = GetLine(i);
			if (s.empty())
				break;
			VertNumsTranslated.push_back(SplitStringIntoNums(s));
		}

		Check(!VertNumsTranslated.empty(), "At least one Vertnum");

		for (;;)
		{
			auto s = GetLine(i);
			if (s.empty())
				break;
			HorzNums.push_back(SplitStringIntoNums(s));
		}

		Check(!HorzNums.empty(), "At least one Horznum");
		Check(HorzNums.size() == e.H, "HorzNumsSize == H");

		int VertSum = 0;
		for (const auto& nums : VertNumsTranslated)
		{
			Check(nums.size() == e.W, "NumsSize == W");
			VertSum += std::accumulate(nums.begin(), nums.end(), 0);
		}

		int HorzSum = 0;

		for (const auto& nums : HorzNums)
		{
			HorzSum += std::accumulate(nums.begin(), nums.end(), 0);
		}

		Check(HorzSum == VertSum, "Num sums differ: HSum/VSum: ", HorzSum, " ", VertSum);

		int HVert = IntSize(VertNumsTranslated);
		int WVert = e.W;
		for (int x = 0; x < HVert; ++x)
			Check(VertNumsTranslated[x].size() == e.W, "All rows must have equal size");

		// translate VertNums
		VertNums.resize(WVert);
		for (int x = 0; x < WVert; ++x)
			VertNums[x].resize(HVert);
		for (int x = 0; x < WVert; ++x)		
			for (int y = 0; y < HVert; ++y)
				VertNums[x][y] = VertNumsTranslated[y][x];

		// remove all zeros
		for (auto& l : VertNums)
		{
			boost::range::remove_erase_if(l, [](int x) {return x == 0; });
		}

		for (auto& l : HorzNums)
		{
			boost::range::remove_erase_if(l, [](int x) {return x == 0; });
		}

		CValues vs(e.W * e.H );
		return CGrid{ e, VertNums, HorzNums, vs };
	}

}
