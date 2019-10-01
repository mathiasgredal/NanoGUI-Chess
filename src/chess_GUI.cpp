#include <vector>
#include "FL/Fl.H"
#include "FL/fl_draw.H"
#include "FL/Fl_Double_Window.H"
#include "chess_gui.h"
#include <iostream>

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

// Draw on screen
void chess_GUI::draw()
{

    // After first click, mark it in upper left corner
    if (click1[0] >= 0)
    {
        // TODO: Add suggestions for where it is possible to move
        fl_rectf(board->s_size * 0.25, board->s_size * 0.25, board->s_size * 0.5, board->s_size * 0.5, FL_GREEN);
        return;
    }
    else
    {
        fl_rectf(board->s_size * 0.25, board->s_size * 0.25, board->s_size * 0.5, board->s_size * 0.5, WIN_COLOR);
    }

    // Draw numbers and letters outside board
    fl_color(FL_BLACK);
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
                fl_rectf(board->s_size * c, board->s_size * r, board->s_size, board->s_size, FL_WHITE);
            }
            else
            {
                fl_rectf(board->s_size * c, board->s_size * r, board->s_size, board->s_size, FL_GRAY);
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
}

// Handle events (e.g. mouse clicking)
int chess_GUI::handle(int event)
{
    // After 2 clicks move the piece
    if (event == FL_PUSH)
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
                bool validmove = true;
                /*
                if(board->Get_Piece(tmp.r1, tmp.c1)->chess_type == Chess_Type::Rook
                        && board->Get_Piece(tmp[0],tmp[1])->color == PLAYER){
                    cout << "Checking move\n";
                }*/

                if(validmove){
                    cout << "Valid move\n";
                    board->Move_Piece(tmp);
                    click1 = {-1, -1};
                    click2 = {-1, -1};
                    redraw();
                    //update_piece(tmp); // Computer move
                    redraw();
                }
                else{
                    cout << "Invalid move\n";
                }

            }
        }
        return 1;
    }
    return Fl_Widget::handle(event);
}


