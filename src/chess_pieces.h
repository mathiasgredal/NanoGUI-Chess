#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include <algorithm>
#include <array>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

enum Chess_Type { King,
    Queen,
    Bishop,
    Knight,
    Rook,
    Pawn,
    EMPTY };

enum Chess_Color { Black,
    White,
    Empty };

class Move {
public:
    // Initialisering
    u_char r1, c1, r2, c2;

    // Constructor
    Move(u_char row1, u_char col1, u_char row2, u_char col2);

    // Metode til at konvertere til vektor
    vector<u_char> to_vector();

    void Print_Move();

    bool operator==(Move mv)
    {
        if (r1 == mv.r1 && r2 == mv.r2 && c1 == mv.c1 && c2 == mv.c2)
            return true;

        return false;
    }
};

class Chess_Piece {
public:
    u_char row;
    u_char col;
    Chess_Color color;
    Chess_Type chessType;
    bool initialMove;

    Chess_Piece(u_char _row = 0, u_char _col = 0, Chess_Color _color = Chess_Color::Empty, Chess_Type _chessType = Chess_Type::EMPTY, bool _initialMove = true);
    friend bool operator==(const Chess_Piece& b1, const Chess_Piece& b2);
    friend bool operator<(const Chess_Piece& b1, const Chess_Piece& b2);
};

class Board {
public:
    // Constructor
    Board(array<Chess_Piece, 64> pieces);

    // Normal chess layout
    static array<Chess_Piece, 64> Default_Board();

    // Board functions
    Chess_Piece Get_Piece(u_char row, u_char col);
    Chess_Piece GetKing(Chess_Color color);
    vector<Chess_Piece> GetPiecesOfColor(Chess_Color color);
    void Move_Piece(Move mv);
    void UndoMove();
    int GetBoardScore();

    // Move functions
    vector<Move> ValidMoves(u_char row, u_char col);
    bool ValidMove(Move usermove);

    // Endgame functions
    bool IsAttacked(u_char row, u_char col, Chess_Color color);
    bool IsCheck();
    bool IsCheckMate();
    // bool IsStalemate();

    // Member variables
    array<Chess_Piece, 64> chess_pieces;
    Chess_Color currentPlayer = Chess_Color::White;
    array<Chess_Piece, 64> lastMove = {};

    // Extra variables
    int board_size {};
    int s_size {};
};

int ChessPieceValue(Chess_Type chessType);

const char* ChessPieceIcon(Chess_Type chessType, Chess_Color color);

u_char GetChessPieceIndex(u_char row, u_char col);

#endif // CHESS_PIECES_H
