#pragma once
#include "../EvolutionaryAlgorithms/EvoNet.hpp"
#include "../Utils/Utils.hpp"
#include "../Functions/Activation/Sigmoid.hpp"
#include "../EvolutionaryAlgorithms/Functions/Crossover/Midpoint.hpp"
#include "../Functions/Activation/TanH.hpp"
#include "../EvolutionaryAlgorithms/Population.hpp"

#include <SFML\Graphics.hpp>
#include <iostream>


namespace opml::Examples
{
	class FlappyBirds_Evo
	{
	public:
		template<typename ...Args>
		FlappyBirds_Evo(size_t popSize, double mutationChance, double mutationAmount, Args... layerSizes)
			: POP_SIZE(popSize)
		{
			this->layer_sizes.emplace_back(2); // 1: x Distance to pipe 2: y Distance to Pipe gap
			(this->layer_sizes.push_back(layerSizes), ...);
			this->layer_sizes.emplace_back(1); // x > 0.5 ? flap : don't flap
			for (size_t i = 0; i < layer_sizes.size(); i++)
			{
				totalNodeCount += layer_sizes[i];
			}

			this->wnd.create(sf::VideoMode(this->WIDTH, this->HEIGHT), this->TITLE, sf::Style::Close | sf::Style::Titlebar);
			this->wnd.setFramerateLimit(60 * accelerationFactor);

			this->population.population.reserve(this->POP_SIZE);
			this->birds.resize(this->POP_SIZE, Bird(sf::RectangleShape(this->BIRD_SIZE)));

			OPML_PRAGMA_OMP(parallel for)
			for (int i = 0; i < this->POP_SIZE; ++i)
			{
				this->population.population.emplace_back(std::make_shared<EvoNet>(layer_sizes));
				this->population.population[i]->setActivationFunction(sigmoid);
				this->population.population[i]->setCrossoverFunction(crossover);
				this->population.population[i]->weightsRange(-1.0, 1.0);
				birds[i].shape.setOrigin({ BIRD_SIZE.x / 2.0f, BIRD_SIZE.y / 2.0f });
				birds[i].shape.setPosition(this->XOFFSET, this->HEIGHT / 2);
				birds[i].pos = this->HEIGHT / 2;
				birds[i].shape.setFillColor({ 255, 255, 255, 45 });
			}
			this->pipes.spawn(this->WIDTH, this->HEIGHT);
		}
		
		void run()
		{
			Clock fpsC;
			Clock c;
			size_t fps{ 0 };
			while (this->wnd.isOpen())
			{
				++generationFrameCounter;
				float dt = 1 / 60.0f;// std::min(c.restart<float>(), 0.1f);
				this->update(dt);
				this->render();


				++fps;
				if(fpsC.getElapsedTime<float>() > 1.0)
				{
					this->wnd.setTitle(this->TITLE + "   FPS: " + std::to_string(fps) + "   Generations: " + std::to_string(this->generations));
					fps = 0;
					fpsC.restart();
				}
			}
		}
	private:
		void update(float dt)
		{
			sf::Event event;
			while (this->wnd.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) { this->wnd.close(); }
				else if (event.type == sf::Event::MouseWheelMoved)
				{
					accelerationFactor += event.mouseWheel.delta / 5.0f;
					accelerationFactor = std::max(0.1f, accelerationFactor);
					this->wnd.setFramerateLimit(60 * accelerationFactor);
				}
				else if(event.type == sf::Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Right)
					{
						OPML_PRAGMA_OMP(parallel for)
						for (int i = 0; i < this->POP_SIZE; ++i)
						{
							if (this->birds[i].alive)
							{
								for (Pipes::PipeShape& p : this->pipes.shapes)
								{
									if (p.focused)
									{
										this->birds[i].shape.setFillColor({ 0, 0, 0, 0 });
										this->birds[i].alive = false;
										this->population.population[i]->fitness = std::pow(generationFrameCounter, 2) - this->birds[i].pos - p.bottom.getPosition().y;
										if (this->population.population[i]->fitness < 1)
										{
											this->population.population[i]->fitness = 1;
										}
									}
								}
							}
						}
					}
				}
			}
			
