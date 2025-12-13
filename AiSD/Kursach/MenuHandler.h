#pragma once

#include "SubWindowGame.h"

class MenuHandler : public Window {
	void createWin() override;
public:
	OptionsWindow* wMain;
	OptionsWindow* wChoose;

	MenuHandler(const int& sizeY, const int& sizeX, WINDOW* parent = nullptr);
};

