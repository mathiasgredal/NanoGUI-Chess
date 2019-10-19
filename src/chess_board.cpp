#include <utility>

#include "chess_pieces.h"


Board::Board(vector<vector<Chess_Piece*>> pieces)
{
    chess_pieces = std::move(pieces);

    emptyPiece = new class Empty();
}

Board::~Board()
{
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if(chess_pieces[row][col] != nullptr)
            {
                delete chess_pieces[row][col];
            }
        }
    }
}

Board::Board(const Board& rhs)
{
    //cout << "Board created by deep copy" << endl;
    board_size = rhs.board_size;
    s_size = rhs.s_size;
    currentPlayer = rhs.currentPlayer;
    chess_pieces = vector(8, vector<Chess_Piece*> (8));
    emptyPiece = rhs.emptyPiece;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if(rhs.chess_pieces[row][col] != nullptr)
            {
                chess_pieces[row][col] = (*rhs.chess_pieces[row][col]).Clone();
            }
        }
    }
}

int Board::GetBoardScore()
{
    float sum = 0;
    for(auto& row: chess_pieces)
    {
        for(auto& piece: row)
        {
            if(piece != nullptr)
                sum += (piece->color != currentPlayer)? piece->piece_Value : piece->piece_Value*-1;
        }
    }
    cout << "Board Evaluation: " << sum << endl;
    return sum;
}

vector<Move> Board::AllPossibleMoves()
{
    return {};
}

vector<Chess_Piece*> Board::GetPiecesOfColor(Chess_Color color)
{
    vector<Chess_Piece*> pieces(0);
    for (auto& row : chess_pieces) {
        for (auto& piece : row) {
            if(piece != nullptr && piece->color == color)
                pieces.push_back(piece);
        }
    }

    return pieces;
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
        return emptyPiece;
}

void Board::Move_Piece(Move mv)
{
    // TODO: This method is doing very dangerous things, can potentially crash the program
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

    // Get Piece
    Chess_Piece* selectedPiece = Get_Piece(row, col);

    // Check if the piece is of the correct color
    if(selectedPiece->color != this->currentPlayer)
        return {};

    Chess_Color opponentPlayer = (this->currentPlayer == Chess_Color::White)?
                Chess_Color::Black : Chess_Color::White;

    vector<Move> validmoves = selectedPiece->ValidMoves(*this);


    // remove invalid moves
    auto it = validmoves.begin();

    while(it != validmoves.end())
    {
        bool invalidMove = false;
        // Get Piece
        Chess_Piece* destinationPiece = Get_Piece(it->r2, it->c2);

        // We are not allowed to attack a king
        if(destinationPiece->chess_type == Chess_Type::King)
            invalidMove = true;

        // We are not allowed to make a move which exposes the king
        Board newboard = *this;

        newboard.Move_Piece(*it);
        for (auto& piece : newboard.GetPiecesOfColor(opponentPlayer)) {
            for (auto& possibleMove : piece->ValidMoves(newboard)) {
                if(newboard.Get_Piece(possibleMove.r2, possibleMove.c2)->chess_type == Chess_Type::King)
                    invalidMove = true;
            }
        }

        if(invalidMove)
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

    for(Move& validmove: validmoves){
        //validmove.Print_Move();
        if(validmove == usermove)
            return true;
    }

    return false;
}
