#pragma once
#include "../EvoNet/EvoNet.hpp"
#include "../Tools/Random.hpp"

#include <SFML\Graphics.hpp>


#include <fstream>
#include <string>
#include <sstream>

/*
	template<typename ...Args>
		EvoNet(Args... inputSizes)
		{
			(this->layer_sizes.emplace_back(inputSizes), ...);
*/

namespace opml::Examples
{
	class FlapyBirds_Evo
	{
	public:
		template<typename ...Args>
		FlapyBirds_Evo(size_t popSize, Args... layerSizes)
		{
			//std::vector<size_t> layer_sizes;
			//(layer_sizes.push_back(layerSizes), ...);

			wnd.create(sf::VideoMode(800, 600), "FlapyBirds_Evo-Sample-Image", sf::Style::Close | sf::Style::Titlebar);
			this->nets.reserve(popSize);
			this->birds.resize(popSize, sf::RectangleShape({5.0f, 5.0f }));
			for (size_t i = 0; i < popSize; ++i)
			{
				//this->nets.emplace_back(layer_sizes);
				birds[i].setPosition(rng.randomInteger<int>(0, 800), rng.randomInteger<int>(0, 600));
				birds[i].setFillColor({ 50,255,255 });
			}
		}

		void run()
		{
			while (wnd.isOpen())
			{
				update();
				reder();
			}
		}
	private:
		void update()
		{
			sf::Event event;
			while (wnd.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) { wnd.close(); }
			}
		}

		void reder()
		{
			wnd.clear();
			for (const sf::RectangleShape &bird : birds)
			{
				wnd.draw(bird);
			}
			wnd.display();
		}


	private:
		std::vector<EvoNet> nets;
		std::vector<sf::RectangleShape> birds;
		sf::RenderWindow wnd;

		rng_mt19937_std rng;
	};
}
