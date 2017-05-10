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

	for (int i = 0; i < NB_APPLE; i++) {
		apple[i].spawn(obstacles, apple);
	}
	Astar::setTo(apple[0].getPosition(), config);
}

void Game::play(RenderWindow& win) {
	win.setFramerateLimit(config.getFPS());

	while (win.isOpen()) {
		handleEvents(win);

		if (config.isRunning()) {
			if (config.isWeightBFSOn())
				Astar::hardReset(obstacles);
			else
				Astar::softReset(obstacles);

			generateObstacles();
			for (int i = 0; i < NB_PLAYER; i++) {
				if (!snek[i].isDead()) {
					Astar::setFrom(snek[i].getHeadPosition());

					if (NB_APPLE == 1) {
						snek[i].setPath(Astar::findPath(config));
					}
					else {
						snek[i].clearPath();
						std::vector<Vector2i> path;
						for (int j = 0; j < NB_APPLE; j++) {
							Astar::setTo(apple[j].getPosition(), config);
							path = Astar::findPath(config);
							if (!snek[i].hasPath() || snek[i].getPathSize() >= path.size())
								snek[i].setPath(path);
						}
					}

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
		}

		gui.display(win, snek, apple, config);
	}
}

bool Game::collide(Snake& s) {
	if (!s.isDead()) {
		if (s.isOutOfBounds() || std::count(obstacles.begin(), obstacles.end(), s.getHeadPosition()) >= 2) {
			s.die();
			return true;
		}

		for (int i = 0; i < NB_APPLE; i++) {
			if (s.getHeadPosition() == apple[i].getPosition()) {
				s.eat();
				apple[i].spawn(obstacles, apple);
				if (NB_APPLE == 1)
					Astar::setTo(apple[0].getPosition(), config);
			}
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

void Game::handleEvents(sf::RenderWindow& win) {
	Event event;
	while (win.pollEvent(event)) {
		if (event.type == Event::Closed)
			win.close();
		else if (event.type == Event::MouseWheelScrolled) {
			gui.scroll(Mouse::getPosition(win), (int)event.mouseWheelScroll.delta);
		}
		else if (event.type == Event::KeyPressed) {
			if (event.key.code == Keyboard::Escape)
				win.close();
			else if (event.key.code == K_PAUSE)
				config.togglePause();
			else if (event.key.code == K_UP_FPS) {
				config.changeFPS(1);
				win.setFramerateLimit(config.getFPS());
			}
			else if (event.key.code == K_DOWN_FPS) {
				config.changeFPS(-1);
				win.setFramerateLimit(config.getFPS());
			}
			else if (event.key.code == K_FPS_CAP) {
				config.toggleFPSCap();
				win.setFramerateLimit(config.getFPS());
			}
			else if (event.key.code == K_TOGGLE_BFS)
				config.toggleWeightBFS();
			else if (event.key.code == K_SHOW_COST)
				config.toggleDisplayCost();
			else if (event.key.code == K_SHOW_WEIGHT)
				config.toggleDisplayWeight();
			else if (event.key.code == K_SHOW_PATH)
				config.toggleDisplayPath();
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
}