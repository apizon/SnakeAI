#include "define.h"
#include "apple.h"

using namespace sf;

Apple::Apple() {
	pos.x = 0;
	pos.y = 0;

	texture.loadFromFile(T_APPLE);
	sprite.setTexture(texture);
	sprite.setColor(Color(rand() % 256, rand() % 256, rand() % 256, 255));
	sprite.scale((float)S_SIZE / T_SIZE, (float)S_SIZE / T_SIZE);
}

void Apple::spawn(std::vector<Vector2i> obstacles) {
	do {
		pos.x = rand() % (G_WIDTH);
		pos.y = rand() % (G_HEIGHT);
		sprite.setPosition((float)pos.x*S_SIZE, (float)pos.y*S_SIZE);
		sprite.setColor(Color(rand() % 256, rand() % 256, rand() % 256, 255));
	} while (std::find(obstacles.begin(), obstacles.end(), Vector2i(pos.x,pos.y)) != obstacles.end());
}

void Apple::display(RenderWindow& win) {
	win.draw(sprite);
}

Vector2i Apple::getPosition() const{
	return pos;
}