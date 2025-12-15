#include "MenuHandler.h"

void MenuHandler::createWin() {
	if (nowSost == setting) {
		wMain->show();
		wChoose->hide();
	} else if (nowSost == choose) {
		wChoose->show();
		wMain->hide();
	}
}

MenuHandler::MenuHandler(const int& sizeY, const int& sizeX, WINDOW* parent) : Window (sizeY, sizeX, parent) {
	isBox = true;

	wMain = new OptionsWindow(sizeY, sizeX-2, win);
	wMain->addOption("Resume");
	wMain->addOption("Change num of node");
	wMain->addOption("Restart");
	wMain->addOption("Auto solve");
	wMain->addOption("Quit");
	wMain->init();
	wMain->moveCenter();

	wChoose = new OptionsWindow(sizeY, sizeX-2, win);
	for (uint8_t i = 1; i <= 5; i++)
		wChoose->addOption(std::to_string(i));
	wChoose->init();
	wChoose->moveCenter();

	addSub(wMain);
	addSub(wChoose);
}
