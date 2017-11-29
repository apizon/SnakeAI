#include "define.h"
#include "apple.h"

using namespace sf;

Apple::Apple() {
	pos.x = 0;
	pos.y = 0;

	texture.loadFromFile(T_APPLE);
	sprite.setTexture(texture);
	sprite.setColor(Color::Red);
	sprite.scale((float)S_SIZE / T_SIZE, (float)S_SIZE / T_SIZE);
}

// We just calculate a random position and see if it's not already occupied and
// start again if it is, not really the most optimized way to do this i suppose 
// but it's not really a major bottleneck
void Apple::spawn(std::vector<Vector2i> obstacles, Apple apple[NB_APPLE]) {
	bool occupied[G_WIDTH][G_HEIGHT] = { false };
	for (auto v : obstacles) {
		occupied[v.x][v.y] = true;
	}
	for (int i = 0; i < NB_APPLE; i++) {
		Vector2i apos = apple[i].getPosition();
		occupied[apos.x][apos.y] = true;
	}

	do {
		pos.x = rand() % (G_WIDTH);
		pos.y = rand() % (G_HEIGHT);
		sprite.setPosition((float)pos.x*S_SIZE, (float)pos.y*S_SIZE);
	} while (occupied[pos.x][pos.y]);
}

void Apple::display(RenderWindow& win) {
	win.draw(sprite);
}

Vector2i Apple::getPosition() const{
	return pos;
}