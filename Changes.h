#pragma once

namespace Solver
{
	template <typename TIterator>
	struct CChanges
	{
		CChanges(TIterator b) :
			m_Begin(b)
		{

		}

		CChanges() = default;

		void Clear()
		{
			m_Positions.clear();
		}

		bool IsEmpty() const
		{
			return m_Positions.empty();
		}

		void Add(TIterator it)
		{
			m_Positions.push_back(it - m_Begin);
		}

		const std::vector<int>& Positions() const
		{
			return m_Positions;
		}

	private:
		TIterator m_Begin;
		std::vector<int> m_Positions;
	};
}
