#include "Menu.h"
#include "Fun.h"

void Menu::ThrowErr(const char* IsErr) {
	std::cout << "\n\t" << IsErr << std::endl;
}

void Menu::AnalizIn(const char* Set) {
	XSetForm XSF(Set);
	if (XSF == "help") {
		std::cout <<
			"\nhelp - this page" <<
			"\nshow - show all content" <<
			"\nfilter - find route" <<
			"\nmake - read you file and analize it" 
			<< std::endl;
		sost = Help;
		return;
	}
	if (XSF == "show") {

	}
}
