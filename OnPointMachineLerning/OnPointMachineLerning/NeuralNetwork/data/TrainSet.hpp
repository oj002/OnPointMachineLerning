#pragma once
#include "../Tools/ArrayTools.hpp"
#include "../Tools/Random.hpp"
#include <algorithm>
#include <list>
#include <string>
#include <vector>

namespace opml
{
	static rng_mt19937_std m_rng;


	class TrainSet
	{
	public:

		TrainSet(size_t INPUT_DEPTH, size_t INPUT_WIDTH, size_t INPUT_HEIGHT, size_t OUTPUT_DEPTH, size_t OUTPUT_WIDTH, size_t OUTPUT_HEIGHT)
			: INPUT_DEPTH(INPUT_DEPTH)
			, INPUT_WIDTH(INPUT_WIDTH)
			, INPUT_HEIGHT(INPUT_HEIGHT)
			, OUTPUT_DEPTH(OUTPUT_DEPTH)
			, OUTPUT_WIDTH(OUTPUT_WIDTH)
			, OUTPUT_HEIGHT(OUTPUT_HEIGHT)
		{
		}

		void addData(vector3D  in, vector3D  expected) 
		{
			if (in.size() != INPUT_DEPTH ||
				in[0].size() != INPUT_WIDTH ||
				in[0][0].size() != INPUT_HEIGHT ||
				expected.size() != OUTPUT_DEPTH ||
				expected[0].size() != OUTPUT_WIDTH ||
				expected[0][0].size() != OUTPUT_HEIGHT) 
			{ return; }
			data.push_back(vector4D{ in, expected });
		}

		TrainSet extractBatch(size_t size)
		{
			if (size > 0 && size <= this->data.size()) 
			{
				TrainSet set = TrainSet(INPUT_DEPTH, INPUT_WIDTH, INPUT_HEIGHT, OUTPUT_DEPTH, OUTPUT_WIDTH, OUTPUT_HEIGHT);
				for (size_t i = 0; i < size; ++i)
				{
					auto index = m_rng.randomInteger<size_t>(0, this->data.size() - 1);
					set.addData(this->getInput(index), this->getOutput(index));
				}
				return set;
			}
			return *this; 
		}

		void shuffle()
		{
			std::shuffle(data.begin(), data.end(), m_rng.getMt19937());
		}

		size_t size() const { return this->data.size(); }

		const vector3D& getInput(size_t index) const
		{
			static vector3D temp;
			if (index < this->data.size())
			{
				return this->data[index][0];
			}
			return temp; 
		}

		const vector3D& getOutput(size_t index) const
		{
			static vector3D temp;
			if (index < this->data.size())
			{
				return this->data[index][1];
			}
			return temp; 
		}

		inline size_t getINPUT_DEPTH() const { return INPUT_DEPTH; }
		inline size_t getINPUT_WIDTH() const { return INPUT_WIDTH; }
		inline size_t getINPUT_HEIGHT() const { return INPUT_HEIGHT; }
		inline size_t getOUTPUT_DEPTH() const { return OUTPUT_DEPTH; }
		inline size_t getOUTPUT_WIDTH() const { return OUTPUT_WIDTH; }
		inline size_t getOUTPUT_HEIGHT() const { return OUTPUT_HEIGHT; }
	public:
		const size_t INPUT_DEPTH, INPUT_WIDTH, INPUT_HEIGHT;
		const size_t OUTPUT_DEPTH, OUTPUT_WIDTH, OUTPUT_HEIGHT;

	private:
		//double[][][][] <- index1: 0 = input, 1 = output || index2-4: index of element
		std::vector<vector4D> data{};
	};
}  // namespace opml