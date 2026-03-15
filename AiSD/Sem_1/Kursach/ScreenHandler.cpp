#include "ScreenHandler.h"

ScreenHandler::ScreenHandler(Window& win) : win(win) {
	bkgdset(COLOR_PAIR(1));
	getmaxyx(stdscr, maxY, maxX);
	win.moveCenter(maxY, maxX);
#ifdef __unix__
	win.setAutoClear(false);
#endif
}

void ScreenHandler::addGameWin(Window* wGame) {
	this->wGame = wGame;
	win.addSub(wGame);
}

void ScreenHandler::addSettingWin(Window* wSetting) {
	this->wSetting = wSetting;
	win.addSub(wSetting);
}

void ScreenHandler::upDate() {
#ifdef _WIN32
	resize_term(0, 0);
#endif
	switch (nowSost)
	{
	case setting:
	case choose:
		wGame->hide();
		wSetting->show();
		break;
	default:
		wSetting->hide();
		wGame->show();
		break;
	}
	if (inpH.getCh() == KEY_RESIZE) {
		clear();
		refresh();
		getmaxyx(stdscr, maxY, maxX);
		if (maxY < win.getSizeY() || maxX < win.getSizeX()) {
			win.hide();
			int posX = maxX / 2 - std::strlen(SMALL_W) / 2;
			posX = posX > 0 ? posX : 0;
			mvprintw(maxY / 2, posX, SMALL_W);
		}
		else {
			win.show();
			win.moveCenter(maxY, maxX);
		}
	}
	win.upDate();
	refresh();
	curs_set(false);
}

void ScreenHandler::init() {
	while (!quit) {
		upDate();
	}
}
