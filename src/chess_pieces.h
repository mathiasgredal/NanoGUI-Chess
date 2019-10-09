#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include <vector>
#include <algorithm>
#include <iostream>


using namespace std;

enum Chess_Type {King, Queen, Bishop, Knight, Rook, Pawn, EMPTY};
enum Chess_Color {Black, White, Empty};

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

    virtual vector<Move> ValidMoves(Board &board);

    virtual ~Chess_Piece();
};

class Board
{
public:
    // Constructor
    Board(vector<vector<Chess_Piece*>> pieces);

    // Normal chess layout
    static vector<vector<Chess_Piece*>> Default_Board();

    void Add_Piece_To_Board(Chess_Piece* piece, int row, int col);
    Chess_Piece* Get_Piece(int row, int col);
    void GetLocation(Chess_Piece* piece, int& row, int& col);
    int GetBoardScore();
    vector<Move> AllPossibleMoves();

    vector<Move> ValidMoves(int row, int col);
    bool ValidMove(int row, int col);
    void Move_Piece(Move mv);


    int board_size;
    int s_size;
    vector<vector<Chess_Piece*>> chess_pieces;
    Chess_Color currentPlayer = Chess_Color::White;

};

static bool IsOccupiedBlock(int row, int col, Board& board)
{
    // Move 1 forward if empty
    if(board.Get_Piece(row, col) == nullptr)
        return false;
    else if (row < 8 && row > 0 && board.Get_Piece(row, col)->chess_type == Chess_Type::EMPTY)
        return false;
    else
        return true;
}

