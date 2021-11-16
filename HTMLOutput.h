#include <iostream>
#include <vector>
#include <string>
#include <variant>

namespace HTML
{
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
		CTable(int W, int H, CElements e) :
			m_Width(W),
			m_Height(H),
			m_Elements(e)
		{

		}
	private:
		int m_Width = 0, m_Height = 0;
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

