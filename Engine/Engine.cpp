// Engine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "GameObjects.h"
#include <string>
#include <vector>

bool fullscreen = false;
int main()
{
	srand(static_cast<unsigned int>(time(0)));	
	GameEngine UnrealEngine6;

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Welcome to SDVA 103!");		
	

	for (int i = 0; i < 50; i++)
	{
		Asteroid* asteroid;
		if (rand() % 2 == 0)
		{
			asteroid = new LargeAsteroid(sf::Vector2f(100 + rand() % 1000, 100 + rand() % 500));
		}
		else
		{
			asteroid = new SmallAsteroid(sf::Vector2f(100 + rand() % 1000, 100 + rand() % 500));
		}
		sf::Vector2f randDir;
		asteroid->SetAngle(rand() % 360);		
		asteroid->SetVelocity(rand()%25 + 50);		

		UnrealEngine6.AddObject(asteroid);
	}
	Player* player = new Player("Sprites/PNG/playerShip2_red.png", sf::Vector2f(500, 300));
	UnrealEngine6.AddObject(player);
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Time dt = clock.restart();		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Return &&
					event.key.alt)
				{
					window.close();
					fullscreen = !fullscreen;
					window.create(fullscreen ? sf::VideoMode(1920, 1080) : sf::VideoMode(1280, 720), "Welcome to SDVA 103!", fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar);
				}
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}

	
		window.clear();
		
		UnrealEngine6.Update(dt.asSeconds());
		UnrealEngine6.Draw(&window);		

		window.display();
	}

	return 0;
}
