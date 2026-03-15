#pragma once

#include "Window.h"
#include "InputHandler.h"

class TowersWindow : public Window {
    void createWin() override;
public:
    TowersWindow(const int& sizeY, const int& sizeX, WINDOW* win = nullptr);
};

class TextWindow : public Window {
protected:
    std::string text = "text.";
    int textPosY = 0, textPosX = 0;
    bool isSelect = false;

    virtual void createWin() override;
public:
    TextWindow(const int& sizeY, const int& sizeX, WINDOW* win = nullptr) : Window(sizeY, sizeX, win) {}
    
    void setText(const std::string& text) { this->text = text; }
    void setTextPos(const int& y, const int& x);
    void setTextPosCenter();
    void setSelect(const bool& isSelect);

    virtual ~TextWindow() override = default;
};

class PosWindow : public Window {
    const uint8_t sizeSel = 5;

    void createWin() override;
public:
    PosWindow(const int& sizeY, const int& sizeX, WINDOW* win = nullptr) : Window(sizeY, sizeX, win) {}
};

class StatisticWindow : public TextWindow {
    void createWin() override;
public:
    StatisticWindow(const int& sizeY, const int& sizeX, WINDOW* win = nullptr);
};

class OptionsWindow : public Window {
    std::vector<std::string> vSetStr;
    int sizeX;

    void createWin() override;
public:
    OptionsWindow(const int& sizeY, const int& sizeX, WINDOW* win = nullptr);
    void init();
    void addOption(const std::string& str);
    int8_t nowSelect();
    ~OptionsWindow();
};