			this->pipes.update(this->XOFFSET, dt);

			++offsetCount;
			if (offsetCount > offset)
			{
				this->pipes.spawn(this->WIDTH, this->HEIGHT);
				offsetCount = 0;
				offset = opml::rng.next<size_t>(this->MIN_PIP_OFFSET, this->MAX_PIP_OFFSET);
			}

			bool allDead{ true };
			OPML_PRAGMA_OMP(parallel for)
			for (int i = 0; i < this->POP_SIZE; ++i)
			{
				std::vector<double> in({ 0.0, 0.0 });

				if (this->birds[i].alive)
				{
					for (Pipes::PipeShape& p : this->pipes.shapes)
					{
						if (p.focused)
						{
							in[0] = ((p.bottom.getPosition().x - this->XOFFSET) * 2 - (this->WIDTH - this->XOFFSET)) / (this->WIDTH - this->XOFFSET);
							if (this->birds[i].pos > p.bottom.getPosition().y)
							{
								in[1] = std::abs(this->birds[i].pos - p.bottom.getPosition().y) / this->HEIGHT;
							}
							else if(this->birds[i].pos < p.bottom.getPosition().y)
							{
								in[1] = std::abs(this->birds[i].pos - p.bottom.getPosition().y) / -static_cast<int>(this->HEIGHT - Pipes::GAP_SIZE);
							}
							else
							{
								in[1] = 0;
							}
							
							if (p.bottom.getPosition().x < this->XOFFSET + this->BIRD_SIZE.x / 2)
							{
								if (p.bottom.getPosition().y < this->birds[i].pos || p.bottom.getPosition().y - Pipes::GAP_SIZE > this->birds[i].pos)
								{
									this->birds[i].shape.setFillColor({ 0, 0, 0, 0 });
									this->birds[i].alive = false;
									this->population.population[i]->fitness = std::pow(generationFrameCounter, 2) - this->birds[i].pos - p.bottom.getPosition().y;
									if (this->population.population[i]->fitness < 1)
									{
										this->population.population[i]->fitness = 1;
									}
								}
							}
						}
					}
				}
				
				if (this->birds[i].alive)
				{
					if (this->population.population[i]->calculate(in)[0] > 0.5)
					{
						this->birds[i].flap();
					}
					this->birds[i].update(dt, XOFFSET, GRAVITY);
					OPML_PRAGMA_OMP(critical)
					if (allDead)
					{
						allDead = false;
					}
				}
			}
			if (allDead)
			{
				reset();
			}
		}

		void render()
		{
			this->wnd.clear();
			this->pipes.draw(wnd);
			for (Bird &bird : this->birds)
			{
				this->wnd.draw(bird.shape);
			}
			this->wnd.display();
		}

	private:
		void reset()
		{
			population.nextGeneration(this->MUTATION_CHANCE, this->MUTATION_AMOUNT);

			offsetCount = 0;
			offset = opml::rng.next<size_t>(this->MIN_PIP_OFFSET, this->MAX_PIP_OFFSET);
			generationFrameCounter = 0;
			++generations;
			this->birds.clear();
			this->birds.resize(this->POP_SIZE, Bird(sf::RectangleShape(this->BIRD_SIZE)));
			this->pipes.reset();

			OPML_PRAGMA_OMP(parallel for)
			for (int i = 0; i < this->POP_SIZE; ++i)
			{
				birds[i].shape.setOrigin({ BIRD_SIZE.x / 2.0f, BIRD_SIZE.y / 2.0f });
				birds[i].shape.setPosition(this->WIDTH / 2, this->XOFFSET);
				birds[i].pos = this->WIDTH / 2;
				birds[i].shape.setFillColor({ 255, 255, 255, 45 });

			}
			this->pipes.spawn(this->WIDTH, this->HEIGHT);
		}
		

		struct Bird
		{
			Bird(const sf::RectangleShape &shape) : shape(shape) {}
			void flap() { flaped = true; }
			void update(float dt, size_t xOffset, float gravity)
			{
				if (flaped && vel > gravity / 10.0f)
				{
					acc = 0.0f; vel = -gravity / 4.0f;
					flaped = false;
				}
				else { acc += gravity * dt; }

				if (acc > gravity) { acc = gravity; }
				vel += acc * dt; 
				pos += vel * dt;
				shape.setPosition(xOffset, pos);
			}
			bool alive = true;
			bool flaped = false;
			float pos = 0.0f, vel = 0.0f, acc = 0.0f;
			sf::RectangleShape shape;
		};

		struct Pipes
		{
			Pipes() = default;
			void spawn(size_t wndWidth, size_t wndHeight)
			{
				sf::RectangleShape top(sf::Vector2f(static_cast<float>(this->WIDTH), static_cast<float>(wndHeight)));
				sf::RectangleShape bottom(sf::Vector2f(static_cast<float>(this->WIDTH), static_cast<float>(wndHeight)));
				bottom.setFillColor({ 255, 100, 0 });
				top.setFillColor({ 255, 100, 0 });
				bottom.setPosition(wndWidth, opml::rng.next<int>(GAP_SIZE * 2, wndHeight - GAP_SIZE));
				top.setPosition(wndWidth, bottom.getPosition().y - GAP_SIZE - wndHeight);
				shapes.emplace_back(top, bottom);
			}

			void update(size_t xOffset, float dt)
			{
				bool found{ false };
				for (std::vector<PipeShape>::iterator it = shapes.begin(); it != shapes.end(); ++it)
				{
					it->focused = false;
					if (!found  && it->bottom.getPosition().x + it->bottom.getGlobalBounds().width > xOffset)
					{
						found = true;
						it->focused = true;
					}
					if (it->bottom.getPosition().x < -it->bottom.getGlobalBounds().width)
					{
						shapes.erase(it);
						it = shapes.begin();
					}
					else
					{
						it->top.move(-SPEED * dt, 0.0f);
						it->bottom.move(-SPEED * dt, 0.0f);
					}
				}
			}

			

			void draw(sf::RenderWindow &wnd)
			{
				for (PipeShape &shape : shapes)
				{
					wnd.draw(shape.top);
					wnd.draw(shape.bottom);
				}
			}

			void reset()
			{
				shapes.clear();
			}

			struct PipeShape
			{
				PipeShape(const sf::RectangleShape &top, const sf::RectangleShape &bottom)
					: top(top), bottom(bottom)
				{}
				sf::RectangleShape top;
				sf::RectangleShape bottom;
				bool focused = false;
			};
			std::vector<PipeShape> shapes;
			static const size_t WIDTH = 25;
			static const size_t GAP_SIZE = 100;
			const float SPEED = 150.0f;
		};

	private:
		sf::RenderWindow wnd;
		Population<EvoNet> population;
		std::vector<Bird> birds;
		std::vector<size_t> layer_sizes;

		Pipes pipes;

		std::shared_ptr<TanH> sigmoid = std::make_shared<TanH>();
		std::shared_ptr<Midpoint> crossover = std::make_shared<Midpoint>();
		const sf::Vector2f BIRD_SIZE { 25.0f, 10.0f };


		const size_t POP_SIZE;
		const size_t WIDTH = 1024, HEIGHT = 600;
		const size_t XOFFSET = this->WIDTH / 4;
		const float GRAVITY = 1000.0f;
		const size_t MIN_PIP_OFFSET = 150, MAX_PIP_OFFSET = 250;
		size_t totalNodeCount{ 0 };

		const std::string TITLE{ "FlapyBirds_Evo" };
		const double MUTATION_CHANCE, MUTATION_AMOUNT;

		float accelerationFactor{ 1.0f };

		size_t generationFrameCounter{ 0 };
		size_t generations{ 0 };
		size_t offsetCount{ 0 };
		size_t offset{ opml::rng.next<size_t>(this->MIN_PIP_OFFSET, this->MAX_PIP_OFFSET) };

	};
}