static bool PositionOutOfBounds(int row, int col)
{
    // Move 1 forward if empty
    if(row < 8 && row >= 0 && col < 8 && col >= 0)
        return false;
    else
        return true;
}

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
            if(board.Get_Piece(row,x)->color == color)
                break;

            possibleMoves.push_back(Move(row, col, row, x));

            if(board.Get_Piece(row,x)->chess_type != Chess_Type::EMPTY)
                break;
        }

        // Negative horisontal move
        for(int x = col-1; x >= 0; x--)
        {
            if(board.Get_Piece(row,x)->color == color)
                break;
            possibleMoves.push_back(Move(row, col, row, x));
            if(board.Get_Piece(row,x)->chess_type != Chess_Type::EMPTY)
                break;
        }

        // Positive vertical move
        for(int x = row+1; x < 8; x++)
        {
            if(board.Get_Piece(x,col)->color == color)
                break;
            possibleMoves.push_back(Move(row, col, x, col));
            if(board.Get_Piece(x,col)->chess_type != Chess_Type::EMPTY)
                break;
        }

        // Negative vertical move
        for(int x = row-1; x >= 0; x--)
        {
            if(board.Get_Piece(x, col)->color == color)
                break;
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

        int row, col;
        board.GetLocation(this, row, col);

        vector<Move> possibleMoves = {};

        // Positive horisontal move
        for(int x = col+1; x < 8; x++)
        {
            if(board.Get_Piece(row,x)->color == color)
                break;

            possibleMoves.push_back(Move(row, col, row, x));

            if(board.Get_Piece(row,x)->chess_type != Chess_Type::EMPTY)
                break;
        }

        // Negative horisontal move
        for(int x = col-1; x >= 0; x--)
        {
            if(board.Get_Piece(row,x)->color == color)
                break;
            possibleMoves.push_back(Move(row, col, row, x));
            if(board.Get_Piece(row,x)->chess_type != Chess_Type::EMPTY)
                break;
        }

        // Positive vertical move
        for(int x = row+1; x < 8; x++)
        {
            if(board.Get_Piece(x,col)->color == color)
                break;
            possibleMoves.push_back(Move(row, col, x, col));
            if(board.Get_Piece(x,col)->chess_type != Chess_Type::EMPTY)
                break;
        }

        // Negative vertical move
        for(int x = row-1; x >= 0; x--)
        {
            if(board.Get_Piece(x, col)->color == color)
                break;
            possibleMoves.push_back(Move(row, col, x, col));
            if(board.Get_Piece(x,col)->chess_type != Chess_Type::EMPTY)
                break;
        }

        // NW move
        for(int x = 1; x < 8; x++)
        {
            if(PositionOutOfBounds(row-x, col-x))
                break;

            if(!IsOccupiedBlock(row-x, col-x, board) )
            {
                possibleMoves.push_back(Move(row, col, row-x, col-x));
            }
            else if(board.Get_Piece(row-x, col-x)->color != color){
                possibleMoves.push_back(Move(row, col, row-x, col-x));
                break;
            }
            else
                break;
        }

        // NE move
        for(int x = 1; x < 8; x++)
        {
            if(PositionOutOfBounds(row-x, col+x))
                break;

            if(!IsOccupiedBlock(row-x, col+x, board) )
            {
                possibleMoves.push_back(Move(row, col, row-x, col+x));
            }
            else if(board.Get_Piece(row-x, col+x)->color != color){
                possibleMoves.push_back(Move(row, col, row-x, col+x));
                break;
            }
            else
                break;
        }

        // SW move
        for(int x = 1; x < 8; x++)
        {
            if(PositionOutOfBounds(row+x, col-x))
                break;

            if(!IsOccupiedBlock(row+x, col-x, board) )
            {
                possibleMoves.push_back(Move(row, col, row+x, col-x));
            }
            else if(board.Get_Piece(row+x, col-x)->color != color){
                possibleMoves.push_back(Move(row, col, row+x, col-x));
                break;
            }
            else
                break;
        }

        // SE move
        for(int x = 1; x < 8; x++)
        {
            if(PositionOutOfBounds(row+x, col+x))
                break;

            if(!IsOccupiedBlock(row+x, col+x, board) )
            {
                possibleMoves.push_back(Move(row, col, row+x, col+x));
            }
            else if(board.Get_Piece(row+x, col+x)->color != color){
                possibleMoves.push_back(Move(row, col, row+x, col+x));
                break;
            }
            else
                break;
        }

        return possibleMoves;
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
        int row, col;
        board.GetLocation(this, row, col);

        vector<Move> possibleMoves = {};

        // NW move
        for(int x = 1; x < 8; x++)
        {
            if(PositionOutOfBounds(row-x, col-x))
                break;

            if(!IsOccupiedBlock(row-x, col-x, board) )
            {
                possibleMoves.push_back(Move(row, col, row-x, col-x));
            }
            else if(board.Get_Piece(row-x, col-x)->color != color){
                possibleMoves.push_back(Move(row, col, row-x, col-x));
                break;
            }
            else
                break;
        }

        // NE move
        for(int x = 1; x < 8; x++)
        {
            if(PositionOutOfBounds(row-x, col+x))
                break;

            if(!IsOccupiedBlock(row-x, col+x, board) )
            {
                possibleMoves.push_back(Move(row, col, row-x, col+x));
            }
            else if(board.Get_Piece(row-x, col+x)->color != color){
                possibleMoves.push_back(Move(row, col, row-x, col+x));
                break;
            }
            else
                break;
        }

        // SW move
        for(int x = 1; x < 8; x++)
        {
            if(PositionOutOfBounds(row+x, col-x))
                break;

            if(!IsOccupiedBlock(row+x, col-x, board) )
            {
                possibleMoves.push_back(Move(row, col, row+x, col-x));
            }
            else if(board.Get_Piece(row+x, col-x)->color != color){
                possibleMoves.push_back(Move(row, col, row+x, col-x));
                break;
            }
            else
                break;
        }

        // SE move
        for(int x = 1; x < 8; x++)
        {
            if(PositionOutOfBounds(row+x, col+x))
                break;

            if(!IsOccupiedBlock(row+x, col+x, board) )
            {
                possibleMoves.push_back(Move(row, col, row+x, col+x));
            }
            else if(board.Get_Piece(row+x, col+x)->color != color){
                possibleMoves.push_back(Move(row, col, row+x, col+x));
                break;
            }
            else
                break;
        }

        return possibleMoves;
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
        // Get coordinates
        int row, col;
        board.GetLocation(this, row, col);

        vector<Move> possibleMoves = {};

        // Top right move
        possibleMoves.push_back(Move(row, col, row+2, col+1));
        // Top left move
        possibleMoves.push_back(Move(row, col, row+2, col-1));

        // Right top move
        possibleMoves.push_back(Move(row, col, row+1, col+2));
        // Right bottom move
        possibleMoves.push_back(Move(row, col, row-1, col+2));

        // Bottom right move
        possibleMoves.push_back(Move(row, col, row-2, col+1));
        // Bottom left move
        possibleMoves.push_back(Move(row, col, row-2, col-1));

        // Left top move
        possibleMoves.push_back(Move(row, col, row+1, col-2));
        // Left bottom move
        possibleMoves.push_back(Move(row, col, row-1, col-2));


        // remove moves out of bounds and targeting pieces of similar color
        vector<Move>::iterator it = possibleMoves.begin();

        while(it != possibleMoves.end()) {
            if(PositionOutOfBounds(it->r2, it->c2) || board.Get_Piece(it->r2, it->c2)->color == color)
                it = possibleMoves.erase(it);
            else
                ++it;
        }

        return possibleMoves;
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
        // Get coordinates
        int row, col;
        board.GetLocation(this, row, col);

        vector<Move> possibleMoves = {};

        int dir = (color==Chess_Color::White)? -1: 1;


        // Move 1 forward if empty
        if(!IsOccupiedBlock(row+dir, col, board) && !PositionOutOfBounds(row, col))
            possibleMoves.push_back(Move(row,col,row+dir,col));

        // Initial move is 2 forward
        if(initialMove && !IsOccupiedBlock(row+dir*2, col, board) && !PositionOutOfBounds(row,col))
            possibleMoves.push_back(Move(row,col,row+dir*2,col));

        // Attack right diagonal
        if(!PositionOutOfBounds(row, col) && IsOccupiedBlock(row+dir, col+1, board) && board.Get_Piece(row+dir, col+1)->color != color)
            possibleMoves.push_back(Move(row,col,row+dir,col+1));

        // Attack left diagonal
        if(!PositionOutOfBounds(row, col) && IsOccupiedBlock(row+dir, col-1, board) && board.Get_Piece(row+dir, col-1)->color != color)
            possibleMoves.push_back(Move(row,col,row+dir,col-1));

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
        color = Chess_Color::Empty;
    }

    virtual vector<Move> ValidMoves(Board &board)
    {
        return {};
    }
};



#endif // CHESS_PIECES_H
