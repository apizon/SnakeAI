#ifndef __GAME_H__
#define __GAME_H__

#include <SFML/Graphics.hpp>
#include "snake.h"
#include "apple.h"
#include "gui.h"
#include "config.h"

class Game {
	private:
		int snakeAlive;								//Number of snakes left
		Apple apple[NB_APPLE];
		Snake snek[NB_PLAYER];
		std::vector<sf::Vector2i> fixedObstacles;
		std::vector<sf::Vector2i> obstacles;		//Obstacles as a list of (x, y)
		Gui gui;									//Interface
		Config config;
	
	public:
		Game();
		void play(sf::RenderWindow& win);
		bool collide(Snake& s);						//Collision check
		void stall(Snake& s);						//Zig-Zag cycle when snake can't find path
		void addObstacle(sf::Vector2i v);			// To add an obstacle with mouse click
		void removeObstacle(sf::Vector2i v);			// To remove an obstacle with mouse click
		void generateObstacles();					//Just add all snake elements into the obstacles list
		void gameOver(sf::RenderWindow& win);		//Game over panel and score display
		void handleEvents(sf::RenderWindow& win);
};

#endif