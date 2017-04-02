#include <iostream>
#include <algorithm>

#include "define.h"
#include "game.h"
#include "astar.h"

using namespace sf;

Game::Game(int nbPlayer) : a() {
	snakeAlive = nbPlayer;

	snek.resize(nbPlayer);
	for (int i = 0; i < snek.size(); i++) {
		snek[i] = new Snake(i);
	}

	generateObstacles();
	a.spawn(obstacles);
}

Game::~Game() {
	for (int i = 0; i < snek.size(); i++) {
		delete snek[i];
	}
}

void Game::play(RenderWindow& win) {
	Astar::init();

	Event event;
	while (win.isOpen()) {
		while (win.pollEvent(event)) {
			if (event.type == Event::Closed)
				win.close();
			if (event.type == Event::MouseWheelScrolled) {
				gui.scroll(Mouse::getPosition(win), event.mouseWheelScroll.delta);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
			win.close();

		/*if (Keyboard::isKeyPressed(Keyboard::Left))
			snek[0]->move(LEFT);
		else if (Keyboard::isKeyPressed(Keyboard::Right))
			snek[0]->move(RIGHT);
		else if (Keyboard::isKeyPressed(Keyboard::Up))
			snek[0]->move(UP);
		else if (Keyboard::isKeyPressed(Keyboard::Down))
			snek[0]->move(DOWN);
		else
			snek[0]->move(NONE);*/

		Astar::setTo(a.getPosition());
		Astar::softReset(obstacles);
		for (auto s : snek) {
			if (!s->isDead()) {
				Astar::setFrom(s->getHeadPosition());
				s->setPath(Astar::findPath());

				if (!s->followPath())
					stall(s);

				generateObstacles();
				if (collide(s)) {
					generateObstacles();
					snakeAlive--;
					if (snakeAlive == 0)
						return;
				}
			}
		}

		gui.display(win, snek, a);
	}
}

bool Game::collide(Snake* s) {
	if (!s->isDead()) {
		if (s->isOutOfBounds() || std::count(obstacles.begin(), obstacles.end(), s->getHeadPosition()) >= 2) {
			s->die();
			return true;
		}

		if (s->getHeadPosition() == a.getPosition()) {
			s->eat();
			a.spawn(obstacles);
		}
	}
	return false;
}

void Game::stall(Snake* s) {
	Vector2i sHead = s->getHeadPosition();
	if (sHead.x < G_WIDTH - 1 && std::count(obstacles.begin(), obstacles.end(), sHead + Vector2i(1, 0)) == 0)
		s->move(RIGHT);
	else if (sHead.x > 0 && std::count(obstacles.begin(), obstacles.end(), sHead + Vector2i(-1, 0)) == 0)
		s->move(LEFT);
	else if (sHead.y > 0 && std::count(obstacles.begin(), obstacles.end(), sHead + Vector2i(0, -1)) == 0)
		s->move(UP);
	else if (sHead.y < G_HEIGHT - 1 && std::count(obstacles.begin(), obstacles.end(), sHead + Vector2i(0, 1)) == 0)
		s->move(DOWN);
	else
		s->move(NONE);
	
}

void Game::generateObstacles() {
	obstacles.clear();
	for (auto s : snek) {
		if (!s->isDead()) {
			std::vector<Vector2i> snekPos = s->getPosition();
			obstacles.reserve(obstacles.size() + snekPos.size());
			obstacles.insert(obstacles.end(), snekPos.begin(), snekPos.end());
		}
	}
}

void Game::gameOver(sf::RenderWindow& win) {
	Event event;
	while (win.isOpen()) {
		while (win.pollEvent(event)) {
			if (event.type == Event::Closed)
				win.close();
			if (event.type == Event::MouseWheelScrolled) {
				gui.scrollGameOver(Mouse::getPosition(win), event.mouseWheelScroll.delta);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
			win.close();

		gui.displayGameOver(win, snek);
	}
}