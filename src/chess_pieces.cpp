#include "chess_pieces.h"

using namespace std;

Move::Move(u_char row1, u_char col1, u_char row2, u_char col2)
{
    r1 = row1;
    c1 = col1;
    r2 = row2;
    c2 = col2;
}

void Move::Print_Move()
{
    cout << "Move from " << char('A' + c1) << 8 - r1;
    cout << " to " << char('A' + c2) << 8 - r2 << endl;
}

// Metode til at konvertere til vektor
vector<u_char> Move::to_vector()
{
    return vector<u_char>({ r1, c1, r2, c2 });
}

Chess_Piece::Chess_Piece(u_char _row, u_char _col, Chess_Color _color, Chess_Type _chessType, bool _initialMove)
{
    row = _row;
    col = _col;
    color = _color;
    chessType = _chessType;
    initialMove = _initialMove;
}

bool operator<(const Chess_Piece& b1, const Chess_Piece& b2)
{
    return (b1.row < b2.row && b1.col < b2.col);
}

bool operator==(const Chess_Piece& b1, const Chess_Piece& b2)
{
    return (b1.row == b2.row && b1.col == b2.col && b1.color == b2.color && b1.chessType == b2.chessType && b1.initialMove == b2.initialMove);
}

int ChessPieceValue(Chess_Type chessType)
{
    switch (chessType) {
    case King:
        return 150000;
    case Queen:
        return 15;
    case Bishop:
        return 5;
    case Knight:
        return 5;
    case Rook:
        return 5;
    case Pawn:
        return 1;
    case EMPTY:
        return 0;
    }
}

const char* ChessPieceIcon(Chess_Type chessType, Chess_Color color)
{
    if (color == Chess_Color::White) {
        switch (chessType) {
        case King:
            return "♔";
        case Queen:
            return "♕";
        case Bishop:
            return "♗";
        case Knight:
            return "♘";
        case Rook:
            return "♖";
        case Pawn:
            return "♙";
        case EMPTY:
            return "";
        }
    } else if (color == Chess_Color::Black) {
        switch (chessType) {
        case King:
            return "♚";
        case Queen:
            return "♛";
        case Bishop:
            return "♝";
        case Knight:
            return "♞";
        case Rook:
            return "♜";
        case Pawn:
            return "♟";
        case EMPTY:
            return "";
        }
    } else if (color == Chess_Color::Empty) {
        return "";
    }
}

u_char GetChessPieceIndex(u_char row, u_char col)
{
    return 8 * row + col;
}
