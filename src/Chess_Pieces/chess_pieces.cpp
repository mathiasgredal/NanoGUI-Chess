#include <iostream>
#include <vector>
#include "chess_pieces.h"

using namespace std;


Board::Board(vector<vector<Chess_Piece*>> pieces)
{
    chess_pieces = pieces;
}

vector<vector<Chess_Piece*>> Board::Default_Board()
{
    vector<vector<Chess_Piece*>> default_pieces(8, vector<Chess_Piece*> (8));

    // Black pieces
    default_pieces[0][0] = new class Rook(2,2,Chess_Color::Black);
    default_pieces[0][1] = new class Knight(2,2,Chess_Color::Black);
    default_pieces[0][2] = new class Bishop(2,2,Chess_Color::Black);
    default_pieces[0][3] = new class Queen(2,2,Chess_Color::Black);
    default_pieces[0][4] = new class King(2,2,Chess_Color::Black);
    default_pieces[0][5] = new class Bishop(2,2,Chess_Color::Black);
    default_pieces[0][6] = new class Knight(2,2,Chess_Color::Black);
    default_pieces[0][7] = new class Rook(2,2,Chess_Color::Black);


    for(auto &piece : default_pieces[1])
        piece = new class Pawn(2,2,Chess_Color::Black);

    default_pieces[7][0] = new class Rook(2,2,Chess_Color::White);
    default_pieces[7][1] = new class Knight(2,2,Chess_Color::White);
    default_pieces[7][2] = new class Bishop(2,2,Chess_Color::White);
    default_pieces[7][3] = new class Queen(2,2,Chess_Color::White);
    default_pieces[7][4] = new class King(2,2,Chess_Color::White);
    default_pieces[7][5] = new class Bishop(2,2,Chess_Color::White);
    default_pieces[7][6] = new class Knight(2,2,Chess_Color::White);
    default_pieces[7][7] = new class Rook(2,2,Chess_Color::White);

    for(auto &piece : default_pieces[6])
        piece = new class Pawn(2,2,Chess_Color::White);

    return default_pieces;
}

void Board::Add_Piece_To_Board(Chess_Piece* piece, int row, int col)
{
    chess_pieces[row][col] = piece;
}

Chess_Piece* Board::Get_Piece(int row, int col){
    if(chess_pieces[row][col])
        return chess_pieces[row][col];
    else
        return new Empty();
}


void Board::Move_Piece(Move mv)
{
    delete chess_pieces[mv.r2][mv.c2];
    chess_pieces[mv.r2][mv.c2] = chess_pieces[mv.r1][mv.c1];
    chess_pieces[mv.r1][mv.c1] = nullptr;
}

void Board::GetLocation(Chess_Piece* piece, int& row, int& col)
{
    for (int r = 0; r < chess_pieces.size(); r++) {
        for (int c = 0; c < chess_pieces[r].size(); c++) {
            if(chess_pieces[r][c] == piece)
            {
                row = r;
                col = c;
                return;
            }
        }
    }
}

bool Chess_Piece::ValidMove(Move usermove, Board &board)
{
    // TODO: Add code here to check for basic errors in move, that might not be checked in function

    vector<Move> validmoves = ValidMoves(board);

    cout << "Our move: ";
    usermove.Print_Move();

    /*
     * TODO: Use STL functions
     *
        if(std::find(validmoves.begin(), validmoves.end(), mv) != validmoves.end()) {
            return true;
        } else {
            return false;
    }*/

    for(auto& validmove: validmoves){
        validmove.Print_Move();
        if(validmove == usermove)
            return true;
    }

    return false;
}

Chess_Piece::~Chess_Piece(){

}


Move::Move(int row1,int col1, int row2, int col2)
{
    r1 = row1;
    c1 = col1;
    r2 = row2;
    c2 = col2;
}

void Move::Print_Move(){
    cout << "Move from " << char('A'+c1) << 8-r1;
    cout << " to " << char('A'+c2) << 8-r2 << endl;
}

// Metode til at konvertere til vektor
vector<int> Move::to_vector()
{
    return vector<int>({r1,c1,r2,c2});
}








