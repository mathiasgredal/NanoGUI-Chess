#include "computer.h"

#include <chrono>
#include <ctime>
#include <vector>

unsigned int randr(unsigned int min, unsigned int max)
{
    double scaled = (double)rand() / RAND_MAX;

    return (max - min + 1) * scaled + min;
}

Move Computer::GetRandomMove(Board& board)
{
    std::vector<Chess_Piece*> availablePieces = board.GetPiecesOfColor(board.currentPlayer);
    std::vector<Move> availableMoves = {};

    for (auto& piece : availablePieces) {
        int row, col;
        board.GetLocation(piece, row, col);
        auto validMoves = board.ValidMoves(row, col);
        availableMoves.insert(availableMoves.end(), validMoves.begin(),
            validMoves.end());
    }

    size_t move_index = randr(0, availableMoves.size() - 1);

    return availableMoves.at(move_index);
}

Move Computer::GetSimpleMove(Board& board)
{
    std::vector<Chess_Piece*> availablePieces = board.GetPiecesOfColor(board.currentPlayer);
    std::vector<Move> availableMoves = {};
    std::vector<int> scores = {};

    for (auto& piece : availablePieces) {
        // cout << piece->iconLetter << endl;
        int row, col;
        board.GetLocation(piece, row, col);
        auto validMoves = board.ValidMoves(row, col);
        availableMoves.insert(availableMoves.end(), validMoves.begin(),
            validMoves.end());
    }

    for (auto& availMove : availableMoves) {
        Board copyBoard = board;
        copyBoard.Move_Piece(availMove);
        scores.push_back(copyBoard.GetBoardScore());
    }

    size_t moveIndex = 0;
    int highestScore = 0;

    for (std::size_t i = 0; i < scores.size(); i++) {
        if (scores[i] > highestScore) {
            highestScore = scores[i];
            moveIndex = i;
        }
    }

    return availableMoves.at(moveIndex);
}

vector<Move> allPossibleMoves(Board& board)
{
    std::vector<Chess_Piece*> availablePieces = board.GetPiecesOfColor(board.currentPlayer);
    std::vector<Move> availableMoves = {};
    for (auto& piece : availablePieces) {
        int row, col;
        board.GetLocation(piece, row, col);
        auto validMoves = board.ValidMoves(row, col);
        availableMoves.insert(availableMoves.end(), validMoves.begin(),
            validMoves.end());
    }

    return availableMoves;
}

int evaluateBoard(Board& board)
{
    int score = 0;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Chess_Piece* piece = board.chess_pieces[r][c];
            if (piece != nullptr) {
                if (piece->color == Chess_Color::White)
                    score += piece->piece_Value;
                else if (piece->color == Chess_Color::Black)
                    score -= piece->piece_Value;
            }
        }
    }

    return score;
}

int positionCount = 0;

int max(int val1, int val2)
{
    if (val1 >= val2)
        return val1;
    else
        return val2;
}

int min(int val1, int val2)
{
    if (val1 <= val2)
        return val1;
    else
        return val2;
}

int minimax(int depth, Board& board, bool color, int alpha, int beta)
{
    // Another check rather than depth, is to check if the game has
    // ended(stalemate, checkmate)
    if (depth == 0)
        return evaluateBoard(board) * color;

    int value = -99999999;

    auto newGameMoves = allPossibleMoves(board);

    for (Move gameMove : allPossibleMoves(board)) {
        positionCount++;

        Board newboard = board;
        newboard.Move_Piece(gameMove);
        if (board.IsCheckMate())
            value = 100000000;
        else
            value = max(value, -minimax(depth - 1, newboard, -color, -beta, -alpha));

        alpha = max(alpha, value);
        if (alpha >= beta)
            break;
    }

    return value;
}

Move Computer::GetMiniMaxMove(Board& board)
{
    positionCount = 0;
    std::clock_t start;
    double duration;

    start = std::clock();

    const int depth = 3;

    // Whitepieces are positive, and black negative. Hence white is the maximising
    // player
    const bool color = (board.currentPlayer == Chess_Color::White) ? 1 : -1;

    cout << evaluateBoard(board) << endl;

    int bestMoveScore = INT_MAX;
    Move bestMove = Move(0, 0, 0, 0);

    std::vector<Move> availableMoves = allPossibleMoves(board);

    for (auto& availMove : availableMoves) {
        Board copyBoard = board;
        copyBoard.Move_Piece(availMove);

        int value = minimax(depth, copyBoard, -1, -10000000, 10000000);
        cout << value << endl;

        if (value <= bestMoveScore) {
            bestMoveScore = value;
            bestMove = availMove;
        }
    }

    bestMove.Print_Move();

    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

    cout << "Evaluated positions: " << positionCount << endl;
    cout << "Positions pr. sek: " << positionCount / duration << endl;

    return bestMove;
}
