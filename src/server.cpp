#include "multiplayerserver.h"
#include <chrono>
#include <iostream>
#include <thread>

int main()
{

    using namespace std::chrono_literals;

    MultiplayerServer server = MultiplayerServer();

    while (true) {
        server.PollEvents();

        std::this_thread::sleep_for(5ms);
    }

    return 0;
}
