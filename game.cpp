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
			// Update weight of the nodes
			if (config.isWeightBFSOn())
				Astar::hardReset(obstacles);
			else
				Astar::softReset(obstacles);

			// We only generate obstacles once per frame (except if a snake die)
			// We only add the snake head to the obstacles because if we don't
			// there is way too much collisions head to head
			// The result is that the snakes don't really move at the same time
			// but one after the other which is not as the real game but make for
			// a better AI
			generateObstacles();
			for (int i = 0; i < NB_PLAYER; i++) {
				if (!snek[i].isDead()) {
					// Set departure point
					Astar::setFrom(snek[i].getHeadPosition());

					if (NB_APPLE == 1) {
						snek[i].setPath(Astar::findPath(config));
					}
					else {
						snek[i].clearPath();
						std::vector<Vector2i> path;
						//Calculate path to every apple and keep the shortest
						for (int j = 0; j < NB_APPLE; j++) {
							Astar::setTo(apple[j].getPosition(), config);
							path = Astar::findPath(config);
							if (!snek[i].hasPath() || snek[i].getPathSize() >= path.size())
								snek[i].setPath(path);
						}
					}

					// Follow path or stall if we couldn't find one
					if (!snek[i].followPath())
						stall(snek[i]);

					// Update obstacles by adding the snake's head new position to it
					obstacles.push_back(snek[i].getHeadPosition());
					if (collide(snek[i])) {
						// If a snake die we just regenerate obstacles
						generateObstacles();
						snakeAlive--;
						// End game when no snake alive
						if (snakeAlive == 0)
							return;
					}
					else {
						// Change weight of the node where the new head is
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
		// Check collision with obstacle
		if (s.isOutOfBounds() || std::count(obstacles.begin(), obstacles.end(), s.getHeadPosition()) >= 2) {
			s.die();
			return true;
		}

		// Check collision with apple
		for (int i = 0; i < NB_APPLE; i++) {
			if (s.getHeadPosition() == apple[i].getPosition()) {
				s.eat();
				apple[i].spawn(obstacles, apple);
				// If there is only 1 apple we only update the AI's destination here
				// instead of every frame to gain speed
				if (NB_APPLE == 1)
					Astar::setTo(apple[0].getPosition(), config);
			}
		}
	}
	return false;
}

// Zig zag in order of priorities : right -> left -> up -> down
// Not the best way to do this as sometimes the snake will block himself
// because he don't let an escape route and take as much place as he can
// while he sould leave a 1 or 2 block free to escape when possible
// Creating all the conditions for this is long, not really intelligent
// and not the main priority though so i just let it at that
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
			// We don't add the last element of the snake in the list of obstacles
			// as it won't exist anymore once the snake move
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

	// Uncomment to enable human control of first snake
	// (If enabled you have to change the main game loop so it ignore
	// the first snake otherwise it will move twice, once from the 
	// player and once from the AI)

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