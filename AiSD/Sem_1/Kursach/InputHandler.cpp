#include "InputHandler.h"
#include "MenuHandler.h"

void InputHandler::useMain() {
    switch (ch)
    {
    case KEY_LEFT:
        horizontalPos--;
        break;
    case KEY_RIGHT:
        horizontalPos++;
        break;
    case KEY_UP:
        verticalPos--;
        break;
    case KEY_DOWN:
        verticalPos++;
        break;
    case 27:
        switch (nowSost)
        {
        case finish:
        case game:
            oldSost = nowSost;
            nowSost = setting;
            break;
        case setting:
            if (oldSost != setting)
                nowSost = oldSost;
            else nowSost = game;
            break;
        case choose:
            if (oldSost != choose)
                nowSost = oldSost;
            else nowSost = setting;
            break;
        default:
            break;
        }
        break;
    default:
        return;
    }
}

void InputHandler::useKeyGame() {
    if (ch >= '1' && ch <= '4') gamePos = ch - '1';
    switch (ch)
    {
    case KEY_LEFT:
        if (gamePos > 0) gamePos--;
        break;
    case KEY_RIGHT:
        if (gamePos < 3) gamePos++;
        break;

    case '\n':
    case ' ':
    case KEY_ENTER:
        if (selectGamePos == -1) {
            if (vHanoys[gamePos].empty()) break;
            selectGamePos = gamePos;
            break;
        }
        if (gamePos != selectGamePos) {
            try {
                moveFromTo(selectGamePos, gamePos);
            }
            catch (const HanErr& e) {
                break;
            }
        }
        selectGamePos = -1;
        break;
    default:
        return;
    }
}

void InputHandler::useSetting() {
    switch (ch)
    {
    case KEY_ENTER:
    case '\n':
        oldSost = nowSost;
        switch (wSetting->wMain->nowSelect())
        {
        case game:
            nowSost = game;
            break;
        case choose:
            oldSost = nowSost;
            nowSost = choose;
            break;
        case restart:
            restartGame();
            break;
        case autoS:
            restartGame();
            FrameRate::newRate();
            autoSolve(numNode, 0, 3, true);
            break;
        case quit:
            nowSost = quit;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void InputHandler::useChoose() {
    switch (ch)
    {
    case KEY_ENTER:
    case '\n':
        numNode = wSetting->wChoose->nowSelect() + 1;
        restartGame();
        nowSost = oldSost;
        break;
    default:
        break;
    }
}

void InputHandler::restartGame() {
    makeTower();
    gamePos = 0;
    selectGamePos = -1;
    hod = 0;
    nowSost = game;
}

int InputHandler::getCh() {
    int chOut = ch;
    ch = ERR;
    return chOut;
}

void InputHandler::init() {
    while (nowSost != quit) {
        ch = getch();
        if (ch == ERR) continue;
        useMain();
        switch (nowSost)
        {
        case game:
            useKeyGame();
            break;
        case setting:
            useSetting();
            break;
        case choose:
            useChoose();
            break;
        case restart:
            break;
        case autoS:
            break;
        case quit:
            break;
        default:
            break;
        }
        FrameRate::newRate();
    }
}

const int8_t& InputHandler::getHorizontalPos() {
    return horizontalPos;
}

const int8_t& InputHandler::getSelectPos() {
    return selectPos;
}

const int8_t& InputHandler::getGamePos() {
    return gamePos;
}

const int8_t& InputHandler::getSelectGamePos() {
    return selectGamePos;
}

const int8_t& InputHandler::getVerticalPos() {
    return verticalPos;
}
