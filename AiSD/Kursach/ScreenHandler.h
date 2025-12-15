#pragma once

#include "Window.h"
#include "SubWindowGame.h"
#include "FrameRate.h"

#define SMALL_W "viewport is very small."

class ScreenHandler {
	int maxY, maxX;
	Window& win;
	Window* wGame;
	Window* wSetting;
	//FrameRate framer;
	//size_t frame = 0;
public:
	ScreenHandler(Window& win);
	void addGameWin(Window* wGame);
	void addSettingWin(Window* wSetting);
	void upDate();
	//void upDateGame();
	//void upDateSetting();
	void init();
};

