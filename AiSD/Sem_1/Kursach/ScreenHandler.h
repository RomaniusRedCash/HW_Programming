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
public:
	ScreenHandler(Window& win);
	void addGameWin(Window* wGame);
	void addSettingWin(Window* wSetting);
	void upDate();
	void init();
};

