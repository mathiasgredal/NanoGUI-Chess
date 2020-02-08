#include "movegenerator.h"

inline static bool IsOccupiedBlock(int row, int col, Board& board)
{
    return !(row < 8 && row >= 0 && board.Get_Piece(row, col).chessType == Chess_Type::EMPTY);
}

inline static bool PositionOutOfBounds(int row, int col)
{
    return !(row < 8 && row >= 0 && col < 8 && col >= 0);
}

vector<Move> MoveGenerator::PawnMoves(Board& board, Chess_Piece& piece)
{
    vector<Move> possibleMoves = {};

    int dir = (piece.color == Chess_Color::White) ? -1 : 1;

    // Move 1 forward if empty
    if (!PositionOutOfBounds(piece.row + dir, piece.col) && !IsOccupiedBlock(piece.row + dir, piece.col, board))
        possibleMoves.emplace_back(piece.row, piece.col, piece.row + dir, piece.col);

    // Initial move is 2 forward
    if (piece.initialMove && !PositionOutOfBounds(piece.row + dir * 2, piece.col) && !IsOccupiedBlock(piece.row + dir * 2, piece.col, board))
        possibleMoves.emplace_back(piece.row, piece.col, piece.row + dir * 2, piece.col);

    // Attack right diagonal
    if (!PositionOutOfBounds(piece.row + dir, piece.col + 1) && IsOccupiedBlock(piece.row + dir, piece.col + 1, board) && board.Get_Piece(piece.row + dir, piece.col + 1).color != piece.color)
        possibleMoves.emplace_back(piece.row, piece.col, piece.row + dir, piece.col + 1);

    // Attack left diagonal
    if (!PositionOutOfBounds(piece.row + dir, piece.col - 1) && IsOccupiedBlock(piece.row + dir, piece.col - 1, board) && board.Get_Piece(piece.row + dir, piece.col - 1).color != piece.color)
        possibleMoves.emplace_back(piece.row, piece.col, piece.row + dir, piece.col - 1);

    return possibleMoves;
}

vector<Move> MoveGenerator::KnightMoves(Board& board, Chess_Piece& piece)
{
    vector<Move> possibleMoves = {
        Move(piece.row, piece.col, piece.row + 2, piece.col + 1), // Top right move
        Move(piece.row, piece.col, piece.row + 2, piece.col - 1), // Top left move
        Move(piece.row, piece.col, piece.row + 1, piece.col + 2), // Right top move
        Move(piece.row, piece.col, piece.row - 1, piece.col + 2), // Right bottom move
        Move(piece.row, piece.col, piece.row - 2, piece.col + 1), // Bottom right move
        Move(piece.row, piece.col, piece.row - 2, piece.col - 1), // Bottom left move
        Move(piece.row, piece.col, piece.row + 1, piece.col - 2), // Left top move
        Move(piece.row, piece.col, piece.row - 1, piece.col - 2) // Left bottom move
    };

    // remove moves out of bounds and targeting pieces of similar color
    auto it = possibleMoves.begin();

    while (it != possibleMoves.end()) {
        if (PositionOutOfBounds(it->r2, it->c2) || board.Get_Piece(it->r2, it->c2).color == piece.color)
            it = possibleMoves.erase(it);
        else
            ++it;
    }

    return possibleMoves;
}

vector<Move> MoveGenerator::BishopMoves(Board& board, Chess_Piece& piece)
{
    vector<Move> possibleMoves = {};

    // NW move
    for (int x = 1; x < 8; x++) {
        if (PositionOutOfBounds(piece.row - x, piece.col - x))
            break;

        if (!IsOccupiedBlock(piece.row - x, piece.col - x, board))
            possibleMoves.emplace_back(piece.row, piece.col, piece.row - x, piece.col - x);
        else if (board.Get_Piece(piece.row - x, piece.col - x).color != piece.color) {
            possibleMoves.emplace_back(piece.row, piece.col, piece.row - x, piece.col - x);
            break;
        } else
            break;
    }

    // NE move
    for (int x = 1; x < 8; x++) {
        if (PositionOutOfBounds(piece.row - x, piece.col + x))
            break;

        if (!IsOccupiedBlock(piece.row - x, piece.col + x, board))
            possibleMoves.emplace_back(piece.row, piece.col, piece.row - x, piece.col + x);
        else if (board.Get_Piece(piece.row - x, piece.col + x).color != piece.color) {
            possibleMoves.emplace_back(piece.row, piece.col, piece.row - x, piece.col + x);
            break;
        } else
            break;
    }

    // SW move
    for (int x = 1; x < 8; x++) {
        if (PositionOutOfBounds(piece.row + x, piece.col - x))
            break;

        if (!IsOccupiedBlock(piece.row + x, piece.col - x, board))
            possibleMoves.emplace_back(piece.row, piece.col, piece.row + x, piece.col - x);
        else if (board.Get_Piece(piece.row + x, piece.col - x).color != piece.color) {
            possibleMoves.emplace_back(piece.row, piece.col, piece.row + x, piece.col - x);
            break;
        } else
            break;
    }

    // SE move
    for (int x = 1; x < 8; x++) {
        if (PositionOutOfBounds(piece.row + x, piece.col + x))
            break;

        if (!IsOccupiedBlock(piece.row + x, piece.col + x, board)) {
            possibleMoves.emplace_back(piece.row, piece.col, piece.row + x, piece.col + x);
        } else if (board.Get_Piece(piece.row + x, piece.col + x).color != piece.color) {
            possibleMoves.emplace_back(piece.row, piece.col, piece.row + x, piece.col + x);
            break;
        } else
            break;
    }

    return possibleMoves;
}

