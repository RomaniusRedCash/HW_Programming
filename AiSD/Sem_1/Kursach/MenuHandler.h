#pragma once

#include "SubWindowGame.h"

class MenuHandler : public Window {
	inline static const char RESSUME[] = "Resume";
	inline static const char CHANGE[] = "Change num of disk";
	inline static const char RESTART[] = "Restart";
	inline static const char AUTO[] = "Auto solve";
	inline static const char QUIT[] = "Quit";

	void createWin() override;
public:
	OptionsWindow* wMain;
	OptionsWindow* wChoose;

	MenuHandler(const int& sizeY, const int& sizeX, WINDOW* parent = nullptr);
};

