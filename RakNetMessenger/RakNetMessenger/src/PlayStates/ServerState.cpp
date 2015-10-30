#include"PlayStates\ServerState.h"

#include <imgui.h>

#include "MessageIdentifiers.h"
#include "BitStream.h"

#define SERVER_PORT 60000
#define TOTAL_MAX_CLIENTS 10
ServerState::ServerState()
{
}

ServerState::~ServerState()
{
}

void ServerState::Initialise(GLFWwindow *a_pWindow, RakNet::RakPeerInterface *a_peer, int a_totalMaxClients, int a_currentRoomSize)
{
	m_peer = a_peer;
	m_totalMaxClients = a_totalMaxClients;
	m_currentRoomSize = a_currentRoomSize;

	RakNet::SocketDescriptor sd(SERVER_PORT, 0);
	m_peer->Startup(a_totalMaxClients, &sd, 1);
	m_peer->SetMaximumIncomingConnections(a_totalMaxClients);
}

void ServerState::Update(float a_dt)
{

}

void ServerState::Draw()
{

}

void ServerState::DrawHeader()
{

}

void ServerState::CheckPackets()
{
	for (m_packet = m_peer->Receive(); m_packet; m_peer->DeallocatePacket(m_packet), m_packet = m_peer->Receive())
	{
		switch (m_packet->data[0])
		{
			//--------------------------------------------------------------------------
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			{
				printf("Another client has disconnected.\n");
			}break;
			//--------------------------------------------------------------------------
			case ID_REMOTE_CONNECTION_LOST:
			{
				printf("Another client has lost the connection.\n");
			}break;
			//--------------------------------------------------------------------------
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
			{
				//TODO:: Send name to everone except sender
				//printf("Another client has connected.\n");

				RakNet::RakString rs;
				RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				printf("%s%s", rs.C_String(), " has connected \n");


				RakNet::SystemAddress addressList[TOTAL_MAX_CLIENTS];

				unsigned short ushort = m_currentRoomSize;
				m_peer->GetConnectionList(addressList, &ushort);

				for (int i = 0; i < ushort; i++)
				{
				  if (addressList[i] == m_packet->systemAddress)
					  continue;

				  //TODO send name to all clients
				  RakNet::BitStream bsOut;
				  bsOut.Write((RakNet::MessageID)ID_REMOTE_NEW_INCOMING_CONNECTION);
				  bsOut.Write(rs.C_String());
				  m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addressList[i], false);
				}
			}break;
			//--------------------------------------------------------------------------

			case ID_NEW_INCOMING_CONNECTION:
			{
				//SetConsoleTextAttribute(m_hConsole, TextColour::YELLOW);
				printf("A connection is incoming.\n");
				//SetConsoleTextAttribute(m_hConsole, TextColour::WHITE);
			}break;
			//--------------------------------------------------------------------------
			case ID_DISCONNECTION_NOTIFICATION:
			{
				printf("A client has disconnected.\n");
			}break;
			case ID_CONNECTION_LOST:
			{
				//SetConsoleTextAttribute(m_hConsole, TextColour::RED);
				printf("A client lost the connection.\n");
				//SetConsoleTextAttribute(m_hConsole, TextColour::WHITE);
			}break;
		}
	}
}