#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>

#include "Chess_Pieces/chess_pieces.h"

#include "chess_GUI.h"


using namespace std;


// Random int generator
int randi(int low, int high)
{
    return rand() * float(high - low) / RAND_MAX + low;
}

void AddPiecesToBoard(Board &board)
{

}


int main()
{
    cout << "yeeha\n";

    int win_size = 10 * 70; // (8+2) x (8+2) squares of 70 pixels
    srand(time(NULL));      // Initialize random generator
    rand();



    Board board(Board::Default_Board());

    AddPiecesToBoard(board);


    // Initialize window and chess board
    Fl_Double_Window win(win_size, win_size, "Chess");

    win.color(WIN_COLOR);

    chess_GUI chess_board(win_size, board);

    win.end();
    win.show();

    return (Fl::run());

}
// COM MOVE - SLUT
