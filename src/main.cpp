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

#include "chess_pieces.h"
#include "chess_GUI.h"

using namespace std;

std::vector<unsigned char> readFile(const char* filename)
{
    // open the file:
    std::ifstream file(filename, std::ios::binary);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<unsigned char> vec;
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<unsigned char>(file),
               std::istream_iterator<unsigned char>());

    return vec;
}

int main()
{
    srand((unsigned)time(0));
    int win_size = 10 * 70; // (8+2) x (8+2) squares of 70 pixels

    nanogui::init();

    {
        nanogui::ref<chess_GUI> chess_board = new chess_GUI(win_size);

        // Do some error handling here. Eg. create folders and download font file online.
        auto fontdata = readFile("./Assets/Fonts/FreeSerif.ttf");
        nvgCreateFontMem(chess_board->nvgContext(), "chess", fontdata.data(), fontdata.size(), 0);

        chess_board->setVisible(true);

        nanogui::mainloop();
    }

    nanogui::shutdown();

    return 0;
}
// COM MOVE - SLUT
