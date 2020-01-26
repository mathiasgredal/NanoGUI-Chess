#include "multiplayerclient.h"

MultiplayerClient::MultiplayerClient()
{
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        exit(EXIT_FAILURE);
    }

    atexit(enet_deinitialize);

    client = enet_host_create(nullptr /* create a client host */,
        1 /* only allow 1 outgoing connection */,
        1 /* allow up 2 channels to be used, 0 and 1 */,
        0 /* assume any amount of incoming bandwidth */,
        0 /* assume any amount of outgoing bandwidth */);

    if (client == nullptr) {
        fprintf(stderr,
            "An error occurred while trying to create an ENet client host.\n");
        exit(EXIT_FAILURE);
    }
}

MultiplayerClient::~MultiplayerClient()
{
    enet_host_destroy(client);
}

std::string MultiplayerClient::PollEvents()
{
    ENetEvent event;
    /* Wait up to 1000 milliseconds for an event. */
    while (enet_host_service(client, &event, 10) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
            printf("A new client connected from %x:%u.\n",
                event.peer->address.host,
                event.peer->address.port);
            /* Store any relevant client information here. */
            //event.peer->data = "Client information";
            break;
        case ENET_EVENT_TYPE_RECEIVE: {
            std::ostringstream convert;

            printf("A packet of length %u containing %s was received from %s on channel %u.\n",
                event.packet->dataLength,
                event.packet->data,
                event.peer->data,
                event.channelID);

            //return static_cast<const char*>(event.packet->);

            for (int i = 0; i < event.packet->dataLength; i++) {
                convert << event.packet->data[i];
            }

            return convert.str();

            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);
            break;
        }

        case ENET_EVENT_TYPE_DISCONNECT:
            printf("%s disconnected.\n", event.peer->data);
            /* Reset the peer's client information. */
            event.peer->data = nullptr;
        }
    }

    return "";
}

bool MultiplayerClient::ConnectToServer()
{
    ENetAddress address;
    ENetEvent event;
    ENetPeer* peer;

    /* Connect to some.server.net:1234. */
    enet_address_set_host(&address, "localhost");
    address.port = 5000;

    /* Initiate the connection, allocating the one channel. */
    peer = enet_host_connect(client, &address, 1, 0);
    if (peer == nullptr) {
        fprintf(stderr,
            "No available peers for initiating an ENet connection.\n");
        exit(EXIT_FAILURE);
    }

    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connection to " << address.host << ":" << address.port << " succeeded." << std::endl;
        return true;
    } else {
        enet_peer_reset(peer);
        std::cout << "Connection to " << address.host << ":" << address.port << " failed." << std::endl;
        return false;
    }
}

void MultiplayerClient::SendMoveToPeer(Move mv, Chess_Color color)
{
    std::ostringstream packetString;
    packetString << ((color == Chess_Color::White) ? "0" : "1") << ": ";
    packetString << mv.to_string();

    /* Create a reliable packet of size 7 containing "packet\0" */
    ENetPacket* packet = enet_packet_create(packetString.str().data(),
        packetString.str().size(),
        ENET_PACKET_FLAG_RELIABLE);

    /* Send the packet to the peer over channel id 0. */
    /* One could also broadcast the packet by         */
    /* enet_host_broadcast (host, 0, packet);         */
    enet_host_broadcast(client, 0, packet);

    /* One could just use enet_host_service() instead. */
    enet_host_flush(client);
}
