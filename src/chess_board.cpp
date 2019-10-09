#include "chess_pieces.h"


Board::Board(vector<vector<Chess_Piece*>> pieces)
{
    chess_pieces = pieces;
}

int Board::GetBoardScore()
{
    float sum = 0;
    for(auto& row: chess_pieces)
    {
        for(auto& piece: row)
        {
            sum += piece->piece_Value;
        }
    }
    cout << "Board Evaluation: " << sum << endl;
    return sum;
}

vector<Move> Board::AllPossibleMoves()
{
    return {};
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

Chess_Piece* Board::Get_Piece(int row, int col)
{
    if(row < chess_pieces.size() && col < chess_pieces.size() && chess_pieces[row][col])
        return chess_pieces[row][col];
    else
        return new class Empty();
}


void Board::Move_Piece(Move mv)
{
    chess_pieces[mv.r1][mv.c1]->RegisterMove(mv);

    delete chess_pieces[mv.r2][mv.c2];
    chess_pieces[mv.r2][mv.c2] = chess_pieces[mv.r1][mv.c1];
    chess_pieces[mv.r1][mv.c1] = nullptr;

    if(currentPlayer == Chess_Color::White)
        currentPlayer = Chess_Color::Black;
    else
        currentPlayer = Chess_Color::White;
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

vector<Move> Board::ValidMoves(int row, int col)
{

}

bool Board::ValidMove(int row, int col)
{

}
