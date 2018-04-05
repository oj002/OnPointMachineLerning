#pragma once
#include "../EvoNet/EvoNet.hpp"
#include "../Tools/Random.hpp"
#include "../Functions/Activation/Sigmoid.hpp"
#include "../EvoNet/Functions/Crossover/Midpoint.hpp"
#include "../Functions/Activation/TanH.hpp"

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
			std::vector<size_t> layer_sizes;
			layer_sizes.emplace_back(2); // 1: x Distance to pipe 2: y Distance to Pipe gap
			(layer_sizes.push_back(layerSizes), ...);
			layer_sizes.emplace_back(2); // x > 0.5 ? jump : don't jump

			this->wnd.create(sf::VideoMode(800, 600), "FlapyBirds_Evo-Sample-Image", sf::Style::Close | sf::Style::Titlebar);
			this->wnd.setVerticalSyncEnabled(true);
			this->nets.reserve(popSize);
			this->birds.resize(popSize, sf::RectangleShape({ 5.0f, 5.0f }));
			for (size_t i = 0; i < popSize; ++i)
			{
				this->nets.emplace_back(layer_sizes);
				this->nets[i].setActivationFunction(sigmoid).setCrossoverFunction(crossover);
				birds[i].setOrigin({ 2.5f, 2.5f });
				birds[i].setPosition(100, 300);
				birds[i].setFillColor({ 50,255,255 });
			}
		}

		void run()
		{
			while (this->wnd.isOpen())
			{
				this->update();
				this->reder();
			}
		}
	private:
		void update()
		{
			std::vector<double> in{ 0.1, 0.2 };
			sf::Event event;
			while (this->wnd.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) { this->wnd.close(); }
			}
			for (size_t i = 0; i < this->nets.size(); ++i)
			{
				this->birds[i].move({ (float)this->nets[i].calculate(in)[0], (float)this->nets[i].calculate(in)[1] });
			}
		}

		void reder()
		{
			this->wnd.clear();
			for (const sf::RectangleShape &bird : this->birds)
			{
				this->wnd.draw(bird);
			}
			this->wnd.display();
		}


	private:
		std::vector<EvoNet> nets;
		std::vector<sf::RectangleShape> birds;
		sf::RenderWindow wnd;

		std::shared_ptr<TanH> sigmoid = std::make_shared<TanH>();
		std::shared_ptr<Midpoint> crossover = std::make_shared<Midpoint>();
		rng_mt19937_std rng;
	};
}
