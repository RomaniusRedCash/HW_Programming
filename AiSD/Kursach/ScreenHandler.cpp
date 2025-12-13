#include "ScreenHandler.h"

ScreenHandler::ScreenHandler(Window& win) : win(win) {

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
	//if (frame == FrameRate::frame) {
	//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	//	return;
	//} 
	//frame = FrameRate::frame;
	resize_term(0, 0);
	getmaxyx(stdscr, maxY, maxX);
	if (maxY < win.getSizeY() || maxX < win.getSizeX()) {
		win.hide();
		clear();
		int posX = maxX / 2 - std::strlen(SMALL_W) / 2;
		posX = posX > 0 ? posX : 0;
		mvprintw(maxY / 2, posX, SMALL_W);
	}
	else {
		clear();
		refresh();
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
	refresh();
	curs_set(false);
}

void ScreenHandler::init() {
	while (!quit) {
		//framer.wait();
		upDate();
	}
}
