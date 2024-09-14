#pragma once

#include <boost/algorithm/string/split.hpp>
#include <sstream>

#include <format>

namespace Utils
{
	template <typename ... TArgs>
	void ArgsToOstream(std::ostream& o, const TArgs&... args...)
	{
		(o << ... << args);
	}

	template <typename ... TArgs>
	inline bool Check(bool res, const TArgs&... args...)
	{
		if (res == true)
			return res;
		std::ostringstream oss;
		ArgsToOstream(oss, args...);
		throw std::runtime_error("Check failed: " + oss.str());
	}

	inline std::string GetLine(std::istream& i)
	{
		std::string s;
		std::getline(i, s);
		Check(i.good(), "GetLine - stream not good");
		return s;
	}

	inline std::vector<int> SplitStringIntoNums(const std::string& s)
	{
		using namespace std;
		vector<string> str;
		vector<int> nums;
		boost::algorithm::split(str, s, [](char c) {return c == ' '; }, boost::algorithm::token_compress_on);

		for (const auto& s : str)
		{
			if (s.empty())
				continue;
			size_t Idx;
			int num = std::stoi(s, &Idx);
			Check(num >= 0, "NumMustBePositive");
			if (Idx != s.size())
				Check(Idx == s.size(), "Cannot convert string to number:", s);
			nums.push_back(num);
		}

		return nums;
	}

	template <typename E> requires std::is_enum_v<E>
	inline std::underlying_type_t<E> operator+(E e)
	{
		return static_cast<std::underlying_type_t<E>>(e);
	}

	inline std::vector<int> GetNextNumsLine(std::istream& i)
	{
		auto s = GetLine(i);
		return SplitStringIntoNums(s);
	}

	inline std::string LoadAllFromStream(std::istream& i)
	{
		return std::string{ std::istreambuf_iterator<char>(i), std::istreambuf_iterator<char>{} };
	}

	template <std::integral T>
	inline int ToInt(T t)
	{
		return static_cast<int>(t);
	}

	template <typename T>
	inline int IntSize(const T& t)
	{
		return ToInt(t.size());
	}


	template <typename T>
	struct Range
	{

		template<class ParseContext>
		constexpr ParseContext::iterator parse(ParseContext& ctx)
		{
			return ctx.begin();
		}

		template<class FmtContext>
		FmtContext::iterator format(const T& v, FmtContext& ctx) const
		{
			using namespace std::literals;
			auto i = ctx.out();
			if (v.empty())
			{
				return std::ranges::copy("[]"sv, i).out;
			}
			bool first = true;
			for (const auto& e : v)
			{
				if (first)
				{
					first = false;
					*i++ = '[';
				}
				else
					*i++ = ',';
				i = std::format_to(i, "{}", e);
			}
			*i++ = ']';
			return i;
		}
	};

}



template<typename T, typename TAlloc>
struct std::formatter<std::vector<T, TAlloc>, char> : Utils::Range<std::vector<T, TAlloc>>
{

};

template<typename T>
struct std::formatter<std::span<T>, char> : Utils::Range<std::span<T>>
{

};
