#pragma once

#include "Header.h"
#include "FrameRate.h"

void makeTower();

class InputHandler {
	int8_t gamePos = 0, selectGamePos = -1;
	int8_t horizontalPos = 0, selectPos = 0, verticalPos = 0;
	int ch = ERR;
	Sost oldSost = game;

	void useMain();
	void useKeyGame();
	void useSetting();
	void useChoose();

	void restartGame();
public:
	int getCh();
	void init();
	const int8_t& getHorizontalPos();
	const int8_t& getSelectPos();
	const int8_t& getGamePos();
	const int8_t& getSelectGamePos();
	const int8_t& getVerticalPos();
};

