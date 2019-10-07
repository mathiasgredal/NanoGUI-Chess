#define GL_SILENCE_DEPRECATION true

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <nanogui/nanogui.h>
#include <fontstash.h>

#include "chess_pieces.h"
#include "chess_GUI.h"




using namespace std;


// Random int generator
int randi(int low, int high)
{
    return rand() * float(high - low) / RAND_MAX + low;
}


int main()
{
    int win_size = 10 * 70; // (8+2) x (8+2) squares of 70 pixels
    srand(time(NULL));      // Initialize random generator
    rand();
    nanogui::init();

    Board board(Board::Default_Board());

    {
        nanogui::ref<chess_GUI> chess_board = new chess_GUI(win_size, board);

        chess_board->drawAll();

        chess_board->setVisible(true);

        nanogui::mainloop();
    }

    nanogui::shutdown();

    return 0;
}
// COM MOVE - SLUT