vector<Move> MoveGenerator::RookMoves(Board& board, Chess_Piece& piece)
{
    vector<Move> possibleMoves = {};

    // Positive horisontal move
    for (int x = piece.col + 1; x < 8; ++x) {
        if (board.Get_Piece(piece.row, x).color == piece.color)
            break;

        possibleMoves.emplace_back(piece.row, piece.col, piece.row, x);

        if (board.Get_Piece(piece.row, x).chessType != Chess_Type::EMPTY)
            break;
    }

    // Negative horisontal move
    for (int x = piece.col - 1; x >= 0; x--) {
        if (board.Get_Piece(piece.row, x).color == piece.color)
            break;
        possibleMoves.emplace_back(piece.row, piece.col, piece.row, x);
        if (board.Get_Piece(piece.row, x).chessType != Chess_Type::EMPTY)
            break;
    }

    // Positive vertical move
    for (int x = piece.row + 1; x < 8; x++) {
        if (board.Get_Piece(x, piece.col).color == piece.color)
            break;
        possibleMoves.emplace_back(piece.row, piece.col, x, piece.col);
        if (board.Get_Piece(x, piece.col).chessType != Chess_Type::EMPTY)
            break;
    }

    // Negative vertical move
    for (int x = piece.row - 1; x >= 0; x--) {
        if (board.Get_Piece(x, piece.col).color == piece.color)
            break;
        possibleMoves.emplace_back(piece.row, piece.col, x, piece.col);
        if (board.Get_Piece(x, piece.col).chessType != Chess_Type::EMPTY)
            break;
    }

    return possibleMoves;
}

