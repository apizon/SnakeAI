#include <iostream>
#include <algorithm>

#include "define.h"
#include "game.h"
#include "astar.h"

using namespace sf;

Game::Game() {
	snakeAlive = NB_PLAYER;
	generateObstacles();

	Astar::init();

	apple.spawn(obstacles);
	Astar::setTo(apple.getPosition());
}

void Game::play(RenderWindow& win) {

	Event event;
	while (win.isOpen()) {
		while (win.pollEvent(event)) {
			if (event.type == Event::Closed)
				win.close();
			else if (event.type == Event::MouseWheelScrolled) {
				gui.scroll(Mouse::getPosition(win), (int)event.mouseWheelScroll.delta);
			}
			else if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Escape)
					win.close();
				else if (event.key.code == Keyboard::Space)
					pause(win);
			}
		}

		/*if (Keyboard::isKeyPressed(Keyboard::Left))
			snek[0].move(LEFT);
		else if (Keyboard::isKeyPressed(Keyboard::Right))
			snek[0].move(RIGHT);
		else if (Keyboard::isKeyPressed(Keyboard::Up))
			snek[0].move(UP);
		else if (Keyboard::isKeyPressed(Keyboard::Down))
			snek[0].move(DOWN);
		else
			snek[0].move(NONE);*/

		if (WEIGHT_BFS)
			Astar::hardReset(obstacles);
		else
			Astar::softReset(obstacles);

		generateObstacles();
		for (int i = 0; i < NB_PLAYER; i++) {
			if (!snek[i].isDead()) {
				Astar::setFrom(snek[i].getHeadPosition());
				snek[i].setPath(Astar::findPath());

				if (!snek[i].followPath())
					stall(snek[i]);

				obstacles.push_back(snek[i].getHeadPosition());
				if (collide(snek[i])) {
					generateObstacles();
					snakeAlive--;
					if (snakeAlive == 0)
						return;
				}
				else {
					Astar::update(snek[i].getHeadPosition());
				}
			}
		}

		gui.display(win, snek, apple);
	}
}

bool Game::collide(Snake& s) {
	if (!s.isDead()) {
		if (s.isOutOfBounds() || std::count(obstacles.begin(), obstacles.end(), s.getHeadPosition()) >= 2) {
			s.die();
			return true;
		}

		if (s.getHeadPosition() == apple.getPosition()) {
			s.eat();
			apple.spawn(obstacles);
			Astar::setTo(apple.getPosition());
		}
	}
	return false;
}

void Game::stall(Snake& s) {
	Vector2i sHead = s.getHeadPosition();
	if (sHead.x < G_WIDTH - 1 && std::count(obstacles.begin(), obstacles.end(), sHead + Vector2i(1, 0)) == 0)
		s.move(RIGHT);
	else if (sHead.x > 0 && std::count(obstacles.begin(), obstacles.end(), sHead + Vector2i(-1, 0)) == 0)
		s.move(LEFT);
	else if (sHead.y > 0 && std::count(obstacles.begin(), obstacles.end(), sHead + Vector2i(0, -1)) == 0)
		s.move(UP);
	else if (sHead.y < G_HEIGHT - 1 && std::count(obstacles.begin(), obstacles.end(), sHead + Vector2i(0, 1)) == 0)
		s.move(DOWN);
	else
		s.move(NONE);
	
}

void Game::generateObstacles() {
	obstacles.clear();
	for (int i = 0; i < NB_PLAYER; i++) {
		if (!snek[i].isDead()) {
			std::vector<Vector2i> snekPos = snek[i].getPosition();
			obstacles.reserve(obstacles.size() + snekPos.size() - 1);
			obstacles.insert(obstacles.end(), snekPos.begin(), snekPos.end() - 1);
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
				gui.scrollGameOver(Mouse::getPosition(win), (int)event.mouseWheelScroll.delta);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
			win.close();

		gui.displayGameOver(win, snek);
	}
}

void Game::pause(sf::RenderWindow &win) {
	Event event;
	while (win.isOpen()) {
		while (win.pollEvent(event)) {
			if (event.type == Event::Closed)
				win.close();
			else if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Escape)
					win.close();
				else if (event.key.code == Keyboard::Space)
					return;
			}
		}
	}
}