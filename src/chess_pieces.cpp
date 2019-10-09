#include "chess_pieces.h"


using namespace std;

vector<Move> Chess_Piece::ValidMoves(Board &board)
{

}

bool Chess_Piece::ValidMove(Move usermove, Board &board)
{
    // TODO: Add code here to check for basic errors in move, that might not be checked in function

    // Check if invalid turn
    if(board.Get_Piece(usermove.r1, usermove.c1)->color != board.currentPlayer)
        return false;

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

    for(Move& validmove: validmoves){
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








