#include "stdafx.h"
#include "HTMLOutput.h"

namespace HTML
{
	CHTMLOutput::CHTMLOutput(const std::string& HTMLTemplate)
	{
		static const std::string& Repl = "$$TABLE$$";
		auto it = std::search(HTMLTemplate.begin(), HTMLTemplate.end(), Repl.begin(), Repl.end());
		Check(it != HTMLTemplate.end(), "No repl in HTML table: " + Repl);
		m_Prefix = std::string(HTMLTemplate.begin(), it);
		m_Postfix = std::string(it + Repl.size(), HTMLTemplate.end());
	}

	void CHTMLOutput::AddTable(const std::string& Description, const CTable& t)
	{
		CInfo i;
		i.Data = t;
		i.Description = Description;
		m_Infos.push_back(i);
	}

	void PrintElement(CElement e, std::ostream& o)
	{
		static const std::vector<std::string> BorderClasses = { "normal", "borderT", "borderL", "borderTL" };
		static const std::vector<std::string> BkgndClasses = { "normal", "black" };

		o << "<TD class=\"" << BkgndClasses[static_cast<int>(e.Background)] 
			<< " " << BorderClasses[static_cast<int>(e.Borders)] << ">" << e.Text << "</TD>";
	}

	void CHTMLOutput::OutputToStream(std::ostream& o)
	{
		for (const auto& i : m_Infos)
		{
			o << "<BR>";
			o << "<H1>" << i.Description << "</H1>";
			o << "<BR>";

			int W = i.Data.Extents().W;

			for (int x = 0; x < W; ++x)
			{
				o << "<TR>";
				for (int y = 0; y < i.Data.Extents().H; ++y)
				{
					const auto& e = i.Data.Elements()[x + y * W];
					PrintElement(e, o);
				}
				o << "</TR>";
			}
		}

	}
}