#pragma once
#include "../Utils/Utils.hpp"
#include <algorithm>
#include <list>
#include <string>
#include <vector>

namespace opml
{
	class TrainSet
	{
	public:

		TrainSet(size_t INPUT_DEPTH, size_t INPUT_WIDTH, size_t INPUT_HEIGHT, size_t OUTPUT_DEPTH, size_t OUTPUT_WIDTH, size_t OUTPUT_HEIGHT) noexcept
			: INPUT_DEPTH(INPUT_DEPTH)
			, INPUT_WIDTH(INPUT_WIDTH)
			, INPUT_HEIGHT(INPUT_HEIGHT)
			, OUTPUT_DEPTH(OUTPUT_DEPTH)
			, OUTPUT_WIDTH(OUTPUT_WIDTH)
			, OUTPUT_HEIGHT(OUTPUT_HEIGHT)
		{
		}

		void addData(const vector3D &in, const vector3D & expected)
		{
			try
			{
				if (in.size() != INPUT_DEPTH || in[0].size() != INPUT_WIDTH || in[0][0].size() != INPUT_HEIGHT ||
					expected.size() != OUTPUT_DEPTH || expected[0].size() != OUTPUT_WIDTH || expected[0][0].size() != OUTPUT_HEIGHT)
				{
					OPML_THROW("input and expected vector don't have the correct dimensions for the TrainSet."); return;
				}
				data.push_back(vector4D{ in, expected });
			}OPML_INTERNAL_CATCH
		}

		TrainSet extractBatch(size_t size)
		{
			try
			{
				if (size > 0 && size <= this->data.size())
				{
					TrainSet set = TrainSet(INPUT_DEPTH, INPUT_WIDTH, INPUT_HEIGHT, OUTPUT_DEPTH, OUTPUT_WIDTH, OUTPUT_HEIGHT);
					for (size_t i = 0; i < size; ++i)
					{
						auto index = opml::rng.randomInteger<size_t>(0, this->data.size() - 1);
						set.addData(this->getInput(index), this->getOutput(index));
					}
					return set;
				}
				else
				{
					out_opml << OPML_MESSAGE("TrainSet is smaller than specified size, returning a smaller one") << '\n';
					return *this;
				}
			} OPML_INTERNAL_CATCH
		}

		void shuffle()
		{
			try
			{
				std::shuffle(data.begin(), data.end(), opml::rng.getMt19937());
			} OPML_INTERNAL_CATCH
		}

		

		const vector3D& getInput(size_t index) const
		{
			try
			{
				if (index < this->data.size())
				{
					return this->data[index][0];
				}
				return {};
			} OPML_INTERNAL_CATCH
		}

		const vector3D& getOutput(size_t index) const
		{
			try
			{
				if (index < this->data.size())
				{
					return this->data[index][1];
				}
				return {};
			} OPML_INTERNAL_CATCH
		}

		inline size_t size() const noexcept { return this->data.size(); }
		inline size_t getINPUT_DEPTH() const noexcept { return INPUT_DEPTH; }
		inline size_t getINPUT_WIDTH() const noexcept { return INPUT_WIDTH; }
		inline size_t getINPUT_HEIGHT() const noexcept { return INPUT_HEIGHT; }
		inline size_t getOUTPUT_DEPTH() const noexcept { return OUTPUT_DEPTH; }
		inline size_t getOUTPUT_WIDTH() const noexcept { return OUTPUT_WIDTH; }
		inline size_t getOUTPUT_HEIGHT() const noexcept { return OUTPUT_HEIGHT; }
	public:
		const size_t INPUT_DEPTH, INPUT_WIDTH, INPUT_HEIGHT;
		const size_t OUTPUT_DEPTH, OUTPUT_WIDTH, OUTPUT_HEIGHT;

	private:
		//double[][][][] <- index1: 0 = input, 1 = output || index2-4: index of element
		std::vector<vector4D> data{};
	};
}  // namespace opml