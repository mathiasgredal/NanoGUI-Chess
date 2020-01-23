#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include <vector>

#include "chess_pieces.h"

using namespace std;

class MoveGenerator {
public:
    static vector<Move> PawnMoves(Board& board, Chess_Piece& piece);
    static vector<Move> KnightMoves(Board& board, Chess_Piece& piece);
    static vector<Move> BishopMoves(Board& board, Chess_Piece& piece);
    static vector<Move> RookMoves(Board& board, Chess_Piece& piece);
    static vector<Move> QueenMoves(Board& board, Chess_Piece& piece);
    static vector<Move> KingMoves(Board& board, Chess_Piece& piece);

private:
    MoveGenerator() {}
};

#endif // MOVEGENERATOR_H
