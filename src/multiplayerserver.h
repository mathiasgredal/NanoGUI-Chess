#ifndef MULTIPLAYERSERVER_H
#define MULTIPLAYERSERVER_H

#include <cstdio>
#include <enet/enet.h>

class MultiplayerServer {
public:
    MultiplayerServer();
    ~MultiplayerServer();

    void PollEvents();

    ENetAddress address;
    ENetHost* server;
};

#endif // MULTIPLAYERSERVER_H
