#include "define.h"
#include "apple.h"

using namespace sf;

Apple::Apple() {
	pos.x = 0;
	pos.y = 0;

	texture.loadFromFile(T_APPLE);
	sprite.setTexture(texture);
	sprite.setColor(Color(255, 0, 0));
	sprite.scale((float)S_SIZE / T_SIZE, (float)S_SIZE / T_SIZE);
}

void Apple::spawn(std::vector<Vector2i> obstacles) {
	bool occupied[G_WIDTH][G_HEIGHT] = { false };
	for (auto v : obstacles) {
		occupied[v.x][v.y] = true;
	}

	do {
		pos.x = rand() % (G_WIDTH);
		pos.y = rand() % (G_HEIGHT);
		sprite.setPosition((float)pos.x*S_SIZE, (float)pos.y*S_SIZE);
		//sprite.setColor(Color(rand() % 256, rand() % 256, rand() % 256, 255));
	} while (occupied[pos.x][pos.y]);
}

void Apple::display(RenderWindow& win) {
	win.draw(sprite);
}

Vector2i Apple::getPosition() const{
	return pos;
}