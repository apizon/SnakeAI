#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <vector>
#include <SFML/Graphics.hpp>

class Snake {
	private:
		int length;
		int direction;
		int score;
		bool dead;
		std::vector<sf::Vector2i> pos;
		std::vector<sf::Vector2i> path;
		sf::Texture texture;
		sf::Sprite sprite;
		sf::Color color;
		
	public:
		Snake();
		Snake(Snake& s);
		void spawn();
		void move(int dir);
		bool followPath();
		void setPath(const std::vector<sf::Vector2i>& p);
		void eat();
		void die();
		void display(sf::RenderWindow& win);
		bool isOutOfBounds() const;
		bool hasPath() const;
		int getScore() const;
		bool isDead() const;
		sf::Color getColor() const;
		sf::Vector2i getHeadPosition() const;
		std::vector<sf::Vector2i> getPosition() const;
};

#endif