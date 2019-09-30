#ifndef CHESS_GUI_H
#define CHESS_GUI_H

#include <vector>
#include "FL/Fl.H"
#include "FL/fl_draw.H"
#include "FL/Fl_Double_Window.H"

// Chess pieces
const int W_KING = 150;
const int W_QUEEN = 9;
const int W_ROOK = 5;
const int W_BISHOP = 4;
const int W_KNIGHT = 3;
const int W_PAWN = 1;
const int EMPTY = 0;
const int B_KING = -150;
const int B_QUEEN = -9;
const int B_ROOK = -5;
const int B_BISHOP = -4;
const int B_KNIGHT = -3;
const int B_PAWN = -1;

// Black pieces are negative and white are positive
const int BLACK = -1;
const int WHITE = 1;

// Player and computer color, PLAYER or COM color times chess piece (e.g. COM*W_KING) is >0 for own pieces and <0 for enemy pieces
const char PLAYER = WHITE;
const char COM = BLACK;

// Color of the window background (around the board)
const int WIN_COLOR = FL_LIGHT2;

using namespace std;

class Move
{
public:
    // Initialisering
    int r1, c1, r2, c2;

    // Constructor
    Move(int row1,int col1, int row2, int col2)
    {
        r1 = row1;
        c1 = col1;
        r2 = row2;
        c2 = col2;
    };

    // Metode til at konvertere til vektor
    vector<int> to_vector()
    {
        return vector<int>({r1,c1,r2,c2});
    }
};

// Function headers for main.cpp file
void print_move(std::vector<int> mv);
std::vector<int> calc_com_move(std::vector<std::vector<int>> board);
bool valid_ROCK_move(Move mv, vector<vector<int>>board);

// Chess board widget
class chess_GUI : public Fl_Widget
{
public:
    chess_GUI(int W, std::vector<std::vector<int>> startposition);
    void draw() override;
    int handle(int event) override;
    void update_piece(std::vector<int> mv);

private:
    const int board_size;
    const int s_size;
    const char *int_to_unicode(int i);
    Fl_Double_Window *parent_win;
    std::vector<std::vector<int>> board;
    std::vector<int> click1;
    std::vector<int> click2;
};

#endif // CHESS_GUI.H
