#ifndef __GAME_H__
#define __GAME_H__

#include <SFML/Graphics.hpp>
#include "snake.h"
#include "apple.h"
#include "gui.h"
#include "config.h"

class Game {
	private:
		int snakeAlive;
		Apple apple[NB_APPLE];
		Snake snek[NB_PLAYER];
		std::vector<sf::Vector2i> obstacles;
		Gui gui;
		Config config;
	
	public:
		Game();
		void play(sf::RenderWindow& win);
		bool collide(Snake& s);
		void stall(Snake& s);
		void generateObstacles();
		void gameOver(sf::RenderWindow& win);
		void handleEvents(sf::RenderWindow& win);
};

#endif