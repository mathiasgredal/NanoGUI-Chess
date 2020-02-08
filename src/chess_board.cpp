#include <utility>

#include "chess_pieces.h"
#include "movegenerator.h"
#include <array>

Board::Board(array<Chess_Piece, 64> pieces)
{
    chess_pieces = std::move(pieces);
}

int Board::GetBoardScore()
{
    float sum = 0;
    for (auto& piece : chess_pieces) {
        sum += (piece.color != currentPlayer) ? ChessPieceValue(piece.chessType) : ChessPieceValue(piece.chessType) * -1;
    }
    //cout << "Board Evaluation: " << sum << endl;
    return sum;
}

vector<Chess_Piece> Board::GetPiecesOfColor(Chess_Color color)
{
    vector<Chess_Piece> pieces(18);

    for (auto& piece : chess_pieces) {
        if (piece.color == color)
            pieces.push_back(piece);
    }

    pieces.shrink_to_fit();
    return pieces;
}

array<Chess_Piece, 64> Board::Default_Board()
{
    array<Chess_Piece, 64> default_pieces;

    default_pieces[GetChessPieceIndex(0, 0)] = Chess_Piece(0, 0, Chess_Color::Black, Chess_Type::Rook);
    default_pieces[GetChessPieceIndex(0, 1)] = Chess_Piece(0, 1, Chess_Color::Black, Chess_Type::Knight);
    default_pieces[GetChessPieceIndex(0, 2)] = Chess_Piece(0, 2, Chess_Color::Black, Chess_Type::Bishop);
    default_pieces[GetChessPieceIndex(0, 3)] = Chess_Piece(0, 3, Chess_Color::Black, Chess_Type::Queen);
    default_pieces[GetChessPieceIndex(0, 4)] = Chess_Piece(0, 4, Chess_Color::Black, Chess_Type::King);
    default_pieces[GetChessPieceIndex(0, 5)] = Chess_Piece(0, 5, Chess_Color::Black, Chess_Type::Bishop);
    default_pieces[GetChessPieceIndex(0, 6)] = Chess_Piece(0, 6, Chess_Color::Black, Chess_Type::Knight);
    default_pieces[GetChessPieceIndex(0, 7)] = Chess_Piece(0, 7, Chess_Color::Black, Chess_Type::Rook);

    for (int i = 0; i < 8; i++)
        default_pieces[GetChessPieceIndex(1, i)] = Chess_Piece(1, i, Chess_Color::Black, Chess_Type::Pawn);

    default_pieces[GetChessPieceIndex(7, 0)] = Chess_Piece(7, 0, Chess_Color::White, Chess_Type::Rook);
    default_pieces[GetChessPieceIndex(7, 1)] = Chess_Piece(7, 1, Chess_Color::White, Chess_Type::Knight);
    default_pieces[GetChessPieceIndex(7, 2)] = Chess_Piece(7, 2, Chess_Color::White, Chess_Type::Bishop);
    default_pieces[GetChessPieceIndex(7, 3)] = Chess_Piece(7, 3, Chess_Color::White, Chess_Type::Queen);
    default_pieces[GetChessPieceIndex(7, 4)] = Chess_Piece(7, 4, Chess_Color::White, Chess_Type::King);
    default_pieces[GetChessPieceIndex(7, 5)] = Chess_Piece(7, 5, Chess_Color::White, Chess_Type::Bishop);
    default_pieces[GetChessPieceIndex(7, 6)] = Chess_Piece(7, 6, Chess_Color::White, Chess_Type::Knight);
    default_pieces[GetChessPieceIndex(7, 7)] = Chess_Piece(7, 7, Chess_Color::White, Chess_Type::Rook);

    for (int i = 0; i < 8; i++)
        default_pieces[GetChessPieceIndex(6, i)] = Chess_Piece(6, i, Chess_Color::White, Chess_Type::Pawn);

    return default_pieces;
}

Chess_Piece Board::Get_Piece(int row, int col)
{
    if (row < 8 && col < 8)
        return chess_pieces[GetChessPieceIndex(row, col)];
    else
        return Chess_Piece(row, col, Chess_Color::Empty, Chess_Type::EMPTY);
}

void Board::Move_Piece(Move mv)
{
    //bool castlingMove = (Get_Piece(mv.r1, mv.c1)->chess_type == Chess_Type::King && abs(mv.c1 - mv.c2) > 1);

    // TODO: This method is doing very dangerous things, can potentially crash the program
    //chess_pieces[mv.r1][mv.c1]->RegisterMove(mv);
    lastMove = mv;

    chess_pieces[GetChessPieceIndex(mv.r2, mv.c2)] = chess_pieces[GetChessPieceIndex(mv.r1, mv.c1)];
    chess_pieces[GetChessPieceIndex(mv.r1, mv.c1)] = Chess_Piece(mv.r1, mv.c1, Chess_Color::Empty, Chess_Type::EMPTY);

    chess_pieces[GetChessPieceIndex(mv.r2, mv.c2)].row = mv.r2;
    chess_pieces[GetChessPieceIndex(mv.r2, mv.c2)].col = mv.c2;
    chess_pieces[GetChessPieceIndex(mv.r2, mv.c2)].initialMove = false;

    if (chess_pieces[GetChessPieceIndex(mv.r2, mv.c2)].chessType == Chess_Type::Pawn) {
        if (chess_pieces[GetChessPieceIndex(mv.r2, mv.c2)].color == Chess_Color::Black && mv.r2 == 7) {
            chess_pieces[GetChessPieceIndex(mv.r2, mv.c2)].chessType = Chess_Type::Queen;
        }

        if (chess_pieces[GetChessPieceIndex(mv.r2, mv.c2)].color == Chess_Color::White && mv.r2 == 0) {
            chess_pieces[GetChessPieceIndex(mv.r2, mv.c2)].chessType = Chess_Type::Queen;
        }
    }

    /*
    if (castlingMove && mv.c1 < mv.c2) // King side castling
    {
        chess_pieces[mv.r1][7]->RegisterMove(mv);
        chess_pieces[mv.r1][mv.c2 - 1] = chess_pieces[mv.r1][7];
        chess_pieces[mv.r1][7] = nullptr;
    }
    if (castlingMove && mv.c1 > mv.c2) {
        // Queen side castling
        chess_pieces[mv.r1][0]->RegisterMove(mv);
        chess_pieces[mv.r1][mv.c2 + 1] = chess_pieces[mv.r1][0];
        chess_pieces[mv.r1][0] = nullptr;
    }*/

    // Switch rounds
    if (currentPlayer == Chess_Color::White)
        currentPlayer = Chess_Color::Black;
    else
        currentPlayer = Chess_Color::White;
}

