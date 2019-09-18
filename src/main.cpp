#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
#include "chess_GUI.h"
using namespace std;

// Random int generator
int randi(int low, int high)
{
    return rand() * float(high - low) / RAND_MAX + low;
}



void print_move(vector<int> mv)
{
    cout << "Move from " << char('A' + mv[1]) << 8 - mv[0];
    cout << " to " << char('A' + mv[3]) << 8 - mv[2] << endl;
}

// Print all moves in a vector for control purposes
void print_moves(vector<vector<int>> moves)
{
    for (unsigned int k = 0; k < moves.size(); k++)
    {
        print_move(moves[k]);
    }
}

// Find all pieces of given type
vector<vector<int>> find_pieces(vector<vector<int>> board, int type)
{
    // Find pieces and return list with {row, col} positions
    vector<vector<int>> list_pieces;
    for (int r = 0; r < int(board.size()); r++)
    {
        for (int c = 0; c < int(board[r].size()); c++)
        {
            if (board[r][c] == type)
            {
                list_pieces.push_back({r, c});
            }
        }
    }
    return list_pieces;
}

// TRIN 1 - UDGANGSPUNKT
// Calculate valid moves for a rook piece
vector<vector<int>> valid_moves_rook(vector<vector<int>> board, int pos_r, int pos_c, int color)
{
    vector<vector<int>> moves; // Each row in this vector is a legal move, e.g. {1,3, 5,7} indicates a move from row 1, col 3 to row 5, col 7

    // Going right
    for (int c = pos_c + 1; c < 8; c++)
    {
        if (color * board[pos_r][c] <= 0)
        {
            moves.push_back({pos_r, pos_c, pos_r, c}); // If empty or enemy piece, it's a legal move
        }
        if (board[pos_r][c] != EMPTY)
        {
            break; // Not empty: Collision detected, break the loop
        }
    }

    print_moves(moves);

    return moves;
}
// TRIN 1 SLUT

// TRIN 2 - UDGANGSPUNKT
// Evaluate and score a board position
float eval_pos(vector<vector<int>> board)
{
    float sum = 0;
    for (int r = 0; r < board.size(); r++)
    {
        for (int c = 0; c < board[r].size(); c++)
        {
            sum += board[r][c];
        }
    }
    cout << "Board Evaluation: " << sum << endl;
    return sum;
}
// TRIN 2 SLUT

// TRIN 3 - UDGANGSPUNKT
vector<int> findBestMove(vector<vector<int>> board, vector<vector<int>> moves_list, int color)
{
    int maxScore = -1000;
    vector<int> bestMove, currentMove;
    vector<vector<int>> tmpBoard = board;

    for (unsigned int i = 0; i < moves_list.size(); i++)
    {
        currentMove = moves_list[i];

        // Move the piece on temporary board
        tmpBoard[currentMove[2]][currentMove[3]] = tmpBoard[currentMove[0]][currentMove[1]];
        tmpBoard[currentMove[0]][currentMove[1]] = EMPTY;

        int score = eval_pos(tmpBoard);

        if (color * score > maxScore)
        {
            maxScore = color * score;
            bestMove = currentMove;
        }

        tmpBoard = board;
    }

    return bestMove;
}
//TRIN 3 - SLUT

// COM MOVE - UDGANGSPUNKT
// Calculate computer move
vector<int> calc_com_move(vector<vector<int>> board)
{
    vector<vector<int>> COM_list;
    vector<vector<int>> current_pieces;

    float score = eval_pos(board); // Evaluate current position

    current_pieces = find_pieces(board, B_ROOK); // Find computer rooks
    COM_list.insert(COM_list.end(), current_pieces.begin(), current_pieces.end());

    int n = COM_list.size();
    if (n == 0)
    {
        cout << "The computer found no pieces to move!" << endl;
        exit(0);
    }

    vector<int> mv = COM_list[randi(0, n)]; // Choose random piece
    mv.push_back(randi(0, 8));              // Choose random end location
    mv.push_back(randi(0, 8));
    return mv;
}

int main()
{
    int win_size = 10 * 70; // (8+2) x (8+2) squares of 70 pixels
    srand(time(NULL));      // Initialize random generator
    rand();

    // TRIN 0 - ØVELSE STARTER HER
    vector<vector<int>> board(8, vector<int>(8, 0));
    // Draw black side
    fill(board.at(board.size() - 7).begin(), board.at(board.size() - 7).end(), B_PAWN);
    board.at(0).at(0) = B_ROOK;
    board.at(0).at(8 - 1) = B_ROOK;

    board.at(0).at(1) = B_KNIGHT;
    board.at(0).at(8 - 2) = B_KNIGHT;

    board.at(0).at(2) = B_BISHOP;
    board.at(0).at(8 - 3) = B_BISHOP;

    board.at(0).at(3) = B_QUEEN;
    board.at(0).at(8 - 4) = B_KING;

    // Draw white side
    fill(board.at(board.size() - 2).begin(), board.at(board.size() - 2).end(), W_PAWN);
    board.at(7).at(0) = W_ROOK;
    board.at(7).at(8 - 1) = W_ROOK;

    board.at(7).at(1) = W_KNIGHT;
    board.at(7).at(8 - 2) = W_KNIGHT;

    board.at(7).at(2) = W_BISHOP;
    board.at(7).at(8 - 3) = W_BISHOP;

    board.at(7).at(3) = W_QUEEN;
    board.at(7).at(8 - 4) = W_KING;
    // TRIN 0 - ØVELSE SLUTTER HER

    // Initialize window and chess board
    Fl_Double_Window win(win_size, win_size, "Chess");
    win.color(WIN_COLOR);
    chess_GUI chess_board(win_size, board);
    win.end();
    win.show();
    eval_pos(board);

    return (Fl::run());

}
// COM MOVE - SLUT
