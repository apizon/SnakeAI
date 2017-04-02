#ifndef __APPLE_H__
#define __APPLE_H__

#include <SFML/Graphics.hpp>

class Apple {
	private:
		sf::Vector2i pos;
		sf::Texture texture;
		sf::Sprite sprite;

	public:
		Apple();
		void spawn(std::vector<sf::Vector2i> obstacles);
		void display(sf::RenderWindow& win);
		sf::Vector2i getPosition() const;
};

#endif