#pragma once
#include "../Utils/Utils.hpp"
#include "../EvolutionaryAlgorithms/Population.hpp"

#include <SFML\Graphics.hpp>
#include <iostream>


namespace opml::Examples
{
	class Image_Evo
	{
	public:
		Image_Evo(size_t numTriangles, const std::string &imgPath = "res/Examples/Mona_Lisa.png")
			: NUM_TRIANGLES(numTriangles)
			, dna(sf::Triangles, numTriangles * 3)
		{
			this->targetImg.loadFromFile(imgPath);
			this->targetTexture.loadFromImage(this->targetImg);
			this->targetSprite.setTexture(this->targetTexture);
			this->WIDTH = this->targetTexture.getSize().x;
			this->HEIGHT = this->targetTexture.getSize().y;


			this->targetWnd.create(sf::VideoMode(this->WIDTH, this->HEIGHT), this->TITLE + "-Target", sf::Style::Close | sf::Style::Titlebar);
			this->outputWnd.create(sf::VideoMode(this->WIDTH, this->HEIGHT), this->TITLE + "-Output", sf::Style::Close | sf::Style::Titlebar);
			sf::VideoMode mode(sf::VideoMode::getDesktopMode());
			this->targetWnd.setPosition(sf::Vector2i(mode.width / 2 - this->WIDTH, mode.height / 2 - this->HEIGHT / 2));

			this->outputImg.create(this->WIDTH, this->HEIGHT);
			this->outputWnd.setPosition(sf::Vector2i(mode.width / 2, mode.height / 2 - this->HEIGHT / 2));

			for (size_t i = 0; i < this->NUM_TRIANGLES * 3; ++i)
			{
				dna[i].position = sf::Vector2f(rng.randomInteger<short>(0, this->WIDTH), rng.randomInteger<short>(0, this->HEIGHT));
				dna[i].color = sf::Color(0, 0, 0, 1);
			}
			renderTexture.create(this->WIDTH, this->HEIGHT);
			renderTexture.clear();
			renderTexture.draw(dna);
			renderTexture.display();
			sf::Image img{ renderTexture.getTexture().copyToImage() };
			for (size_t x = 0; x < this->WIDTH; ++x)
			{
				for (size_t y = 0; y < this->HEIGHT; ++y)
				{
					fitness += std::abs(img.getPixel(x, y).r - targetImg.getPixel(x, y).r) + std::abs(img.getPixel(x, y).g - targetImg.getPixel(x, y).g) + std::abs(img.getPixel(x, y).b - targetImg.getPixel(x, y).b);
					maxFitness += targetImg.getPixel(x, y).r + targetImg.getPixel(x, y).g + targetImg.getPixel(x, y).b;
				}
			}
		}

		void run()
		{
			size_t iterations{ 0 };
			while (this->targetWnd.isOpen() && this->outputWnd.isOpen())
			{
				this->update();
				this->render();
				++iterations;
				if (iterations % 1000 == 0)
				{
					this->outputWnd.setTitle(TITLE + "   Iteration: " + std::to_string(iterations));
				}
			}
			this->targetWnd.close();
			this->outputWnd.close();
		}

		void update()
		{
			sf::Event event;
			while (this->targetWnd.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) { this->targetWnd.close(); }
			}
			while (this->outputWnd.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) { this->outputWnd.close(); }
			}

			sf::VertexArray newDNA{ dna };
			mutate(newDNA);

			renderTexture.clear();
			renderTexture.draw(newDNA);
			renderTexture.display();
			