vector<Move> MoveGenerator::QueenMoves(Board& board, Chess_Piece& piece)
{
    vector<Move> possibleMoves = {};

    // Positive horisontal move
    for (int x = piece.col + 1; x < 8; x++) {
        if (board.Get_Piece(piece.row, x).color == piece.color)
            break;

        possibleMoves.emplace_back(piece.row, piece.col, piece.row, x);

        if (board.Get_Piece(piece.row, x).chessType != Chess_Type::EMPTY)
            break;
    }

    // Negative horisontal move
    for (int x = piece.col - 1; x >= 0; x--) {
        if (board.Get_Piece(piece.row, x).color == piece.color)
            break;
        possibleMoves.emplace_back(piece.row, piece.col, piece.row, x);
        if (board.Get_Piece(piece.row, x).chessType != Chess_Type::EMPTY)
            break;
    }

    // Positive vertical move
    for (int x = piece.row + 1; x < 8; x++) {
        if (board.Get_Piece(x, piece.col).color == piece.color)
            break;
        possibleMoves.emplace_back(piece.row, piece.col, x, piece.col);
        if (board.Get_Piece(x, piece.col).chessType != Chess_Type::EMPTY)
            break;
    }

    // Negative vertical move
    for (int x = piece.row - 1; x >= 0; x--) {
        if (board.Get_Piece(x, piece.col).color == piece.color)
            break;
        possibleMoves.emplace_back(piece.row, piece.col, x, piece.col);
        if (board.Get_Piece(x, piece.col).chessType != Chess_Type::EMPTY)
            break;
    }

    // NW move
    for (int x = 1; x < 8; x++) {
        if (PositionOutOfBounds(piece.row - x, piece.col - x))
            break;

        if (!IsOccupiedBlock(piece.row - x, piece.col - x, board)) {
            possibleMoves.emplace_back(piece.row, piece.col, piece.row - x, piece.col - x);
        } else if (board.Get_Piece(piece.row - x, piece.col - x).color != piece.color) {
            possibleMoves.emplace_back(piece.row, piece.col, piece.row - x, piece.col - x);
            break;
        } else
            break;
    }

    // NE move
    for (int x = 1; x < 8; x++) {
        if (PositionOutOfBounds(piece.row - x, piece.col + x))
            break;

        if (!IsOccupiedBlock(piece.row - x, piece.col + x, board)) {
            possibleMoves.emplace_back(piece.row, piece.col, piece.row - x, piece.col + x);
        } else if (board.Get_Piece(piece.row - x, piece.col + x).color != piece.color) {
            possibleMoves.emplace_back(piece.row, piece.col, piece.row - x, piece.col + x);
            break;
        } else
            break;
    }

    // SW move
    for (int x = 1; x < 8; x++) {
        if (PositionOutOfBounds(piece.row + x, piece.col - x))
            break;

        if (!IsOccupiedBlock(piece.row + x, piece.col - x, board)) {
            possibleMoves.emplace_back(piece.row, piece.col, piece.row + x, piece.col - x);
        } else if (board.Get_Piece(piece.row + x, piece.col - x).color != piece.color) {
            possibleMoves.emplace_back(piece.row, piece.col, piece.row + x, piece.col - x);
            break;
        } else
            break;
    }

    // SE move
    for (int x = 1; x < 8; x++) {
        if (PositionOutOfBounds(piece.row + x, piece.col + x))
            break;

        if (!IsOccupiedBlock(piece.row + x, piece.col + x, board)) {
            possibleMoves.emplace_back(piece.row, piece.col, piece.row + x, piece.col + x);
        } else if (board.Get_Piece(piece.row + x, piece.col + x).color != piece.color) {
            possibleMoves.emplace_back(piece.row, piece.col, piece.row + x, piece.col + x);
            break;
        } else
            break;
    }

    return possibleMoves;
}

vector<Move> MoveGenerator::KingMoves(Board& board, Chess_Piece& piece)
{
    vector<Move> possibleMoves = {};

    // Top
    possibleMoves.emplace_back(piece.row, piece.col, piece.row + 1, piece.col);
    // Top right
    possibleMoves.emplace_back(piece.row, piece.col, piece.row + 1, piece.col + 1);
    // right
    possibleMoves.emplace_back(piece.row, piece.col, piece.row, piece.col + 1);
    // bottom right
    possibleMoves.emplace_back(piece.row, piece.col, piece.row - 1, piece.col + 1);
    // bottom
    possibleMoves.emplace_back(piece.row, piece.col, piece.row - 1, piece.col);
    // bottom left
    possibleMoves.emplace_back(piece.row, piece.col, piece.row - 1, piece.col - 1);
    // left
    possibleMoves.emplace_back(piece.row, piece.col, piece.row, piece.col - 1);
    // top left
    possibleMoves.emplace_back(piece.row, piece.col, piece.row + 1, piece.col - 1);

    // remove moves out of bounds and targeting pieces of similar color
    auto it = possibleMoves.begin();

    while (it != possibleMoves.end()) {
        if (PositionOutOfBounds(it->r2, it->c2) || board.Get_Piece(it->r2, it->c2).color == piece.color)
            it = possibleMoves.erase(it);
        else
            ++it;
    }

    /*
     Castling requirements:
         1. Rook and king is in initial move and on first rank
         2. No pieces between king and rook
         3. King is not in check
         4. The king does not pass through a square that is attacked
    */
    /*
            if(initialMove)
            {
                // Does kingside rook exist && Is king side rook on inital move
                Chess_Piece* kingsideRook = board.Get_Piece(row, 7);
                if(kingsideRook->chess_type == Chess_Type::Rook &&
       kingsideRook->initialMove)
                {
                    bool kingsidecastle = true;

    // Are there any pieces between king and rook
    for(int i = col+1; i < 7; i++)
        if(board.Get_Piece(row, i)->chess_type !=
            Chess_Type::EMPTY) kingsidecastle = false;

    // Are any pieces the king moves through attacked
    if(board.IsAttacked(row, col+1, Chess_Color::White) ||
        board.IsAttacked(row, col+2, Chess_Color::White)) kingsidecastle = false;

    if(kingsidecastle)
        possibleMoves.emplace_back(row, col, row, col+2);
}



// Check that there are no pieces between king and rook


// King side castling

// Queen side castling

//possibleMoves.emplace_back(row, col, row, col+2);
//possibleMoves.emplace_back(row, col, row, col-2);
}
*/

    return possibleMoves;
}
