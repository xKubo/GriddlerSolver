#include <iostream>
#include <vector>
#include <string>
#include <variant>

#include "Splice.h"

namespace HTML
{
	using Splice::CExtents;

	enum struct Color
	{
		Black,
		White,
	};

	enum struct Borders
	{
		None, 
		Right,
		Bottom,
		BottomRight,
	};

	struct CElement
	{
		std::string Text;
		Color Background = Color::White;
		Borders Borders = Borders::None;
	};

	using CElements = std::vector<CElement>;

	struct CTable
	{
		CTable() {}
		CTable(CExtents ex, CElements e) :
			m_Extents(ex),
			m_Elements(e)
		{

		}
	private:
		CExtents m_Extents;
		std::vector<CElement> m_Elements;
	};

	struct CHTMLOutput
	{
		CHTMLOutput(const std::istream& HTMLTemplate)
		{

		}

		void AddTable(const std::string& Description, const CTable& t)
		{

		}

		void AddError(const std::string& s)
		{

		}

		void OutputToStream(std::ostream& o)
		{

		}

	private:
		
	};
	
}