			sf::Image img{ renderTexture.getTexture().copyToImage() };
			double newFitness{ 0 };
			OPML_PRAGMA_OMP(parallel)
			{
				double partialFitness{ 0 };
				OPML_PRAGMA_OMP(for schedule(static))
				for (int x = 0; x < this->WIDTH; ++x)
				{
					for (size_t y = 0; y < this->HEIGHT; ++y)
					{
						partialFitness += std::abs(img.getPixel(x, y).r - targetImg.getPixel(x, y).r) + std::abs(img.getPixel(x, y).g - targetImg.getPixel(x, y).g) + std::abs(img.getPixel(x, y).b - targetImg.getPixel(x, y).b);
					}
				}
				OPML_PRAGMA_OMP(atomic)
				newFitness += partialFitness;
			}
			if (newFitness < this->fitness)
			{
				this->fitness = newFitness;
				this->dna = newDNA;
			}
		}

		void render()
		{
			this->targetWnd.clear(sf::Color(255,255,255,255));
			this->outputWnd.clear(sf::Color(255,255,255,255));

			this->outputWnd.draw(this->dna);
			this->targetWnd.draw(this->targetSprite);

			this->targetWnd.display();
			this->outputWnd.display();
		}

	private:
		template <typename T>
		T clip(const T& n, const T& lower, const T& upper) {
			return std::max(lower, std::min(n, upper));
		}

		void mutate(sf::VertexArray &dna)
		{
			double roulette{ rng.randomReal<double>(0, 2.8) };
			double drastic{ rng.randomReal<double>(0, 2.0) };

			if (roulette < 1.0)
			{
				size_t index{ rng.randomInteger<size_t>(0, dna.getVertexCount() - 1) };
				if (roulette < 0.25)
				{
					if (drastic < 1)
					{
						dna[index].color.a += rng.randomInteger<short>(255 / -5, 255 / 5);
						clip<uint8_t>(dna[index].color.a, 0, 255);
					} else { dna[index].color.a = rng.randomInteger<short>(0, 255); }
				}
				else if (roulette < 0.5)
				{
					if (drastic < 1)
					{
						dna[index].color.r += rng.randomInteger<short>(255 / -10, 255 / 10);
						clip<uint8_t>(dna[index].color.r, 0, 255);
					} else { dna[index].color.a = rng.randomInteger<short>(0, 255); }
				}
				else if (roulette < 0.75)
				{
					if (drastic < 1)
					{
						dna[index].color.g += rng.randomInteger<short>(255 / -10, 255 / 10);
						clip<uint8_t>(dna[index].color.g, 0, 255);
					} else { dna[index].color.a = rng.randomInteger<short>(0, 255); }
				}
				else
				{
					if (drastic < 1)
					{
						dna[index].color.b += rng.randomInteger<short>(255 / -10, 255 / 10);
						clip<uint8_t>(dna[index].color.b, 0, 255);
					} else { dna[index].color.a = rng.randomInteger<short>(0, 255); }
				}
			}
			else if (roulette < 2.0)
			{
				size_t index{ rng.randomInteger<size_t>(0, dna.getVertexCount() - 1) };
				if (roulette < 1.5)
				{
					if (drastic < 1)
					{
						dna[index].position.x += rng.randomInteger<short>(this->WIDTH / -10, this->WIDTH / 10);
						clip<float>(dna[index].position.x, 0, this->WIDTH);
					}
					else { dna[index].position.x = rng.randomInteger<short>(0, this->WIDTH); }
				}
				else
				{
					if (drastic < 1)
					{
						dna[index].position.y += rng.randomInteger<short>(this->HEIGHT / -10, this->HEIGHT / 10);
						clip<float>(dna[index].position.y, 0, this->HEIGHT);
					} else { dna[index].position.y = rng.randomInteger<short>(0, this->HEIGHT); }
				}
			}
			else
			{
				size_t index{ rng.randomInteger<size_t>(0, (dna.getVertexCount() - 3) / 3) * 3 };
				size_t destination{ rng.randomInteger<size_t>(0, (dna.getVertexCount() - 3) / 3) * 3 };
				std::swap(dna[index], dna[destination]);
				std::swap(dna[index + 1], dna[destination + 1]);
				std::swap(dna[index + 2], dna[destination + 2]);
			}
		}

	private:
		sf::VertexArray dna;
		size_t maxFitness{ 0 };
		size_t fitness{ 0 };

		const size_t NUM_TRIANGLES;
		size_t WIDTH, HEIGHT;
		const std::string TITLE{ "Image_Evo" };

		sf::Image targetImg;
		sf::RenderWindow targetWnd;
		sf::Texture targetTexture;
		sf::Sprite targetSprite;

		sf::Image outputImg;
		sf::RenderWindow outputWnd;
		sf::RenderTexture renderTexture;
	};
}