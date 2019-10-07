#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

enum Chess_Type {King, Queen, Bishop, Knight, Rook, Pawn, EMPTY};
enum Chess_Color {Black, White};

class Board;

class Move
{
public:
    // Initialisering
    int r1, c1, r2, c2;

    // Constructor
    Move(int row1,int col1, int row2, int col2);

    // Metode til at konvertere til vektor
    vector<int> to_vector();

    void Print_Move();

    bool operator == (Move mv){
        if(r1 == mv.r1 && r2 == mv.r2 && c1 == mv.c1 && c2 == mv.c2)
            return true;
        else
            return false;
    }
};


class Chess_Piece
{
    public:
        Chess_Type chess_type;
        Chess_Color color;
        const char* iconLetter;
        int piece_Value;

        virtual bool ValidMove(Move mv, Board &board);

        virtual void RegisterMove(Move mv){

        };

        virtual vector<Move> ValidMoves(Board &board) = 0;

        virtual ~Chess_Piece();
};

class Board
{
public:
    // Initialisering
    vector<vector<Chess_Piece*>> chess_pieces;

    // Constructor
    Board(vector<vector<Chess_Piece*>> pieces);

    static vector<vector<Chess_Piece*>> Default_Board();

    void Add_Piece_To_Board(Chess_Piece* piece, int row, int col);
    Chess_Piece* Get_Piece(int row, int col);
    void Move_Piece(Move mv);
    void GetLocation(Chess_Piece* piece, int& row, int& col);
    int GetBoardScore();
    vector<Move> AllPossibleMoves();

    int board_size;
    int s_size;
};

class Rook : public Chess_Piece
{
    public:
    Rook(int _row, int _col, Chess_Color _color)
    {
        chess_type = Chess_Type::Rook;
        piece_Value = 5;

        if(_color == Chess_Color::White)
            iconLetter = "♖";
        else
            iconLetter = "♜";

        color = _color;
    }

    virtual vector<Move> ValidMoves(Board &board)
    {
        int row, col;
        board.GetLocation(this, row, col);

        vector<Move> possibleMoves = {};

        // Positive horisontal move
        for(int x = col+1; x < 8; x++)
        {
            possibleMoves.push_back(Move(row, col, row, x));
            if(board.Get_Piece(row,x)->chess_type != Chess_Type::EMPTY)
                break;
        }

        // Negative horisontal move
        for(int x = col-1; x >= 0; x--)
        {
            possibleMoves.push_back(Move(row, col, row, x));
            if(board.Get_Piece(row,x)->chess_type != Chess_Type::EMPTY)
                break;
        }

        // Positive vertical move
        for(int x = row+1; x < 8; x++)
        {
            possibleMoves.push_back(Move(row, col, x, col));
            if(board.Get_Piece(x,col)->chess_type != Chess_Type::EMPTY)
                break;
        }

        // Negative vertical move
        for(int x = row-1; x >= 0; x--)
        {
            possibleMoves.push_back(Move(row, col, x, col));
            if(board.Get_Piece(x,col)->chess_type != Chess_Type::EMPTY)
                break;
        }

        return possibleMoves;
    }
};

class King : public Chess_Piece
{
    public:
    King(int _row, int _col, Chess_Color _color)
    {
        chess_type = Chess_Type::King;
        piece_Value = 150;

        if(_color == Chess_Color::White)
            iconLetter = "♔";
        else
            iconLetter = "♚";

        color = _color;
    }

    virtual vector<Move> ValidMoves(Board &board)
    {

        return {Move(0,0,5,5)};
    }
};

class Queen : public Chess_Piece
{
    public:
    Queen(int _row, int _col, Chess_Color _color)
    {
        chess_type = Chess_Type::Queen;
        piece_Value = 15;

        if(_color == Chess_Color::White)
            iconLetter = "♕";
        else
            iconLetter = "♛";

        color = _color;
    }

    virtual vector<Move> ValidMoves(Board &board)
    {
        return {Move(0,0,5,5)};
    }
};

class Bishop : public Chess_Piece
{
    public:
    Bishop(int _row, int _col, Chess_Color _color)
    {
        chess_type = Chess_Type::Bishop;
        piece_Value = 5;

        if(_color == Chess_Color::White)
            iconLetter = "♗";
        else
            iconLetter = "♝";

        color = _color;
    }

    virtual vector<Move> ValidMoves(Board &board)
    {
        return {Move(0,0,5,5)};
    }
};

class Knight : public Chess_Piece
{
    public:
    Knight(int _row, int _col, Chess_Color _color)
    {
        chess_type = Chess_Type::Knight;
        piece_Value = 5;

        if(_color == Chess_Color::White)
            iconLetter = "♘";
        else
            iconLetter = "♞";

        color = _color;
    }

    virtual vector<Move> ValidMoves(Board &board)
    {

        return {Move(0,0,5,5)};
    }
};

class Pawn : public Chess_Piece
{
    public:
    Pawn(int _row, int _col, Chess_Color _color)
    {
        chess_type = Chess_Type::Pawn;
        piece_Value = 1;

        if(_color == Chess_Color::White)
            iconLetter = "♙";
        else
            iconLetter = "♟";

        color = _color;
    }

    virtual void RegisterMove(Move mv)
    {
        initialMove = false;
    }


    virtual vector<Move> ValidMoves(Board &board)
    {
        int row, col;
        board.GetLocation(this, row, col);

        vector<Move> possibleMoves = {};

        int dir = (color==Chess_Color::White)? -1: 1;


        possibleMoves.push_back(Move(row,col,row+dir,col));

        if(initialMove){
            possibleMoves.push_back(Move(row,col,row+2*dir,col));
        }

        return possibleMoves;

    }

private:
    bool initialMove = true;
};

class Empty : public Chess_Piece
{
    public:
    Empty()
    {
        chess_type = Chess_Type::EMPTY;
        piece_Value = 0;
        iconLetter = " ";
    }

    virtual vector<Move> ValidMoves(Board &board)
    {
        return {};
    }
};



#endif // CHESS_PIECES_H
