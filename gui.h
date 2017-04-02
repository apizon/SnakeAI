#ifndef __GUI_H__
#define __GUI_H__

#include "SFML/Graphics.hpp"
#include "define.h"
#include "snake.h"
#include "apple.h"

class Gui {
	private:
		int scrollVal, scrollGameOverVal;
		std::multimap<int, int> scoreMap;
		sf::Font font;
		sf::RectangleShape vert_bar, horiz_bar, black_back;
		sf::Text title_score, score;
		sf::RectangleShape game_over_bar, game_over_black_back;
		sf::Text game_over, game_over_score;

	public:
		Gui();
		void scroll(sf::Vector2i& mousePos, int i);
		void scrollGameOver(sf::Vector2i& mousePos, int i);
		void sortByScore(std::vector<Snake*>& snek);
		void display(sf::RenderWindow& win, std::vector<Snake*>& snek, Apple& a);
		void displayGameOver(sf::RenderWindow& win, std::vector<Snake*>& snek);
};

#endif