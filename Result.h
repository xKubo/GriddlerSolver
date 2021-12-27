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
		TooManyBlacks = 4,
	};

	template <typename TResult = void>
	struct [[nodiscard]] CResult
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

		const TResult& Result() const
		{
			return std::get<TResult>(m_Data);
		}

		TResult& Result()
		{
			return std::get<TResult>(m_Data);
		}

		ErrorCode Code() const
		{
			return std::get<ErrorCode>(m_Data);
		}

		operator bool() const
		{
			return m_Data.index() == 1;
		}

	private:
		std::variant<ErrorCode, TResult> m_Data = ErrorCode::Uninitialized;
	};

	template <>
	struct [[nodiscard]] CResult<void>
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
