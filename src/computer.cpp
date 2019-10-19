#include "computer.h"
#include <vector>

Move Computer::GetRandomMove(Board& board)
{
    std::vector<Chess_Piece*> availablePieces = board.GetPiecesOfColor(board.currentPlayer);
    std::vector<Move> availableMoves = {};

    for (auto& piece : availablePieces) {
        int row, col;
        board.GetLocation(piece, row, col);
        auto validMoves = board.ValidMoves(row, col);
        availableMoves.insert(availableMoves.end(), validMoves.begin(), validMoves.end());
    }

    size_t move_index = rand()%(availableMoves.size()-1);

    return availableMoves.at(move_index);
}

Move Computer::GetSimpleMove(Board& board)
{
    std::vector<Chess_Piece*> availablePieces = board.GetPiecesOfColor(board.currentPlayer);
    std::vector<Move> availableMoves = {};
    std::vector<int> scores = {};

    for (auto& piece : availablePieces) {
        cout << piece->iconLetter << endl;
        int row, col;
        board.GetLocation(piece, row, col);
        auto validMoves = board.ValidMoves(row, col);
        availableMoves.insert(availableMoves.end(), validMoves.begin(), validMoves.end());
    }

    for (auto& availMove : availableMoves) {
        Board copyBoard = board;
        copyBoard.Move_Piece(availMove);
        scores.push_back(copyBoard.GetBoardScore());
    }



    size_t moveIndex = 0;
    int highestScore = 0;


    for (std::size_t i = 0; i < scores.size(); i++) {

        if(scores[i] > highestScore)
        {
            highestScore = scores[i];
            moveIndex = i;
        }
    }



    return availableMoves.at(moveIndex);
}
