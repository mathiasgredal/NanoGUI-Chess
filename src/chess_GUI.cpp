#include "chess_gui.h"

chess_GUI::chess_GUI(int W, Board& startposition) : Fl_Widget(0, 0, W, W, 0)
{
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


void chess_GUI::DrawBoard()
{
    fl_rectf(0,0,board->board_size,board->board_size, WIN_COLOR);

    // Draw numbers and letters outside board
    fl_color(fl_darker(FL_WHITE));
    fl_font(FL_HELVETICA, board->s_size * 0.75);
    char row[2] = {'8', '\0'};
    char col[2] = {'A', '\0'};
    for (int r = 1; r < 9; r++)
    {
        fl_draw(row, 0, board->s_size * r, board->s_size, board->s_size, FL_ALIGN_CENTER);
        fl_draw(row, board->board_size - board->s_size, board->s_size * r, board->s_size, board->s_size, FL_ALIGN_CENTER);
        fl_draw(col, board->s_size * r, 0, board->s_size, board->s_size, FL_ALIGN_CENTER);
        fl_draw(col, board->s_size * r, board->board_size - board->s_size, board->s_size, board->s_size, FL_ALIGN_CENTER);
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
                fl_rectf(board->s_size * c, board->s_size * r, board->s_size, board->s_size,fl_darker(FL_WHITE));
            }
            else
            {
                fl_rectf(board->s_size * c, board->s_size * r, board->s_size, board->s_size, fl_darker(FL_DARK1));
            }
            white = !white;
        }
        white = !white;
    }

    // Draw pieces
    fl_color(FL_BLACK);
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            fl_draw(board->Get_Piece(r,c)->iconLetter, board->s_size * (c + 1), board->s_size * (r + 1), board->s_size, board->s_size, FL_ALIGN_CENTER);
        }
    }

    // After first click, mark it in upper left corner, and draw possible moves
    if (click1[0] >= 0)
    {
        fl_rectf(board->s_size * 0.25, board->s_size * 0.25, board->s_size * 0.5, board->s_size * 0.5, FL_GREEN);


        vector<Move> possibleMoves = board->Get_Piece(click1[0], click1[1])->ValidMoves(*board);


        for(auto& possibleMove: possibleMoves){
            fl_color(FL_RED);
            possibleMove.Print_Move();
            float r = board->s_size*0.3;

            float x = board->s_size * (possibleMove.c2 + 1)+board->s_size*0.5-r*0.5;
            float y = board->s_size * (possibleMove.r2 + 1)+board->s_size*0.5-r*0.5;


            fl_pie(x, y, r, r, -180,180);
        }

    }
    else
        fl_rectf(board->s_size * 0.25, board->s_size * 0.25, board->s_size * 0.5, board->s_size * 0.5, WIN_COLOR);

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

        fl_rectf(x, y, buttonWidth, buttonHeight, buttonColor);

        fl_color(FL_WHITE);
        fl_font(FL_HELVETICA, 35);
        fl_draw(buttonTexts.at(i).c_str(), board->board_size/2, y+buttonHeight/2, 0,0, FL_ALIGN_CENTER);
    }

}

void chess_GUI::DrawMainMenu()
{
    // Draw Title
    fl_color(FL_WHITE);
    fl_font(FL_HELVETICA, 60);
    fl_draw("Skak - Programmering", board->board_size/2,board->board_size/10, 0,0, FL_ALIGN_CENTER);

    // Add Buttons: Play, Settings, Credits, Quit
    Add_Buttons(menuButtons);

    // Add names
    fl_color(FL_WHITE);
    fl_font(FL_HELVETICA, 12);
    fl_draw("Made By: Mathias Gredal, Niklas Haim, Peter - 2.X", board->board_size/2, board->board_size*0.97f, 0,0, FL_ALIGN_CENTER);
}

void chess_GUI::DrawSettings()
{
    fl_rectf(0,0,board->board_size,board->board_size, WIN_COLOR);

    fl_color(FL_WHITE);

    fl_draw("Yeet", board->board_size/2, board->board_size/10);
}

void chess_GUI::DrawCredits()
{

}

// Draw on screen
void chess_GUI::draw()
{
    switch (ui_state)
    {
    case UI_STATE::MAIN_MENU:
        DrawMainMenu();
        break;
    case UI_STATE::SETTINGS:
        DrawSettings();
        break;
    case UI_STATE::IN_GAME:
        DrawBoard();
        break;
    case UI_STATE::PAUSED:
        break;
    case UI_STATE::GAMEOVER:
        break;
    }
}

void chess_GUI::HandleInGame(int event)
{
    std::vector<int> click = {Fl::event_y() / board->s_size - 1, Fl::event_x() / board->s_size - 1};
    if (click[0] >= 0 && click[0] < 8 && click[1] >= 0 && click[1] < 8)
    {
        if (click1[0] < 0 && board->Get_Piece(click[0],click[1])->chess_type != Chess_Type::EMPTY)
        {
            click1 = click;
            redraw();
        }
        else
        {
            click2 = click;
            Move tmp = Move(click1[0], click1[1], click2[0], click2[1]);
            bool validmove = board->Get_Piece(click1[0],click1[1])->ValidMove(tmp, *board);

            /*
                if(board->Get_Piece(tmp.r1, tmp.c1)->chess_type == Chess_Type::Rook
                        && board->Get_Piece(tmp[0],tmp[1])->color == PLAYER){
                    cout << "Checking move\n";
                }*/

            if(validmove){
                cout << "Valid move\n";
                board->Move_Piece(tmp);

                //update_piece(tmp); // Computer move
                redraw();
            }
            else{
                cout << "Invalid move\n";
            }
            click1 = {-1, -1};
            click2 = {-1, -1};
            redraw();

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

void chess_GUI::HandleMainMenu(int event)
{
    std::vector<int> click = {Fl::event_x(), Fl::event_y()};

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
                redraw();
                return;
            case 1: // Settings
                ui_state = UI_STATE::SETTINGS;
                redraw();
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

void chess_GUI::HandleSettings(int event)
{


}

void chess_GUI::HandleCredits(int event)
{

}


// Handle events (e.g. mouse clicking)
int chess_GUI::handle(int event)
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
}


