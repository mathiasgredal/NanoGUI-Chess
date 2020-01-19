#define GL_SILENCE_DEPRECATION true

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <nanogui/nanogui.h>
#include <cmrc/cmrc.hpp>

#include "chess_pieces.h"
#include "chess_GUI.h"

using namespace std;

CMRC_DECLARE(Assets);


int main()
{
    srand((unsigned)time(0));
    int win_size = 10 * 70; // (8+2) x (8+2) squares of 70 pixels

    auto fontfile = cmrc::Assets::get_filesystem().open("Assets/FreeSerif.ttf");

    std::vector<unsigned char> fontData(fontfile.begin(), fontfile.end());



    nanogui::init();
    {
        nanogui::ref<chess_GUI> chess_board = new chess_GUI(win_size);        

        nvgCreateFontMem(chess_board->nvgContext(), "chess", fontData.data(), fontData.size(), 0);

        chess_board->setVisible(true);

        nanogui::mainloop();
    }

    nanogui::shutdown();

    return 0;
}
// COM MOVE - SLUT
