#include <SFML/Graphics.hpp>

#include "define.h"
#include "game.h"
#include "snake.h"

using namespace sf;

int main() {
	RenderWindow win(VideoMode(W_WIDTH, W_HEIGHT), "Snake");
	win.setFramerateLimit(FPS);

	srand(SEED);

	Game g(15);
	g.play(win);
	g.gameOver(win);

	return EXIT_SUCCESS;
}