#include "chess_pieces.h"


using namespace std;

vector<Move> Chess_Piece::ValidMoves(Board &board)
{

}

Chess_Piece::~Chess_Piece()= default;


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








