#pragma once
#include "Exeptions.hpp"
#include <chrono>

namespace opml
{
	class Clock
	{
	public:
		Clock()
			try : m_start(std::chrono::steady_clock::now())
		{
		} OPML_INTERNAL_CATCH

		template<typename T>
		T restart()
		{
			try
			{
				const std::chrono::steady_clock::time_point old = m_start;
				m_start = std::chrono::steady_clock::now();
				const std::chrono::duration<T> elapsedTime = m_start - old;
				return elapsedTime.count();
			} OPML_INTERNAL_CATCH
		}

		void restart()
		{
			try{ m_start = std::chrono::steady_clock::now(); }
			OPML_INTERNAL_CATCH
		}

		template<typename T>
		T getElapsedTime() const
		{
			try 
			{
				const std::chrono::duration<T> elapsedTime = std::chrono::steady_clock::now() - m_start;
				return elapsedTime.count();
			} OPML_INTERNAL_CATCH
		}

	private:
		std::chrono::steady_clock::time_point m_start{};

	};

}  // namespace opml