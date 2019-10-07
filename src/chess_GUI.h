#ifndef CHESS_GUI_H
#define CHESS_GUI_H
#include <nanogui/nanogui.h>
#include <vector>
#include <iostream>

#include "chess_pieces.h"

using namespace nanogui;

// Player and computer color, PLAYER or COM color times chess piece (e.g. COM*W_KING) is >0 for own pieces and <0 for enemy pieces
const char PLAYER = Chess_Color::White;
const char COM = Chess_Color::Black;

// Color of the window background (around the board)
const Color WIN_COLOR = Color(0x23, 0x27, 0x2A, 255);

enum UI_STATE {MAIN_MENU, SETTINGS, CREDITS, IN_GAME, GAMEOVER, PAUSED};

using namespace std;

// Chess board widget
class chess_GUI : public Screen
{
public:
    chess_GUI(int W, Board &startposition);
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);

    virtual void draw(NVGcontext *ctx);
    virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers);

    virtual void drawContents();

private:
    UI_STATE ui_state = UI_STATE::MAIN_MENU;
    Board *board;
    std::vector<int> click1;
    std::vector<int> click2;
    int chessPieceFontFace;

    void DrawBoard(NVGcontext *ctx);
    void DrawMainMenu(NVGcontext *ctx);
    void DrawSettings(NVGcontext *ctx);
    void DrawCredits(NVGcontext *ctx);

    Color buttonColor = Color(0x72,0x89, 0xDA, 255);
    int buttonWidth = 300;
    int buttonHeight = 70;
    int distButtons = 50;
    int centerOffset = -40;
    vector<string> menuButtons = {"Play", "Settings", "Credits", "Quit"};
    void Add_Buttons(vector<string> buttonTexts);

    nanogui::ref<Window> pausedWindow;

    void HandleInGame(const Vector2i &p);
    void HandleMainMenu(const Vector2i &p);
    void HandleSettings(const Vector2i &p);
    void HandleCredits(const Vector2i &p);

};

#endif // CHESS_GUI.H
