#ifndef COMPUTER_H
#define COMPUTER_H

#include "chess_pieces.h"

class Computer {
public:
    static Move GetRandomMove(Board& board);
    static Move GetSimpleMove(Board& board);
    static Move GetMiniMaxMove(Board& board);

private:
    Computer() {}
};

#endif // COMPUTER_H
