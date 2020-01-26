#include "multiplayerserver.h"

MultiplayerServer::MultiplayerServer()
{
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
    }

    atexit(enet_deinitialize);

    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */
    address.host = ENET_HOST_ANY;
    /* Bind the server to port 1234. */
    address.port = 5000;

    server = enet_host_create(&address /* the address to bind the server host to */,
        2 /* allow up to 2 clients and/or outgoing connections */,
        2 /* allow up to 2 channels to be used, 0 and 1 */,
        0 /* assume any amount of incoming bandwidth */,
        0 /* assume any amount of outgoing bandwidth */);

    if (server == nullptr) {
        fprintf(stderr,
            "An error occurred while trying to create an ENet server host.\n");
        exit(EXIT_FAILURE);
    }
}

MultiplayerServer::~MultiplayerServer()
{
    enet_host_destroy(server);
}

void MultiplayerServer::PollEvents()
{
    ENetEvent event;
    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service(server, &event, 1000) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
            printf("A new client connected from %x:%u.\n",
                event.peer->address.host,
                event.peer->address.port);
            /* Store any relevant client information here. */
            //event.peer -> data = "Client information";
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            printf("A packet of length %u containing %s was received from %s on channel %u.\n",
                event.packet->dataLength,
                event.packet->data,
                event.peer->data,
                event.channelID);
            /* Clean up the packet now that we're done using it. */
            enet_host_broadcast(server, 0, event.packet);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            printf("%s disconnected.\n", event.peer->data);
            /* Reset the peer's client information. */
            event.peer->data = nullptr;
        }
    }
}
