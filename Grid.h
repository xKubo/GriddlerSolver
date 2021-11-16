#pragma once

#include <vector>

namespace Grid
{
	using CNumbers = std::vector<int>;

	struct CGrid
	{
	
	private:
		std::vector<CNumbers> Vertical, Horizontal;
	};


	CGrid LoadFromStream(std::istream& i)
	{
		return { };
	}
}

