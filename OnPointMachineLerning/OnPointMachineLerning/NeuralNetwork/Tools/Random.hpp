#pragma once
#include <random>

namespace opml
{
	class rng_mt19937_std
	{
	private:
		std::random_device randomDevice;
		std::mt19937 mt;

	public:

		rng_mt19937_std()
			:  mt(randomDevice())
		{

		}

		explicit rng_mt19937_std(unsigned int seed)
			:  mt(seed)
		{

		}

		template<typename T>
		T randomInteger()
		{
			std::uniform_int_distribution<T> dist(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
			return dist(mt);
		}

		template<typename T>
		T randomInteger(T min, T max)
		{
			std::uniform_int_distribution<T> dist(min, max);
			return dist(mt);
		}

		template<typename T>
		T randomReal()
		{
			std::uniform_real_distribution<T> dist(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
			return dist(mt);
		}

		template<typename T>
		T randomReal(T min, T max)
		{
			std::uniform_real_distribution<T> dist(min, max);
			return dist(mt);
		}

		std::mt19937& getMt19937()
		{
			return mt;
		}
	};
} // namespace opml