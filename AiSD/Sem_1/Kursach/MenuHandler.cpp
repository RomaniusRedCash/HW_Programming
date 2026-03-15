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
	setBox(true);

	wMain = new OptionsWindow(sizeY, sizeX-2, getWin());
	wMain->addOption(RESSUME);
	wMain->addOption(CHANGE);
	wMain->addOption(RESTART);
	wMain->addOption(AUTO);
	wMain->addOption(QUIT);
	wMain->init();
	wMain->moveCenter();

	wChoose = new OptionsWindow(sizeY, sizeX-2, getWin());
	for (uint8_t i = 1; i <= maxNumNode; i++)
		wChoose->addOption(std::to_string(i));
	wChoose->init();
	wChoose->moveCenter();

	addSub(wMain);
	addSub(wChoose);
}
