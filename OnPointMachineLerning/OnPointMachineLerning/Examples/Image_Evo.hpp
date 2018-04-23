#pragma once
#include "../EvolutionaryAlgorithms/Population.hpp"
#include "../Utils/Utils.hpp"

#include <SFML\Graphics.hpp>
#include <iostream>


namespace opml::Examples
{
	class Image_Evo
	{
	public:
		explicit Image_Evo(unsigned int numTriangles, const std::string &imgPath = "res/Examples/Mona_Lisa.png")
			: dna(sf::Triangles, numTriangles * 3)
			, NUM_TRIANGLES(numTriangles)
		{
			this->targetImg.loadFromFile(imgPath);
			this->targetTexture.loadFromImage(this->targetImg);
			this->targetSprite.setTexture(this->targetTexture);
			this->WIDTH = this->targetTexture.getSize().x;
			this->HEIGHT = this->targetTexture.getSize().y;


			this->targetWnd.create(sf::VideoMode(this->WIDTH, this->HEIGHT), this->TITLE + "-Target", sf::Style::Close | sf::Style::Titlebar);
			this->outputWnd.create(sf::VideoMode(this->WIDTH, this->HEIGHT), this->TITLE + "-Output", sf::Style::Close | sf::Style::Titlebar);
			const sf::VideoMode mode(sf::VideoMode::getDesktopMode());
			this->targetWnd.setPosition(sf::Vector2i(mode.width / 2 - this->WIDTH - 100, mode.height / 2 - this->HEIGHT / 2));

			this->outputImg.create(this->WIDTH, this->HEIGHT);
			this->outputWnd.setPosition(sf::Vector2i(mode.width / 2 + 100, mode.height / 2 - this->HEIGHT / 2));

			for (size_t i = 0; i < this->NUM_TRIANGLES * 3; ++i)
			{
				dna[i].position = sf::Vector2f(static_cast<float>(rng.next<unsigned int>(0, this->WIDTH)), static_cast<float>(rng.next<unsigned int>(0, this->HEIGHT)));
				if (i % 3 == 0)
				{
					dna[i].color = sf::Color(static_cast<sf::Uint8>(rng.next<uint16_t>(1, 255)), static_cast<sf::Uint8>(rng.next<uint16_t>(1, 255)), static_cast<sf::Uint8>(rng.next<uint16_t>(1, 255)), 1);
					dna[i + 1].color = dna[i].color;
					dna[i + 2].color = dna[i].color;
				}
			}
			renderTexture.create(this->WIDTH, this->HEIGHT);
			renderTexture.clear();
			renderTexture.draw(dna);
			renderTexture.display();
			sf::Image img{ renderTexture.getTexture().copyToImage() };
			for (unsigned int x = 0; x < this->WIDTH; ++x)
			{
				for (unsigned int y = 0; y < this->HEIGHT; ++y)
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
			sf::Event event{};
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
				mutate(&newDNA);

				renderTexture.clear();
				renderTexture.draw(newDNA);
				renderTexture.display();

				sf::Image img{ renderTexture.getTexture().copyToImage() };

				double newError{ 0 };
				OPML_PRAGMA_OMP(parallel)
				{
					double partialError{ 0 };
					OPML_PRAGMA_OMP(for)
					for (int x = 0; x < gsl::narrow_cast<int>(this->WIDTH); ++x)
					{
						for (unsigned int y = 0; y < this->HEIGHT; ++y)
						{
							const sf::Color c(img.getPixel(x, y));
							const sf::Color target(targetImg.getPixel(x, y));
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
		void caputure(const std::string& path = "out")
		{
			renderTexture.clear();
			renderTexture.draw(this->dna);
			renderTexture.display();

			sf::Image img{ renderTexture.getTexture().copyToImage() };
			img.saveToFile(path + ".png");

			std::ofstream fout(path + ".svg");
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
				fout << std::to_string(this->dna[i].color.b) << ")\" fill-opacity=\"" << std::to_string(static_cast<double>(this->dna[i].color.a) / 255.0) << "\"/>\n"; // a dosn't work
			}
			fout << "</svg>";
			fout.close();
		}

		template <typename T>
		T clip(const T& n, const T& lower, const T& upper) noexcept {
			return std::max(lower, std::min(n, upper));
		}

		void mutate(gsl::not_null<sf::VertexArray*> _dna)
		{
			const double roulette{ rng.next<double>(0, 2.8) };
			const double drastic{ rng.next<double>(0, 1.0) };

			if (roulette < 1.0)
			{
				const size_t index{ rng.next<size_t>(0, (_dna->getVertexCount() - 3) / 3) * 3 };
				if (roulette < 0.25)
				{
					if (drastic < 0.75)
					{
						_dna->operator[](index).color.a += gsl::narrow_cast<sf::Int8>(rng.next<int16_t>(-25, 25));
						clip<uint8_t>(_dna->operator[](index).color.a, 1, 255);
						_dna->operator[](index + 1).color.a = _dna->operator[](index).color.a;
						_dna->operator[](index + 2).color.a = _dna->operator[](index).color.a;
					}
					else 
					{
						_dna->operator[](index).color.a = gsl::narrow_cast<sf::Uint8>(rng.next<int16_t>(1, 255));
						_dna->operator[](index + 1).color.a = _dna->operator[](index).color.a;
						_dna->operator[](index + 2).color.a = _dna->operator[](index).color.a;
					}
				}
				else if (roulette < 0.5)
				{
					if (drastic < 0.75)
					{
						_dna->operator[](index).color.r += gsl::narrow_cast<sf::Int8>(rng.next<int16_t>(-25, 25));
						clip<uint8_t>(_dna->operator[](index).color.r, 0, 255);
						_dna->operator[](index + 1).color.r = _dna->operator[](index).color.r;
						_dna->operator[](index + 2).color.r = _dna->operator[](index).color.r;
					} 
					else
					{ 
						_dna->operator[](index).color.r = gsl::narrow_cast<sf::Uint8>(rng.next<int16_t>(0, 255));
						_dna->operator[](index + 1).color.r = _dna->operator[](index).color.r;
						_dna->operator[](index + 2).color.r = _dna->operator[](index).color.r;
					}
				}
				else if (roulette < 0.75)
				{
					if (drastic < 0.75)
					{
						_dna->operator[](index).color.g += gsl::narrow_cast<sf::Int8>(rng.next<int16_t>(-25, 25));
						clip<uint8_t>(_dna->operator[](index).color.g, 0, 255);
						_dna->operator[](index + 1).color.g = _dna->operator[](index).color.g;
						_dna->operator[](index + 2).color.g = _dna->operator[](index).color.g;
					} 
					else 
					{
						_dna->operator[](index).color.g = gsl::narrow_cast<sf::Uint8>(rng.next<int16_t>(0, 255));
						_dna->operator[](index + 1).color.g = _dna->operator[](index).color.g;
						_dna->operator[](index + 2).color.g = _dna->operator[](index).color.g;
					}
				}
				else
				{
					if (drastic < 0.75)
					{
						_dna->operator[](index).color.b += gsl::narrow_cast<sf::Int8>(rng.next<int16_t>(-25, 25));
						clip<uint8_t>(_dna->operator[](index).color.b, 0, 255);
						_dna->operator[](index + 1).color.b = _dna->operator[](index).color.b;
						_dna->operator[](index + 2).color.b = _dna->operator[](index).color.b;
					} 
					else
					{
						_dna->operator[](index).color.b = gsl::narrow_cast<sf::Uint8>(rng.next<int16_t>(0, 255));
						_dna->operator[](index + 1).color.b = _dna->operator[](index).color.b;
						_dna->operator[](index + 2).color.b = _dna->operator[](index).color.b;
					}
				}
			}
			else if (roulette < 2.0)
			{
				const size_t index{ rng.next<size_t>(0, _dna->getVertexCount() - 1) };
				if (roulette < 1.5)
				{
					if (drastic < 0.75)
					{
						_dna->operator[](index).position.x += rng.next<unsigned int>(this->WIDTH / -5, this->WIDTH / 5);
						clip<float>(_dna->operator[](index).position.x, 0.0f, static_cast<float>(this->WIDTH));
					}
					else { _dna->operator[](index).position.x = static_cast<float>(rng.next<unsigned int>(0, this->WIDTH)); }
				}
				else
				{
					if (drastic < 0.75)
					{
						_dna->operator[](index).position.y += rng.next<unsigned int>(this->HEIGHT / -5, this->HEIGHT / 5);
						clip<float>(_dna->operator[](index).position.y, 0.0f, static_cast<float>(this->HEIGHT));
					} else { _dna->operator[](index).position.y = static_cast<float>(rng.next<unsigned int>(0, this->HEIGHT)); }
				}
			}
			else
			{
				const size_t index{ rng.next<size_t>(0, (_dna->getVertexCount() - 3) / 3) * 3 };
				const size_t destination{ rng.next<size_t>(0, (_dna->getVertexCount() - 3) / 3) * 3 };
				std::swap(_dna->operator[](index), _dna->operator[](destination));
				std::swap(_dna->operator[](index + 1), _dna->operator[](destination + 1));
				std::swap(_dna->operator[](index + 2), _dna->operator[](destination + 2));
			}
		}

	private:
		sf::VertexArray dna;
		double error{ 0 };
		unsigned int mutations{ 0 };

		const unsigned int NUM_TRIANGLES;
		unsigned int WIDTH, HEIGHT;
		const std::string TITLE{ "Image_Evo" };

		sf::Image targetImg;
		sf::RenderWindow targetWnd;
		sf::Texture targetTexture;
		sf::Sprite targetSprite;

		sf::Image outputImg;
		sf::RenderWindow outputWnd;
		sf::RenderTexture renderTexture;
	};
}  // namespace Examples  // namespace opml