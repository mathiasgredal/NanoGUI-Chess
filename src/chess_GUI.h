#ifndef CHESS_GUI_H
#define CHESS_GUI_H

#include <vector>
#include <iostream>

#include "FL/Fl.H"
#include "FL/fl_draw.H"
#include "FL/Fl_Double_Window.H"
#include "chess_pieces.h"


// Player and computer color, PLAYER or COM color times chess piece (e.g. COM*W_KING) is >0 for own pieces and <0 for enemy pieces
const char PLAYER = Chess_Color::White;
const char COM = Chess_Color::Black;

// Color of the window background (around the board)
const int WIN_COLOR = 0x23272A00;

enum UI_STATE {MAIN_MENU, SETTINGS, CREDITS, IN_GAME, GAMEOVER, PAUSED};

using namespace std;

// Chess board widget
class chess_GUI : public Fl_Widget
{
public:
    chess_GUI(int W, Board &startposition);
    void draw() override;
    int handle(int event) override;

private:
    Fl_Double_Window *parent_win;
    UI_STATE ui_state = UI_STATE::MAIN_MENU;
    Board *board;
    std::vector<int> click1;
    std::vector<int> click2;

    void DrawBoard();
    void DrawMainMenu();
    void DrawSettings();
    void DrawCredits();

    int buttonColor = 0x7289DA00;
    int buttonWidth = 300;
    int buttonHeight = 70;
    int distButtons = 50;
    int centerOffset = -40;
    vector<string> menuButtons = {"Play", "Settings", "Credits", "Quit"};
    void Add_Buttons(vector<string> buttonTexts);

    void HandleInGame(int event);
    void HandleMainMenu(int event);
    void HandleSettings(int event);
    void HandleCredits(int event);

};

#endif // CHESS_GUI.H
