#include <iostream>
#include "gui.h"
#include "astar.h"

using namespace sf;

Gui::Gui() {
	scrollVal = 0;
	scrollGameOverVal = 0;

	//Game in progress
	vert_bar.setSize(Vector2f(1, W_HEIGHT));
	vert_bar.setFillColor(Color::White);
	vert_bar.setPosition(G_WIDTH*S_SIZE, 0);

	horiz_bar.setSize(Vector2f(W_WIDTH / 4, 1));
	horiz_bar.setFillColor(Color::White);
	horiz_bar.setPosition(G_WIDTH*S_SIZE, W_HEIGHT / 7);

	black_back.setSize(Vector2f(W_WIDTH / 4, W_HEIGHT / 7));
	black_back.setFillColor(Color::Black);
	black_back.setPosition(G_WIDTH * S_SIZE, 0);

	font.loadFromFile(FONT);

	title_score.setString("SCORE");
	title_score.setFont(font);
	title_score.setCharacterSize(W_HEIGHT / 8);
	title_score.setOutlineColor(Color::White);
	title_score.setPosition(7 * W_WIDTH / 8 - title_score.getLocalBounds().width / 2, 0);

	score.setString("P00 : 000");
	score.setFont(font);
	score.setCharacterSize(W_HEIGHT / 10);
	score.setPosition((float)(7 * W_WIDTH / 8 - score.getLocalBounds().width / 2), 0);

	//Game over
	game_over_bar.setSize(Vector2f(W_WIDTH, 1));
	game_over_bar.setFillColor(Color::White);
	game_over_bar.setPosition(0, W_HEIGHT / 4);

	game_over_black_back.setSize(Vector2f(W_WIDTH, W_HEIGHT / 4));
	game_over_black_back.setFillColor(Color::Black);
	game_over_black_back.setPosition(0, 0);

	game_over.setString("GAME OVER");
	game_over.setFont(font);
	game_over.setCharacterSize(W_HEIGHT / 4);
	game_over.setOutlineColor(Color::White);
	game_over.setPosition(W_WIDTH / 2 - game_over.getLocalBounds().width / 2, 0);

	game_over_score.setString("Player 000 : 000");
	game_over_score.setFont(font);
	game_over_score.setCharacterSize(W_HEIGHT / 8);
	game_over_score.setPosition((float)(W_WIDTH / 2 - game_over_score.getLocalBounds().width / 2), 0);
}

void Gui::scroll(Vector2i& mousePos, int val) {
	if (mousePos.x > vert_bar.getPosition().x && mousePos.x < W_WIDTH
		&& mousePos.y > horiz_bar.getPosition().y && mousePos.y < W_HEIGHT) {
		if (scrollVal <= 0)
			scrollVal += 25 * val;
		if (scrollVal > 0)
			scrollVal = 0;
	}
}

void Gui::scrollGameOver(Vector2i& mousePos, int val) {
	if (mousePos.x >= 0 && mousePos.x < W_WIDTH
		&& mousePos.y > game_over_bar.getPosition().y && mousePos.y < W_HEIGHT) {
		if (scrollGameOverVal <= 0)
			scrollGameOverVal += 30 * val;
		if (scrollGameOverVal > 0)
			scrollGameOverVal = 0;
	}
}

void Gui::sortByScore(Snake snek[NB_PLAYER]) {
	for (int i = 0; i < NB_PLAYER; i++) {
		scoreMap.emplace(snek[i].getScore(), i);
	}
}

void Gui::display(RenderWindow& win, Snake snek[NB_PLAYER], Apple apple) {
	sortByScore(snek);

	win.clear();

	if (DISPLAY_WEIGHT || DISPLAY_COST) {
		RectangleShape rect;
		for (int i = 0; i < G_WIDTH; i++) {
			for (int j = 0; j < G_HEIGHT; j++) {
				int color = 0;
				if(DISPLAY_WEIGHT)
					color += 128 * (1- Astar::getWeight(i, j) / WEIGHT_REF);
				if(DISPLAY_COST)
					color += 128 * (1 - Astar::getCost(i, j) / WEIGHT_REF);

				rect.setFillColor(Color(color, color, color));
				rect.setPosition(i*S_SIZE, j * S_SIZE);
				rect.setSize(Vector2f(S_SIZE, S_SIZE));
				win.draw(rect);
			}
		}
	}

	if (NB_PLAYER > 10) {
		if (scrollVal < 6 * W_HEIGHT / 7 - (int)(W_HEIGHT / 12 * (NB_PLAYER + 1)))
			scrollVal = 6 * W_HEIGHT / 7 - (int)(W_HEIGHT / 12 * (NB_PLAYER + 1));
	}
	else
		scrollVal = 0;

	int i = 0;
	for (auto it = scoreMap.rbegin(); it != scoreMap.rend(); ++it) {
		String pnum, pscore;
		if (it->second + 1 < 10)
			pnum = "0" + std::to_string(it->second + 1);
		else
			pnum = std::to_string(it->second + 1);

		if (it->first < 10)
			pscore = "00" + std::to_string(it->first);
		else if (it->first < 100)
			pscore = "0" + std::to_string(it->first);
		else
			pscore = std::to_string(it->first);

		score.setString("P" + pnum + " : " + pscore);
		score.setFillColor(snek[it->second].getColor());
		score.setPosition(score.getPosition().x, (float)(W_HEIGHT / 7 + scrollVal + i * W_HEIGHT / 12));
		win.draw(score);

		if (!snek[it->second].isDead())
			snek[it->second].display(win);

		i++;
	}
	scoreMap.clear();

	win.draw(black_back);
	win.draw(vert_bar);
	win.draw(horiz_bar);
	win.draw(title_score);
	apple.display(win);
	win.display();
}

void Gui::displayGameOver(sf::RenderWindow& win, Snake snek[NB_PLAYER]) {
	sortByScore(snek);

	win.clear();

	if (NB_PLAYER > 10) {
		if (scrollGameOverVal < 3 * W_HEIGHT / 4 - (int)(W_HEIGHT / 12 * (NB_PLAYER + 1)))
			scrollGameOverVal = 3 * W_HEIGHT / 4 - (int)(W_HEIGHT / 12 * (NB_PLAYER + 1));
	}
	else
		scrollVal = 0;

	int i = 0;
	for (auto it = scoreMap.rbegin(); it != scoreMap.rend(); ++it) {
		String pnum, pscore;
		if (it->second + 1 < 10)
			pnum = "0" + std::to_string(it->second + 1);
		else
			pnum = std::to_string(it->second + 1);

		if (it->first < 10)
			pscore = "00" + std::to_string(it->first);
		else if (it->first < 100)
			pscore = "0" + std::to_string(it->first);
		else
			pscore = std::to_string(it->first);

		game_over_score.setString("Player " + pnum + " : " + pscore);
		game_over_score.setFillColor(snek[it->second].getColor());
		game_over_score.setPosition(game_over_score.getPosition().x, (float)(W_HEIGHT / 4 + scrollGameOverVal + i * W_HEIGHT / 12));
		win.draw(game_over_score);

		i++;
	}

	win.draw(game_over_black_back);
	win.draw(game_over_bar);
	win.draw(game_over);
	win.display();

	scoreMap.clear();
}