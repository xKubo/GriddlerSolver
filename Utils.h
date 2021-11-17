#pragma once

#include <boost/algorithm/string/split.hpp>
#include <sstream>

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
			size_t Idx;
			int num = std::stoi(s, &Idx);
			Check(num >= 0, "NumMustBePositive");
			if (Idx != s.size())
				Check(Idx == s.size(), "Cannot convert string to number:", s);
			nums.push_back(num);
		}

		return nums;
	}

	inline std::vector<int> GetNextNumsLine(std::istream& i)
	{
		auto s = GetLine(i);
		return SplitStringIntoNums(s);
	}

	inline std::string LoadAllFromStream(std::istream& i)
	{
		return std::string{ std::istream_iterator<char>(i), std::istream_iterator<char>{} };
	}

	template <typename T>
	inline int IntSize(const std::vector<T>& t)
	{
		return static_cast<int>(t.size());
	}
}
