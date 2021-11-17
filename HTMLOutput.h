#include <iostream>
#include <vector>
#include <string>
#include <variant>

#include "Splice.h"
#include "Utils.h"

namespace HTML
{
	using Splice::CExtents;
	using Utils::Check;

	enum struct CColor
	{
		White,
		Black,
	};

	enum struct CBorders
	{
		None = 0, 
		Top = 1,
		Left = 2,
		TopLeft = 3,
	};

	struct CElement
	{
		std::string Text;
		CColor Background = CColor::White;
		CBorders Borders = CBorders::None;
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

		CExtents Extents() const
		{
			return m_Extents;
		}

		const CElements &Elements() const
		{
			return m_Elements;
		}

	private:
		CExtents m_Extents;
		CElements m_Elements;
	};

	struct CHTMLOutput
	{
		struct CInfo
		{
			std::string Description;
			CTable Data;
		};

		CHTMLOutput(const std::string& HTMLTemplate);

		void AddTable(const std::string& Description, const CTable& t);

		void AddError(const std::string& s)
		{
			CInfo i;
			i.Description = s;
			m_Infos.push_back(i);
		}

		void OutputToStream(std::ostream& o);

	private:
		std::string m_Prefix, m_Postfix;
		std::vector<CInfo> m_Infos;
	};

	template <typename E>
	inline E AddFlag(E &e1, E e2)
	{
		e1 = static_cast<E>(static_cast<int>(e1) | static_cast<int>(e2));
		return e1;
	}
	
}

