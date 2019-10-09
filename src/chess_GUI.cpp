#include "chess_gui.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>

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




chess_GUI::chess_GUI(int W, Board& startposition) : nanogui::Screen(Eigen::Vector2i(W, W), "Skak", false)
{
    setBackground(WIN_COLOR);

    auto fontdata = readFile("./Assets/Fonts/FreeSerif.ttf");
    //unsigned char* fontdatapointer = fontdata.data();
    //int fontsize = fontdata.size();
    chessPieceFontFace = nvgCreateFontMem(nvgContext(), "chess", fontdata.data(), fontdata.size(), 0);

    cout << chessPieceFontFace << endl;

    /*
    Theme* newTheme = new Theme(nvgContext());

    newTheme->mWindowHeaderGradientTop = Color(0x23,0x27,0x2A,255);
    newTheme->mWindowHeaderGradientBot = newTheme->mWindowHeaderGradientTop;

    newTheme->mWindowTitleFocused = Color(255,255,255,255);
    newTheme->mWindowTitleUnfocused = Color(255,255,255,255);

    newTheme->mWindowCornerRadius = 0;
    newTheme->mWindowDropShadowSize = 3;

    setTheme(newTheme);
    */


    /// dvar, bar, strvar, etc. are double/bool/string/.. variables

    FormHelper *gui = new FormHelper(this);
    pausedWindow  = gui->addWindow(Eigen::Vector2i(10, 10), "Paused");

    gui->addButton("Continue", [this](){ ui_state = UI_STATE::IN_GAME; });
    gui->addButton("Quit to Main Menu", [this](){ ui_state = UI_STATE::MAIN_MENU; pausedWindow->setVisible(false); });
    gui->addButton("Quit", [this](){ exit(1); });

    pausedWindow->center();
    pausedWindow->setFocused(true);
    pausedWindow->performLayout(nvgContext());

    pausedWindow->setVisible(false);

    board = &startposition;
    board->board_size = W;
    board->s_size = W/10;

    click1 = {-1, -1};
    click2 = {-1, -1};

    // If computer is white, start with a computer move
    if (COM == Chess_Color::White)
    {
        //update_piece(calc_com_move(board));
    }
}


