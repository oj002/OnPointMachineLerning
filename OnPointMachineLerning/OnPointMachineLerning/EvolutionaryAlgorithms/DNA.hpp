#pragma once
#include <memory>
namespace opml
{
	template<typename T>
	struct DNA
	{
		virtual void mutate(double persentage) noexcept = 0;
		virtual std::shared_ptr<T> crossover(const T *parent) const noexcept = 0;

		double fitness{ 0 };
	};
} // namespace opml