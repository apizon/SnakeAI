#include "config.h"

Config::Config() {
	weightBFS = true;
	displayWeight = false;
	displayCost = false;
	displayPath = false;
	pause = true;
	fpsCap = true;
	fps = 30;
}

void Config::toggleWeightBFS() {
	weightBFS = !weightBFS;
}

void Config::toggleDisplayWeight() {
	displayWeight = !displayWeight;
}

void Config::toggleDisplayCost() {
	displayCost = !displayCost;
}

void Config::toggleDisplayPath() {
	displayPath = !displayPath;
}

void Config::changeFPS(int val) {
	if(fps > 1 || val > 0)
		fps += val * 1;
}

void Config::toggleFPSCap() {
	fpsCap = !fpsCap;
}

void Config::togglePause() {
	pause = !pause;
}

bool Config::isWeightBFSOn() const {
	return weightBFS;
}

bool Config::isDisplayWeightOn() const {
	return displayWeight;
}

bool Config::isDisplayCostOn() const {
	return displayCost;
}

bool Config::isDisplayPathOn() const {
	return displayPath;
}

bool Config::isRunning() const {
	return !pause;
}

int Config::getFPS() const {
	if(fpsCap)
		return fps;
	return 0;
}
