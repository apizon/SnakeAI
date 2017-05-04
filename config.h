#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "SFML/Graphics.hpp"
#include "define.h"

class Config {
	private:
		bool weightBFS;
		bool displayWeight;
		bool displayCost;
		bool displayPath;
		bool pause;
		bool fpsCap;
		int fps;

	public:
		Config();
		void toggleWeightBFS();
		void toggleDisplayWeight();
		void toggleDisplayCost();
		void toggleDisplayPath();
		void changeFPS(int val);
		void toggleFPSCap();
		void togglePause();
		bool isWeightBFSOn() const;
		bool isDisplayWeightOn() const;
		bool isDisplayCostOn() const;
		bool isDisplayPathOn() const;
		bool isRunning() const;
		int getFPS() const;
};

#endif
