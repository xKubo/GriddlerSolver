#pragma once

namespace Splice
{
	struct CExtents
	{
		CExtents() = default;
		int W = 0, H = 0;
	};

	struct CBasicSplice
	{

		CExtents Extents;
		int Stride = 0;

	};


}