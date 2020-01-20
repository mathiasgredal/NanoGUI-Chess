#define GL_SILENCE_DEPRECATION true

#include <cmrc/cmrc.hpp>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <math.h>
#include <nanogui/nanogui.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "chess_GUI.h"
#include "chess_pieces.h"

using namespace std;

CMRC_DECLARE(Assets);

void Start()
{
    srand((unsigned)time(0));
    int win_size = 10 * 70; // (8+2) x (8+2) squares of 70 pixels

    auto chessfont = cmrc::Assets::get_filesystem().open("Assets/FreeSerif.ttf");
    std::vector<unsigned char> chessFontData(chessfont.begin(), chessfont.end());

    nanogui::init();
    {
        nanogui::ref<chess_GUI> chess_board = new chess_GUI(win_size);

        nvgCreateFontMem(chess_board->nvgContext(), "chess", chessFontData.data(), chessFontData.size(), 0);

        chess_board->setVisible(true);

        nanogui::mainloop();
    }

    nanogui::shutdown();
}

int main()
{
    Start();

    return 0;
}
// COM MOVE - SLUT