vector<Move> PieceValidMoves(Chess_Piece piece, Board& board)
{
    switch (piece.chessType) {
    case King:
        return MoveGenerator::KingMoves(board, piece);
    case Queen:
        return MoveGenerator::QueenMoves(board, piece);
    case Bishop:
        return MoveGenerator::BishopMoves(board, piece);
    case Knight:
        return MoveGenerator::KnightMoves(board, piece);
    case Rook:
        return MoveGenerator::RookMoves(board, piece);
    case Pawn:
        return MoveGenerator::PawnMoves(board, piece);
    case EMPTY:
        return {};
    }
}

vector<Move> Board::ValidMoves(int row, int col)
{

    // Get Piece
    Chess_Piece selectedPiece = Get_Piece(row, col);

    // Check if the piece is of the correct color
    if (selectedPiece.color != this->currentPlayer)
        return {};

    Chess_Color opponentPlayer = (this->currentPlayer == Chess_Color::White) ? Chess_Color::Black : Chess_Color::White;

    vector<Move> validmoves = PieceValidMoves(selectedPiece, *this);

    // remove invalid moves
    auto it = validmoves.begin();
    while (it != validmoves.end()) {
        bool invalidMove = false;
        // Get Piece
        Chess_Piece destinationPiece = Get_Piece(it->r2, it->c2);

        // We are not allowed to attack a king
        if (destinationPiece.chessType == Chess_Type::King)
            invalidMove = true;

        // We are not allowed to make a move which exposes the king
        Board newboard = *this;

        newboard.Move_Piece(*it);
        for (auto& piece : newboard.GetPiecesOfColor(opponentPlayer)) {
            for (auto& possibleMove : PieceValidMoves(piece, newboard)) {
                if (newboard.Get_Piece(possibleMove.r2, possibleMove.c2).chessType == Chess_Type::King)
                    invalidMove = true;
            }
        }

        if (invalidMove)
            it = validmoves.erase(it);
        else
            ++it;
    }

    return validmoves;
}

bool Board::ValidMove(Move usermove)
{
    vector<Move> validmoves = ValidMoves(usermove.r1, usermove.c1);

    // TODO: Use STL functions
    // if(std::find(validmoves.begin(), validmoves.end(), mv) != validmoves.end())
    //     return true;
    // else
    //     return false;

    for (Move& validmove : validmoves) {
        //validmove.Print_Move();
        if (validmove == usermove)
            return true;
    }

    return false;
}

Chess_Piece Board::GetKing(Chess_Color color)
{
    // Calculate if player is in check
    vector<Chess_Piece> all_pieces = GetPiecesOfColor(currentPlayer);

    for (auto& piece : all_pieces) {
        if (piece.chessType == Chess_Type::King) {
            return piece;
        }
    }

    return Chess_Piece(0, 0, Chess_Color::Empty, Chess_Type::EMPTY);
}

bool Board::IsAttacked(int row, int col, Chess_Color color)
{
    if (currentPlayer == Chess_Color::Empty)
        return false;

    Chess_Color enemyColor = (color == Chess_Color::White) ? Chess_Color::Black : Chess_Color::White;

    // Loop all enemy moves, if any of them reach the king, then he is in check
    vector<Chess_Piece> enemyPieces = GetPiecesOfColor(enemyColor);

    for (auto& enemypiece : enemyPieces) {
        if (enemypiece.chessType != Chess_Type::King) {
            vector<Move> enemyMoves = PieceValidMoves(enemypiece, *this);

            for (auto& enemyMove : enemyMoves) {
                if (enemyMove.r2 == row && enemyMove.c2 == col) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Board::IsCheck()
{
    Chess_Piece kingPiece = GetKing(currentPlayer);

    return IsAttacked(kingPiece.row, kingPiece.col, currentPlayer);
}

bool Board::IsCheckMate()
{
    bool isCheckMate = true;

    // Can only be checkmate if king is in check
    if (!IsCheck()) {
        isCheckMate = false;
        return isCheckMate;
    }

    // Here we just check whether we can do any moves.
    for (int i = 0; i < chess_pieces.size(); i++) {
        const int c = i % 8;
        const int r = (i - c) / 8;

        if (ValidMoves(r, c).size() != 0)
            isCheckMate = false;
    }

    return isCheckMate;
}
