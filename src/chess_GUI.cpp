#include <vector>
#include "FL/Fl.H"
#include "FL/fl_draw.H"
#include "FL/Fl_Double_Window.H"
#include "chess_gui.h"
#include <iostream>

chess_GUI::chess_GUI(int W, std::vector<std::vector<int>> startposition) : Fl_Widget(0, 0, W, W, 0), board_size(W), s_size(W / 10)
{
    board = startposition;
    click1 = {-1, -1};
    click2 = {-1, -1};

    // If computer is white, start with a computer move
    if (COM == WHITE)
    {
        update_piece(calc_com_move(board));
    }
}

// Draw on screen
void chess_GUI::draw()
{
    // After first click, mark it in upper left corner
    if (click1[0] >= 0)
    {
        fl_rectf(s_size * 0.25, s_size * 0.25, s_size * 0.5, s_size * 0.5, FL_GREEN);
        return;
    }
    else
    {
        fl_rectf(s_size * 0.25, s_size * 0.25, s_size * 0.5, s_size * 0.5, WIN_COLOR);
    }

    // Draw numbers and letters outside board
    fl_color(FL_BLACK);
    fl_font(FL_HELVETICA, s_size * 0.75);
    char row[2] = {'8', '\0'};
    char col[2] = {'A', '\0'};
    for (int r = 1; r < 9; r++)
    {
        fl_draw(row, 0, s_size * r, s_size, s_size, FL_ALIGN_CENTER);
        fl_draw(row, board_size - s_size, s_size * r, s_size, s_size, FL_ALIGN_CENTER);
        fl_draw(col, s_size * r, 0, s_size, s_size, FL_ALIGN_CENTER);
        fl_draw(col, s_size * r, board_size - s_size, s_size, s_size, FL_ALIGN_CENTER);
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
                fl_rectf(s_size * c, s_size * r, s_size, s_size, FL_WHITE);
            }
            else
            {
                fl_rectf(s_size * c, s_size * r, s_size, s_size, FL_GRAY);
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
            fl_draw(int_to_unicode(board[r][c]), s_size * (c + 1), s_size * (r + 1), s_size, s_size, FL_ALIGN_CENTER);
        }
    }
}

// Handle events (e.g. mouse clicking)
int chess_GUI::handle(int event)
{
    // After 2 clicks move the piece
    if (event == FL_PUSH)
    {
        std::vector<int> click = {Fl::event_y() / s_size - 1, Fl::event_x() / s_size - 1};
        if (click[0] >= 0 && click[0] < 8 && click[1] >= 0 && click[1] < 8)
        {
            if (click1[0] < 0 && board[click[0]][click[1]] != EMPTY)
            {
                click1 = click;
                redraw();
            }
            else
            {
                click2 = click;
                std::vector<int> tmp = click1;
                tmp.insert(tmp.end(), click2.begin(), click2.end());
                update_piece(tmp); // Player move
                click1 = {-1, -1};
                click2 = {-1, -1};
                redraw();
                tmp = calc_com_move(board);
                update_piece(tmp); // Computer move
                print_move(tmp);   // Print to terminal
                redraw();
            }
        }
        return 1;
    }
    return Fl_Widget::handle(event);
}

// Update positions for all pieces, input vector{from_r, from_c, to_r, to_c}
void chess_GUI::update_piece(std::vector<int> mv)
{
    board[mv[2]][mv[3]] = board[mv[0]][mv[1]];
    board[mv[0]][mv[1]] = EMPTY;
}

// Translate piece int value to corresponding unicode character
const char *chess_GUI::int_to_unicode(int i)
{
    switch (i)
    {
    case W_KING:
        return u8"\u2654";
    case W_QUEEN:
        return u8"\u2655";
    case W_ROOK:
        return u8"\u2656";
    case W_BISHOP:
        return u8"\u2657";
    case W_KNIGHT:
        return u8"\u2658";
    case W_PAWN:
        return u8"\u2659";
    default:
        return " ";
    case B_KING:
        return u8"\u265A";
    case B_QUEEN:
        return u8"\u265B";
    case B_ROOK:
        return u8"\u265C";
    case B_BISHOP:
        return u8"\u265D";
    case B_KNIGHT:
        return u8"\u265E";
    case B_PAWN:
        return u8"\u265F";
    }
}
