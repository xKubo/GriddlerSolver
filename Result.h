#pragma once

#include <variant>

namespace Result
{
	enum struct ErrorCode
	{
		NoError = 0,
		Uninitialized = 1,
		LineTooSmall = 2,
		AlreadySpecified = 3,
	};

	template <typename TResult = void>
	struct CResult
	{
		CResult() = default;

		CResult(TResult&& r) :
			m_Data(std::move(r))
		{

		}

		CResult(const TResult& r) :
			m_Data(r)
		{

		}

		CResult(ErrorCode ec) :
			m_Data(ec)
		{

		}

		TResult& Result()
		{
			return std::get<TResult>();
		}

		ErrorCode Code() const
		{
			return std::get<ErrorCode>(m_Data);
		}

		operator bool() const
		{
			return m_Data.index() == 0;
		}

	private:
		std::variant<ErrorCode, TResult> m_Data = ErrorCode::Uninitialized;
	};

	template <>
	struct CResult<void>
	{
		CResult() = default;

		CResult(ErrorCode ec) :
			m_Code(ec)
		{

		}

		operator bool() const
		{
			return m_Code == ErrorCode::NoError;
		}

		ErrorCode Code() const
		{
			return m_Code;
		}

	private:
		ErrorCode m_Code = ErrorCode::Uninitialized;
	};

	using CVoid = CResult<void>;
}