void chess_GUI::DrawBoard(NVGcontext *ctx)
{
    // Draw numbers and letters outside board
    nvgBeginPath(nvgContext());
    nvgFontSize(nvgContext(), board->s_size);
    nvgFontFace(nvgContext(), "sans");
    nvgFillColor(nvgContext(), nvgRGBA(200, 200, 200, 255)); // Text color
    nvgTextAlign(nvgContext(), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

    char row[2] = {'8', '\0'};
    char col[2] = {'A', '\0'};
    for (int r = 1; r < 9; r++)
    {
        nvgText(nvgContext(), board->s_size/2, board->s_size * r+board->s_size/2, row, nullptr);
        nvgText(nvgContext(), board->board_size - board->s_size/2 , board->s_size * r + board->s_size/2, row, nullptr);
        nvgText(nvgContext(), board->s_size * r + board->s_size/2, board->s_size/2, col, nullptr);
        nvgText(nvgContext(), board->s_size * r + board->s_size/2, board->board_size-board->s_size/2, col, nullptr);

        row[0] = row[0] - 1;
        col[0] = col[0] + 1;
    }

    // Draw squares
    bool white = true;
    for (int r = 1; r < 9; r++)
    {
        for (int c = 1; c < 9; c++)
        {
            if (white)
            {
                nvgBeginPath(nvgContext());
                nvgRect(nvgContext(), board->s_size * c, board->s_size * r, board->s_size, board->s_size);
                nvgFillColor(nvgContext(), nvgRGBA(180, 180, 180, 255));
                nvgFill(nvgContext());
            }
            else
            {
                nvgBeginPath(nvgContext());
                nvgRect(nvgContext(), board->s_size * c, board->s_size * r, board->s_size, board->s_size);
                nvgFillColor(nvgContext(), nvgRGBA(120, 120, 120, 255));
                nvgFill(nvgContext());

            }
            white = !white;
        }
        white = !white;
    }

    // Draw pieces
    nvgBeginPath(nvgContext());
    nvgFontSize(nvgContext(), board->s_size);
    nvgFontFace(nvgContext(), "chess");
    nvgFillColor(nvgContext(), nvgRGBA(0, 0, 0, 255)); // Text color
    nvgTextAlign(nvgContext(), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {

            nvgText(nvgContext(), board->s_size * (c + 1.5), board->s_size * (r + 1.5), board->Get_Piece(r,c)->iconLetter , nullptr);
        }
    }

    // After first click, mark it in upper left corner, and draw possible moves
    if (click1[0] >= 0 && board->Get_Piece(click1[0], click1[1])->color == board->currentPlayer)
    {
        nvgBeginPath(nvgContext());
        nvgRect(nvgContext(), board->s_size * 0.25, board->s_size * 0.25, board->s_size * 0.5, board->s_size * 0.5);
        nvgFillColor(nvgContext(), Color(0,255,0,255)); // Text color
        nvgFill(nvgContext());

        //fl_rectf(board->s_size * 0.25, board->s_size * 0.25, board->s_size * 0.5, board->s_size * 0.5, FL_GREEN);


        vector<Move> possibleMoves = board->Get_Piece(click1[0], click1[1])->ValidMoves(*board);


        for(auto& possibleMove: possibleMoves){
            float r = board->s_size*0.16;

            float x = board->s_size * (possibleMove.c2 + 1)+board->s_size*0.5;
            float y = board->s_size * (possibleMove.r2 + 1)+board->s_size*0.5;

            nvgBeginPath(nvgContext());
            nvgCircle(nvgContext(), x, y, r);
            nvgFillColor(nvgContext(), nvgRGBA(255, 0, 0, 255)); // Text color
            nvgFill(nvgContext());
            //possibleMove.Print_Move();
        }

    }
    else {
        nvgBeginPath(nvgContext());
        nvgRect(nvgContext(), board->s_size * 0.25, board->s_size * 0.25, board->s_size * 0.5, board->s_size * 0.5);
        nvgFillColor(nvgContext(), WIN_COLOR); // Text color
        nvgFill(nvgContext());
        //fl_rectf(board->s_size * 0.25, board->s_size * 0.25, board->s_size * 0.5, board->s_size * 0.5, WIN_COLOR);
    }
}

void chess_GUI::Add_Buttons(vector<string> buttonTexts)
{
    auto totalButtonHeight = (distButtons*(buttonTexts.size()-1)+buttonHeight*(buttonTexts.size()-1));

    for (int i = 0; i < buttonTexts.size(); i++)
    {
        // i = 1 and offset = totalButtonHeight-buttonheight/2
        // i = 2 and offset =

        int offset = totalButtonHeight-distButtons*i-buttonHeight*i+centerOffset;
        int x = board->board_size/2-buttonWidth/2;
        int y = board->board_size/2-buttonHeight/2-(offset-totalButtonHeight/2);

        nvgBeginPath(nvgContext());
        nvgRect(nvgContext(), x, y, buttonWidth, buttonHeight);
        nvgFillColor(nvgContext(), buttonColor);
        nvgFill(nvgContext());

        nvgBeginPath(nvgContext());
        nvgFontSize(nvgContext(), 35.0f);
        nvgFontFace(nvgContext(), "sans");
        nvgFillColor(nvgContext(), nvgRGBA(255, 255, 255, 255));
        nvgTextAlign(nvgContext(), NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgText(nvgContext(), board->board_size/2, y+buttonHeight/2, buttonTexts.at(i).c_str(), NULL);
    }

}

void chess_GUI::DrawMainMenu(NVGcontext *ctx)
{
    // Draw Title
    nvgBeginPath(ctx);
    nvgFontSize(ctx, 60.0f);
    nvgFontFace(ctx, "sans");
    nvgFillColor(ctx, nvgRGBA(255, 255, 255, 255));
    nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgText(ctx, board->board_size/2,board->board_size/10, "Skak - Programmering", NULL);

    // Add Buttons: Play, Settings, Credits, Quit
    Add_Buttons(menuButtons);

    // Add names
    nvgBeginPath(ctx);
    nvgFontSize(ctx, 14.0f);
    nvgFontFace(ctx, "sans");
    nvgFillColor(ctx, nvgRGBA(255, 255, 255, 255));
    nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgText(ctx, board->board_size/2, board->board_size*0.97f, "Made By: Mathias Gredal, Niklas Haim, Peter - 2.X", NULL);

}

void chess_GUI::DrawSettings(NVGcontext *ctx)
{
    /*
    fl_rectf(0,0,board->board_size,board->board_size, WIN_COLOR);

    fl_color(FL_WHITE);

    fl_draw("Yeet", board->board_size/2, board->board_size/10);
    */
}

void chess_GUI::DrawCredits(NVGcontext *ctx)
{

}

bool chess_GUI::keyboardEvent(int key, int scancode, int action, int modifiers)
{
    if(key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        if(ui_state != UI_STATE::PAUSED)
            ui_state = UI_STATE::PAUSED;
        else
            ui_state = UI_STATE::IN_GAME;
    }
    cout << "you clicked " << key << endl;
}

bool chess_GUI::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers)
{
    Screen::mouseButtonEvent(p, button, down, modifiers);

    // After 2 clicks move the piece
    if (down)
    {
        switch (ui_state)
        {
        case UI_STATE::MAIN_MENU:
            HandleMainMenu(p);
            break;
        case UI_STATE::SETTINGS:
            HandleSettings(p);
            break;
        case UI_STATE::IN_GAME:
            if(button == GLFW_MOUSE_BUTTON_1 && down)
            HandleInGame(p);
            break;
        case UI_STATE::PAUSED:
            break;
        case UI_STATE::GAMEOVER:
            break;
        }
    }

    return 0;
}


void chess_GUI::draw(NVGcontext *ctx)
{


    switch (ui_state)
    {
    case UI_STATE::MAIN_MENU:
        DrawMainMenu(ctx);
        break;
    case UI_STATE::SETTINGS:
        DrawSettings(ctx);
        break;
    case UI_STATE::IN_GAME:
        DrawBoard(ctx);
        if(pausedWindow->visible())
            pausedWindow->setVisible(false);
        break;
    case UI_STATE::PAUSED:
        DrawBoard(ctx);
        if(!pausedWindow->visible())
            pausedWindow->setVisible(true);

        break;
    case UI_STATE::GAMEOVER:
        break;
    }

    Screen::draw(ctx);
}

void chess_GUI::drawContents()
{

}


void chess_GUI::HandleInGame(const Vector2i &p)
{
    std::vector<int> click = {p.y() / board->s_size - 1, p.x() / board->s_size - 1};

    if (click[0] >= 0 && click[0] < 8 && click[1] >= 0 && click[1] < 8)
    {
        if (click1[0] < 0 && board->Get_Piece(click[0],click[1])->chess_type != Chess_Type::EMPTY)
        {
            if(board->Get_Piece(click[0], click[1])->color != board->currentPlayer)
            {
                click1 = {-1, -1};
                click2 = {-1, -1};
            }
            else
            {
                click1 = click;
                drawAll();
            }
        }
        else
        {
            click2 = click;
            Move tmp = Move(click1[0], click1[1], click2[0], click2[1]);


            bool validmove = board->Get_Piece(click1[0],click1[1])->ValidMove(tmp, *board);

            if(board->Get_Piece(tmp.r1, tmp.c1)->color != board->currentPlayer)
                validmove = false;

            if(validmove){
                cout << "Valid move" << endl;
                board->Move_Piece(tmp);

                //update_piece(tmp); // Computer move
                drawAll();
            }
            else{
                cout << "Invalid move" << endl;
            }
            click1 = {-1, -1};
            click2 = {-1, -1};
            drawAll();

        }
    }

}


bool Click_Within_Range(vector<int> range, vector<int> inputclick)
{
    if(inputclick[0] >= range[0] && inputclick[0] <= range[2])
    {
        if(inputclick[1] >= range[1] && inputclick[1] <= range[3])
        {
            return true;
        }
    }

    return false;
}

void chess_GUI::HandleMainMenu(const Vector2i &p)
{
    std::vector<int> click = {p.x(), p.y()};

    std::vector<vector<int>> buttonRanges = {};

    auto totalButtonHeight = (distButtons*(menuButtons.size()-1)+buttonHeight*(menuButtons.size()-1));

    for (int i = 0; i < menuButtons.size(); i++)
    {
        int offset = totalButtonHeight-distButtons*i-buttonHeight*i+centerOffset;
        int x = board->board_size/2-buttonWidth/2;
        int y = board->board_size/2-buttonHeight/2-(offset-totalButtonHeight/2);

        buttonRanges.push_back({x,y,x+buttonWidth, y+buttonHeight});

    }


    for(int i = 0; i < buttonRanges.size(); i++)
    {
        if(Click_Within_Range(buttonRanges[i], click))
        {
            switch(i)
            {
            case 0: // Play
                ui_state = UI_STATE::IN_GAME;
                drawAll();
                return;
            case 1: // Settings
                ui_state = UI_STATE::SETTINGS;
                drawAll();
                return;
            case 2: // Credits
                return;
            case 3: // Quit
                exit(1);
                return;
            }
        }
    }
}

void chess_GUI::HandleSettings(const Vector2i &p)
{


}

void chess_GUI::HandleCredits(const Vector2i &p)
{

}


// Handle events (e.g. mouse clicking)
/*int chess_GUI::handle(int event)
{
    // After 2 clicks move the piece
    if (event == FL_PUSH)
    {
        switch (ui_state)
        {
        case UI_STATE::MAIN_MENU:
            HandleMainMenu(event);
            break;
        case UI_STATE::SETTINGS:
            HandleSettings(event);
            break;
        case UI_STATE::IN_GAME:
            HandleInGame(event);
            break;
        case UI_STATE::PAUSED:
            break;
        case UI_STATE::GAMEOVER:
            break;
        }

        return 1;
    }

    return Fl_Widget::handle(event);

    return 0;
}*/


