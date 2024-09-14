#pragma once

#include <format>
#include <sstream>
#include <iostream>
#include <functional>

namespace Diag
{
	enum Level
	{
		Error, 
		Info,
		Verbose,
	};

	struct CContext
	{
		std::string Name = "";
	};

	struct CDiag;

	struct CMsg
	{
		CMsg(CDiag& d, Level l) :
			m_pDiag(&d)
		{

		}

		CMsg() = default;
		~CMsg();

		CMsg(const CMsg&) = delete;
		CMsg& operator=(const CMsg&) = delete;

		CMsg(CMsg&& m) noexcept :
			m_pDiag(m.m_pDiag),
			m_oss(std::move(m.m_oss))
		{
			m.m_pDiag = nullptr;
		}

		CMsg& operator=(CMsg&& m)
		{
			if (&m != this)
			{
				std::swap(m_pDiag, m.m_pDiag);
				std::swap(m_oss, m.m_oss);
			}
			return *this;
		}

		template <typename T>
		CMsg& operator<<(T && t)
		{
			m_oss << std::forward<T>(t);
			return *this;
		}

	private:
		CDiag* m_pDiag = nullptr;
		std::ostringstream m_oss;
	};

	using FReportString = std::function<void(const CContext& c, std::string&& Msg)>;

	struct CDiag
	{
		CDiag() = default;
		CDiag(FReportString f) : m_ReportString(f) {}

		CDiag(CContext c, FReportString f) :
			m_Context(c), m_ReportString(f)
		{

		}

		template <typename ... Args>
		void fmt(Level l, std::format_string<Args...> f, Args&& ... args)
		{
			ReportString(std::format(f, std::forward<Args>(args)...));
		}

		template <typename ... Args>
		void fmtI(std::format_string<Args...> f, Args&& ... args)
		{
			fmt(Level::Info, f, std::forward<Args>(args)...);
		}

		template <typename ... Args>
		void fmtE(std::format_string<Args...> f, Args&& ... args)
		{
			fmt(Level::Error, f, std::forward<Args>(args)...);
		}

		CMsg LOG(Level l)
		{
			return CMsg(*this, l);
		}

		CMsg LOGI(Level l)
		{
			return LOG(Level::Info);
		}

		CMsg LOGE(Level l)
		{
			return LOG(Level::Error);
		}

		const CContext& Context()
		{
			return m_Context;
		}

		void ReportString(std::string&& Msg)
		{
			ReportString(m_Context, std::move(Msg));
		}

	private:

		void ReportString(CContext& c, std::string&& Msg)
		{
			m_ReportString(c, std::move(Msg));
		}
		CContext m_Context;
		FReportString m_ReportString;
	};

	inline CDiag CreateConsoleDiag(std::string ClassName)
	{
		return CDiag{CContext{ClassName}, [](const CContext& c, std::string&& Msg) {std::cout << c.Name << ":" << Msg << std::endl; }};
	}

	inline CDiag CreateNoDiag()
	{
		return CDiag{ CContext{{}}, [](const CContext& c, std::string&& Msg) {} };
	}

	inline CMsg::~CMsg() {
		if (m_pDiag)
			m_pDiag->ReportString(m_oss.str());
	}

}

