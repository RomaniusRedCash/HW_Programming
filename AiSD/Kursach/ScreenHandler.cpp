#include "ScreenHandler.h"

ScreenHandler::ScreenHandler(Window& win) : win(win) {
	getmaxyx(stdscr, maxY, maxX);

	win.moveCenter(maxY, maxX);

	win.setBox(true);
}

void ScreenHandler::addGameWin(Window* wGame) {
	this->wGame = wGame;
	// win.addSub(wGame);
}

void ScreenHandler::addSettingWin(Window* wSetting) {
	this->wSetting = wSetting;
	// win.addSub(wSetting);
}

void ScreenHandler::upDate() {
#ifdef __WIN32__
	resize_term(0, 0);
#endif
	getmaxyx(stdscr, maxY, maxX);
	clear();
	refresh();

	if (maxY < win.getSizeY() || maxX < win.getSizeX()) {
		win.hide();
		int posX = maxX / 2 - std::strlen(SMALL_W) / 2;
		posX = posX > 0 ? posX : 0;
		mvprintw(maxY / 2, posX, SMALL_W);
	}

	else {
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
		win.show();
		win.moveCenter(maxY, maxX);
	}

	#ifdef __uinix__
	wclear(win.getWin());
	wrefresh(win.getWin());
	#endif

	// clear();
	// printw("%dx%d", maxY, maxX);
	// mvprintw()
	// win.upDate();
	refresh();
	curs_set(false);

}

void ScreenHandler::init() {
	while (!quit) {
		//framer.wait();
		upDate();
	}
}
