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
			this->targetWnd.setPosition(sf::Vector2i(mode.width / 2 - this->WIDTH - 100, mode.height / 2 - this->HEIGHT / 2));

			this->outputImg.create(this->WIDTH, this->HEIGHT);
			this->outputWnd.setPosition(sf::Vector2i(mode.width / 2 + 100, mode.height / 2 - this->HEIGHT / 2));

			for (size_t i = 0; i < this->NUM_TRIANGLES * 3; ++i)
			{
				dna[i].position = sf::Vector2f(rng.randomInteger<short>(0, this->WIDTH), rng.randomInteger<short>(0, this->HEIGHT));
				if (i % 3 == 0)
				{
					dna[i].color = sf::Color(rng.randomInteger<short>(1, 255), rng.randomInteger<short>(1, 255), rng.randomInteger<short>(1, 255), 1);
					dna[i + 1].color = dna[i].color;
					dna[i + 2].color = dna[i].color;
				}
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
					this->error += std::abs(img.getPixel(x, y).r - targetImg.getPixel(x, y).r) + std::abs(img.getPixel(x, y).g - targetImg.getPixel(x, y).g) + std::abs(img.getPixel(x, y).b - targetImg.getPixel(x, y).b);
				}
			}
		}

		void run()
		{
			Clock c;
			while (this->targetWnd.isOpen() && this->outputWnd.isOpen())
			{
				this->update();
				this->render();
				if (c.getElapsedTime<float>() > 0.5f)
				{
					this->outputWnd.setTitle(this->TITLE + "   Mut: " + std::to_string(this->mutations) + "   Err: " + std::to_string(this->error));
					c.restart();
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
			{
				caputure();
			}

			for (size_t i = 0; i < 100; ++i)
			{
				sf::VertexArray newDNA{ dna };
				mutate(newDNA);

				renderTexture.clear();
				renderTexture.draw(newDNA);
				renderTexture.display();

				sf::Image img{ renderTexture.getTexture().copyToImage() };

				double newError{ 0 };
				OPML_PRAGMA_OMP(parallel)
				{
					double partialError{ 0 };
					OPML_PRAGMA_OMP(for schedule(dynamic))
					for (int x = 0; x < this->WIDTH; ++x)
					{
						for (size_t y = 0; y < this->HEIGHT; ++y)
						{
							sf::Color c(img.getPixel(x, y));
							sf::Color target(targetImg.getPixel(x, y));
							partialError += std::abs(c.r - target.r) + std::abs(c.g - target.g) + std::abs(c.b - target.b);
						}
					}
					OPML_PRAGMA_OMP(atomic)
					newError += partialError;
				}
				++mutations;
				if (newError < this->error)
				{
					this->error = newError;
					this->dna = newDNA;
					break;
				}
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
		void caputure(const std::string path = "out.svg")
		{
			std::ofstream fout(path);
			fout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
			fout << "<!DOCTYPE svg>\n";
			fout << "<svg viewBox=\"0 0 " << std::to_string(this->WIDTH) << ' ' << std::to_string(this->HEIGHT) << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
			for (size_t i = 0; i < this->dna.getVertexCount(); i += 3)
			{
				fout << "<polygon points=\"";
				fout << std::to_string(static_cast<unsigned char>(this->dna[i].position.x)) << ',' << std::to_string(static_cast<unsigned char>(this->dna[i].position.y)) << ' ';
				fout << std::to_string(static_cast<unsigned char>(this->dna[i + 1].position.x)) << ',' << std::to_string(static_cast<unsigned char>(this->dna[i + 1].position.y)) << ' ';
				fout << std::to_string(static_cast<unsigned char>(this->dna[i + 2].position.x)) << ',' << std::to_string(static_cast<unsigned char>(this->dna[i + 2].position.y)) << "\" fill=\"rgba(";
				fout << std::to_string(this->dna[i].color.r) << ',' << std::to_string(this->dna[i].color.g) << ',';
				fout << std::to_string(this->dna[i].color.b) << ',' << std::to_string(static_cast<double>(this->dna[i].color.a) / 255.0) << ")\"/>\n"; // a dosn't work
			}
			fout << "</svg>";
			fout.close();
		}

		template <typename T>
		T clip(const T& n, const T& lower, const T& upper) {
			return std::max(lower, std::min(n, upper));
		}

		void mutate(sf::VertexArray &dna)
		{
			double roulette{ rng.randomReal<double>(0, 2.8) };
			double drastic{ rng.randomReal<double>(0, 1.0) };

			if (roulette < 1.0)
			{
				size_t index{ rng.randomInteger<size_t>(0, (dna.getVertexCount() - 3) / 3) * 3 };
				if (roulette < 0.25)
				{
					if (drastic < 0.75)
					{
						dna[index].color.a += rng.normal<double>(0, 80);
						clip<uint8_t>(dna[index].color.a, 1, 255);
						dna[index + 1].color.a = dna[index].color.a;
						dna[index + 2].color.a = dna[index].color.a;
					}
					else 
					{
						dna[index].color.a = rng.randomInteger<short>(1, 255);
						dna[index + 1].color.a = dna[index].color.a;
						dna[index + 2].color.a = dna[index].color.a;
					}
				}
				else if (roulette < 0.5)
				{
					if (drastic < 0.75)
					{
						dna[index].color.r += rng.normal<double>(0, 80);
						clip<uint8_t>(dna[index].color.r, 0, 255);
						dna[index + 1].color.r = dna[index].color.r;
						dna[index + 2].color.r = dna[index].color.r;
					} 
					else
					{ 
						dna[index].color.r = rng.randomInteger<short>(0, 255);
						dna[index + 1].color.r = dna[index].color.r;
						dna[index + 2].color.r = dna[index].color.r;
					}
				}
				else if (roulette < 0.75)
				{
					if (drastic < 0.75)
					{
						dna[index].color.g += rng.normal<double>(0, 80);
						clip<uint8_t>(dna[index].color.g, 0, 255);
						dna[index + 1].color.g = dna[index].color.g;
						dna[index + 2].color.g = dna[index].color.g;
					} 
					else 
					{
						dna[index].color.g = rng.randomInteger<short>(0, 255);
						dna[index + 1].color.g = dna[index].color.g;
						dna[index + 2].color.g = dna[index].color.g;
					}
				}
				else
				{
					if (drastic < 0.75)
					{
						dna[index].color.b += rng.normal<double>(0, 80);
						clip<uint8_t>(dna[index].color.b, 0, 255);
						dna[index + 1].color.b = dna[index].color.b;
						dna[index + 2].color.b = dna[index].color.b;
					} 
					else
					{
						dna[index].color.b = rng.randomInteger<short>(0, 255);
						dna[index + 1].color.b = dna[index].color.b;
						dna[index + 2].color.b = dna[index].color.b;
					}
				}
			}
			else if (roulette < 2.0)
			{
				size_t index{ rng.randomInteger<size_t>(0, dna.getVertexCount() - 1) };
				if (roulette < 1.5)
				{
					if (drastic < 0.75)
					{
						dna[index].position.x += rng.randomInteger<short>(this->WIDTH / -5, this->WIDTH / 5);
						clip<float>(dna[index].position.x, 0, this->WIDTH);
					}
					else { dna[index].position.x = rng.randomInteger<short>(0, this->WIDTH); }
				}
				else
				{
					if (drastic < 0.75)
					{
						dna[index].position.y += rng.randomInteger<short>(this->HEIGHT / -5, this->HEIGHT / 5);
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
		size_t error{ 0 };
		size_t mutations{ 0 };

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