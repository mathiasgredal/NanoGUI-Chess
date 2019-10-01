#ifndef CHESS_GUI_H
#define CHESS_GUI_H

#include <vector>
#include "FL/Fl.H"
#include "FL/fl_draw.H"
#include "FL/Fl_Double_Window.H"
#include "Chess_Pieces/chess_pieces.h"


// Player and computer color, PLAYER or COM color times chess piece (e.g. COM*W_KING) is >0 for own pieces and <0 for enemy pieces
const char PLAYER = Chess_Color::White;
const char COM = Chess_Color::Black;

// Color of the window background (around the board)
const int WIN_COLOR = FL_LIGHT2;

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
    Board *board;
    std::vector<int> click1;
    std::vector<int> click2;
};

#endif // CHESS_GUI.H
