#include "computer.h"

#include <chrono>
#include <cmath>
#include <ctime>
#include <future>
#include <map>
#include <thread>
#include <vector>

unsigned int randr(unsigned int min, unsigned int max)
{
    double scaled = (double)rand() / RAND_MAX;

    return (max - min + 1) * scaled + min;
}

Move Computer::GetRandomMove(Board& board)
{
    std::vector<Chess_Piece> availablePieces = board.GetPiecesOfColor(board.currentPlayer);
    std::vector<Move> availableMoves = {};

    for (auto& piece : availablePieces) {
        auto validMoves = board.ValidMoves(piece.row, piece.col);
        availableMoves.insert(availableMoves.end(), validMoves.begin(),
            validMoves.end());
    }

    size_t move_index = randr(0, availableMoves.size() - 1);

    return availableMoves.at(move_index);
}

Move Computer::GetSimpleMove(Board& board)
{
    std::vector<Chess_Piece> availablePieces = board.GetPiecesOfColor(board.currentPlayer);
    std::vector<Move> availableMoves = {};
    std::vector<int> scores = {};

    for (auto& piece : availablePieces) {
        // cout << piece->iconLetter << endl;
        auto validMoves = board.ValidMoves(piece.row, piece.col);
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
    std::vector<Move> availableMoves = {};
    for (auto& piece : board.chess_pieces) {
        if (piece.color == board.currentPlayer) {
            const auto validMoves = board.ValidMoves(piece.row, piece.col);
            availableMoves.insert(availableMoves.end(), validMoves.begin(),
                validMoves.end());
        }
    }

    return availableMoves;
}

int evaluateBoard(Board& board)
{
    int score = 0;

    for (int i = 0; i < board.chess_pieces.size(); i++) {
        Chess_Piece piece = board.chess_pieces[i];
        if (piece.color == Chess_Color::White)
            score += ChessPieceValue(piece.chessType);
        else if (piece.color == Chess_Color::Black)
            score -= ChessPieceValue(piece.chessType);
    }

    return score;
}

static std::atomic<int> positionCount = 0;

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
        return evaluateBoard(board) * (color); // when using depths of even number remove sign
    else if (board.IsCheckMate())
        return 1000000 * color;

    int value = -999999;

    auto newGameMoves = allPossibleMoves(board);

    for (Move& gameMove : allPossibleMoves(board)) {
        positionCount++;

        Board newboard = board;
        newboard.Move_Piece(gameMove);
        value = max(value, -minimax(depth - 1, newboard, -color, -beta, -alpha));

        alpha = max(alpha, value);
        if (alpha >= beta)
            break;
    }

    return value;
}

const int threads_to_use = 4;

std::vector<Move> sortMoves(std::vector<Move> moves, Board& board)
{
    vector<pair<Move, int>> MoveScores(moves.size());

    for (int i = 0; i < moves.size(); i++) {
        MoveScores[i].first = moves[i];
        MoveScores[i].second = ChessPieceValue(board.Get_Piece(moves[i].r2, moves[i].c2).chessType);
    }

    std::sort(MoveScores.begin(), MoveScores.end(), [](auto& left, auto& right) {
        return left.second < right.second;
    });
}

std::vector<pair<Move, int>> GetThreadMoves(vector<Move> someMoves, Board someBoard, int depth)
{
    std::vector<std::pair<Move, int>> threadResults = {};

    for (auto& threadMove : someMoves) {
        Board copyBoard = someBoard;

        copyBoard.Move_Piece(threadMove);

        int value = minimax(depth, copyBoard, -1, -10000000, 10000000);
        std::cout << "Score " << value << " - ";
        threadMove.Print_Move();

        threadResults.emplace_back(threadMove, value);
    }

    return threadResults;
}

Move Computer::GetMiniMaxMove(Board& board)
{

    positionCount = 0;
    std::clock_t start;
    double duration;

    start = std::clock();

    // For some reason this has to be even numbers
    const int depth = 4;

    // Whitepieces are positive, and black negative. Hence white is the maximising
    // player
    const bool color = (board.currentPlayer == Chess_Color::White) ? 1 : -1;

    int bestMoveScore = INT_MAX;
    Move bestMove = Move(0, 0, 0, 0);

    std::vector<std::pair<Move, int>> minimaxResults = {};

    std::vector<Move> availableMoves = allPossibleMoves(board);
    //std::vector<Move> sortedMoves = sort(availableMoves.begin(), availableMoves.end(), [](const Move& a, const Move& b){return true;});

    std::cout << "Total moves: " << availableMoves.size() + 1 << std::endl;

    array<vector<Move>, threads_to_use> movesPrThread;

    while (availableMoves.size() != 0) {
        for (vector<Move>& moveInThread : movesPrThread) {
            moveInThread.push_back(availableMoves.back());
            availableMoves.pop_back();

            if (availableMoves.size() == 0)
                break;
        }
    }

    std::vector<future<vector<pair<Move, int>>>> t;

    for (int i = 0; i < threads_to_use; i++) {
        t.emplace_back(async(GetThreadMoves, movesPrThread[i], board, depth));
    }

    for (auto& moveThread : t) {
        std::vector<pair<Move, int>> threadMoves = moveThread.get();

        minimaxResults.insert(minimaxResults.end(), threadMoves.begin(), threadMoves.end());
    }
    /*
    for (int i = 0; i < availableMoves.size(); i++) {
        Board copyBoard = board;
        copyBoard.Move_Piece(availableMoves[i]);

        int value = minimax(depth, copyBoard, -1, -10000000, 10000000);

        cout << value;
        availableMoves[i].Print_Move();

        minimaxResults.emplace_back(availableMoves[i], value);
    }*/

    for (auto& result : minimaxResults) {
        //std::cout << "Score: " << result.second << " - ";
        result.first.Print_Move();
        if (result.second <= bestMoveScore) {
            bestMoveScore = result.second;
            bestMove = result.first;
        }
    }

    std::cout << "Best ";
    bestMove.Print_Move();

    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

    cout << "Evaluated positions: " << positionCount << endl;
    cout << "Positions pr. sek: " << positionCount / duration << endl;

    return bestMove;
}
