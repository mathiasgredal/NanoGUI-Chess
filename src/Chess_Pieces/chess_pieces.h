#ifndef CHESS_PIECES_H
#define CHESS_PIECES_H

#include <vector>

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
};


class Chess_Piece
{
    public:
        Chess_Type chess_type;
        Chess_Color color;
        const char* iconLetter;
        int piece_Value;
        int row;
        int col;

        virtual vector<Move> validMoves(Board &board) = 0;

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
        row=_row;
        col=_col;
    }

    virtual vector<Move> validMoves(Board &board)
    {
        return {Move(0,0,5,5)};
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
        row=_row;
        col=_col;
    }

    virtual vector<Move> validMoves(Board &board)
    {
        /*
         *    bool collision = false;
            if(mv.r1 == mv.r2)
            {

                if(mv.c2>mv.c1)
                {
                    cout << "poshori\n";
                    //We have a poshorizontal move
                    for(int x = mv.c1; x <= mv.c2; x++)
                    {
                        if(board[mv.r1][x] != 0)
                        {
                            collision = true;
                            return !collision;
                        }
                    }
                }
                if(mv.c2<mv.c1)
                {
                    cout << "neghori\n";
                    //We have a neghorizontal move
                    for(int x = mv.c1; x <= mv.c2; x--)
                    {
                        if(board[mv.r1][x] != 0)
                        {
                            collision = true;
                            return !collision;
                        }
                    }
                }

            }
            if(mv.c1 == mv.c2)
            {
                if(mv.r2>mv.r1)
                {
                    cout << "posveri\n";
                    //We have a posvertical move
                    for(int y = mv.r1; y <= mv.r2; y++)
                    {
                        if(board[y][mv.c1] != 0)
                        {
                            collision = true;
                            return !collision;
                        }
                    }
                }
                if(mv.r2<mv.r1)
                {
                    cout << "negveri\n";
                    //We have a negvertical move
                    for(int y = mv.r1; y <= mv.r2; y--)
                    {
                        if(board[y][mv.c1] != 0)
                        {
                            collision = true;
                            return !collision;
                        }
                    }
                }

            }
            return(mv.r1 == mv.r2 || mv.c1 == mv.c2);
        */
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
        row=_row;
        col=_col;
    }

    virtual vector<Move> validMoves(Board &board)
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
        row=_row;
        col=_col;
    }

    virtual vector<Move> validMoves(Board &board)
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
        row=_row;
        col=_col;
    }

    virtual vector<Move> validMoves(Board &board)
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
        row=_row;
        col=_col;
    }

    virtual vector<Move> validMoves(Board &board)
    {
        return {Move(0,0,5,5)};
    }
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

    virtual vector<Move> validMoves(Board &board)
    {
        return {};
    }
};



#endif // CHESS_PIECES_H
