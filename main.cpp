#include <SFML/Graphics.hpp>

#include "define.h"
#include "game.h"
#include "snake.h"

using namespace sf;

int main() {
	RenderWindow win(VideoMode(W_WIDTH, W_HEIGHT), "Snake", Style::Fullscreen);
	win.setFramerateLimit(FPS);

	srand(SEED);

	Game g;
	g.play(win);
	g.gameOver(win);

	return EXIT_SUCCESS;
}