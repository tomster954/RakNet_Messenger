#include"PlayStates\ServerState.h"

#include <imgui.h>

#include "ClientMessageStruct.h"

#include "MessageIdentifiers.h"
#include "RakNetStatistics.h"
#include "BitStream.h"
#include "MyMessageIdentifiers.h"

#include "glfw3.h"

#include <string>

#define SERVER_PORT 60000
#define TOTAL_MAX_CLIENTS 10
ServerState::ServerState() : 
m_serverStats(RakNet::RakNetStatistics())
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
	m_pWindow = a_pWindow;

	glfwGetWindowSize(m_pWindow, &m_windowWidth, &m_windowHeight);

	RakNet::SocketDescriptor sd(SERVER_PORT, 0);
	m_peer->Startup(a_totalMaxClients, &sd, 1);
	m_peer->SetMaximumIncomingConnections(a_totalMaxClients);
}

void ServerState::Update(float a_dt)
{
	glfwGetWindowSize(m_pWindow, &m_windowWidth, &m_windowHeight);

	CheckPackets();
}

void ServerState::Draw()
{
	//Setting Up Next Window
	ImGui::SetNextWindowSize(ImVec2((float)m_windowWidth, (float)m_windowHeight), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Once);

	//BEGIN
	ImGui::Begin("Server Messages", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	//------------------------------------
		DrawHeader();
		DisplayServerMessages();
	//------------------------------------
	ImGui::End();
	//END
}

void ServerState::DrawHeader()
{
	////Getting the address
	//RakNet::SystemAddress address;
	//address = m_peer->GetSystemAddressFromGuid(m_peer->GetMyGUID());
	//
	////Drawing server stats
	//m_peer->GetStatistics(address, &m_serverStats);
	//RakNet::StatisticsToString(&m_serverStats, m_serverStatsStrBuff, 0);
	//ImGui::Text(m_serverStatsStrBuff);
	//
	////Finding the number of connections
	//unsigned short connections;
	//m_peer->GetConnectionList(&address, &connections);
	//std::string blah = std::to_string(connections);
	//ImGui::Text(blah.c_str());

	//TODO display info ie server size, connected people, ping if ur cool...
	ImGui::Text("Room: 3/10");
	ImGui::Text("Up Time: 01.20.19");
	ImGui::Separator();
}

void ServerState::CheckPackets()
{
	for (m_packet = m_peer->Receive(); m_packet; m_peer->DeallocatePacket(m_packet), m_packet = m_peer->Receive())
	{
		switch (m_packet->data[0])
		{
			case ID_SEND_MESSAGE:
			{
				//TODO:: RECEIVE AND SEND TO ALL CLIENTS
				
				ClientMessage rs;
				//Recieved a message from a client
				RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);

				//send to all clients
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_SEND_MESSAGE);
				bsOut.Write<ClientMessage>(rs);
				m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_packet->systemAddress, true);
				
				//Print message to client
				std::string space = ": ";
				m_serverMessages.push_back(rs.name + space + rs.message);

			}break;
			//--------------------------------------------------------------------------
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			{
				m_serverMessages.push_back("Another client has disconnected");
				printf("Another client has disconnected.\n");
			}break;
			//--------------------------------------------------------------------------
			case ID_REMOTE_CONNECTION_LOST:
			{
				m_serverMessages.push_back("Another client has lost the connection");
				printf("Another client has lost the connection.\n");
			}break;
			//--------------------------------------------------------------------------
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
			{
				RakNet::RakString rs;
				RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				
				std::string s = " has Connected\n";
				printf("%s%s", rs.C_String(), s);
				
				m_serverMessages.push_back(rs.C_String() + s);

				//RakNet::SystemAddress addressList[TOTAL_MAX_CLIENTS];
				//
				//unsigned short ushort = m_currentRoomSize;
				//m_peer->GetConnectionList(addressList, &ushort);
				//
				//for (int i = 0; i < ushort; i++)
				//{
				//  if (addressList[i] == m_packet->systemAddress)
				//	  continue;

				  //TODO send name to all clients
				  RakNet::BitStream bsOut;
				  bsOut.Write((RakNet::MessageID)ID_REMOTE_NEW_INCOMING_CONNECTION);
				  bsOut.Write(rs.C_String());
				  m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_packet->systemAddress, true);
				//}
			}break;
			//--------------------------------------------------------------------------

			case ID_NEW_INCOMING_CONNECTION:
			{
				m_serverMessages.push_back("A connection is incoming");
				printf("A connection is incoming.\n");
			}break;
			//--------------------------------------------------------------------------
			case ID_DISCONNECTION_NOTIFICATION:
			{
				m_serverMessages.push_back("A client has disconnected");
				printf("A client has disconnected.\n");
			}break;
			case ID_CONNECTION_LOST:
			{
				m_serverMessages.push_back("A client lost the connection");
				printf("A client lost the connection.\n");
			}break;
		}
	}
}

void ServerState::DisplayServerMessages()
{

	for (int i = 0; i < m_serverMessages.size(); i++)
	{
		//Only display if the message isnt NULL
		ImGui::Text(m_serverMessages[i].c_str());

	}
	//todo:: Loop servermessages and draw them to the server window
}