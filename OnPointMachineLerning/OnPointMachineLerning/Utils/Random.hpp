#pragma once
#include "Exeptions.hpp"
#include <random>

namespace opml
{
	class rng_mt19937_std
	{
	public:
		rng_mt19937_std() noexcept : mt(randomDevice()) { }
		explicit rng_mt19937_std(unsigned int seed) : mt(seed) { }

		template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
		T next(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max()) noexcept
		{
			const std::uniform_int_distribution<T> dist(min, max);
			return dist(mt);
		}
		template<typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
		T next(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max()) noexcept
		{
			const std::uniform_real_distribution<T> dist(min, max);
			return dist(mt);
		}

	public:
		std::random_device randomDevice;
		std::mt19937 mt;
	};
	/*
		rng_mt19937_std()
			try : mt(randomDevice())
		{
		} OPML_INTERNAL_CATCH

		explicit rng_mt19937_std(unsigned int seed)
			try : mt(seed)
		{
		} OPML_INTERNAL_CATCH

		template<typename T>
		T randomInteger()
		{
			try 
			{
				std::uniform_int_distribution<T> dist(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
				return dist(mt);
			} OPML_INTERNAL_CATCH
		}*/
} // namespace opml