#ifndef MULTIPLAYERCLIENT_H
#define MULTIPLAYERCLIENT_H

#include "chess_pieces.h"
#include <cstdio>
#include <cstring>
#include <enet/enet.h>

class MultiplayerClient {
public:
    MultiplayerClient();
    ~MultiplayerClient();

    std::string PollEvents();
    bool ConnectToServer();
    void DisconnectFromServer();
    void SendMoveToPeer(Move mv, Chess_Color color);

    ENetHost* client;
};

#endif // MULTIPLAYERCLIENT_H
