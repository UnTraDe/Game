#include "Server.h"

Server::Server(int port)
{
	m_ENetAddress.host = ENET_HOST_ANY;
	m_ENetAddress.port = port;

	m_ENetHost = enet_host_create(&m_ENetAddress, 32, 2, 0, 0);

	world.Initialize();
}

Server::~Server()
{
	enet_host_destroy(m_ENetHost);
}

void Server::Run()
{
	std::thread worldThread(&World::Run, world);
	
	int counter = 0;
	ENetEvent event;
	WorldStatePacket wsp;

	while (enet_host_service(m_ENetHost, &event, 1000) > -1)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
			event.peer->data = &counter;
			m_Clients.insert(std::pair<int, Client>(counter, Client(event.peer)));
			counter++;

			//Temp
			wsp = world.TakeSnapshot();
			m_Clients[counter - 1].Send(wsp.GetData(), wsp.GetDataSize(), 0, 0);

			break;
		case ENET_EVENT_TYPE_RECEIVE:
			ProcessPacket(event.packet);
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			printf("Client %d disconected.\n", event.peer->data);
			//event.peer->data = NULL;
			break;
		}

		WorldStatePacket wsp = world.TakeSnapshot();
	}
}

void Server::ProcessPacket(ENetPacket* packet)
{
	
}