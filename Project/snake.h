#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <vector>
#include <SFML/Graphics.hpp>
#include "config.h"

class Snake {
	private:
		int length;
		int direction;					// Direction of the head
		int score;
		bool dead;
		std::vector<sf::Vector2i> pos;	// The snake is just stored as a list of (x, y)
		std::vector<sf::Vector2i> path; // Same for the path
		sf::Texture texture;
		sf::Sprite sprite;
		sf::Color color;				// Original image is white but we can color it thanks to SFML
		
	public:
		Snake();
		Snake(Snake& s);
		void spawn();											// Random positionning
		void move(int dir);										// Move in said direction
		bool followPath();										// Move according to the path
		void setPath(const std::vector<sf::Vector2i>& p);
		void clearPath();										// Empty path
		void eat();												// Grow and gain score
		void die();
		void display(sf::RenderWindow& win, Config& config);
		bool isOutOfBounds() const;								// Basic check to see if the snake hit a side wall
		bool hasPath() const;
		int getScore() const;
		bool isDead() const;
		sf::Color getColor() const;
		sf::Vector2i getHeadPosition() const;
		std::vector<sf::Vector2i> getPosition() const;
		int getPathSize() const;
};

#endif