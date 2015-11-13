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
m_serverStats(RakNet::RakNetStatistics()),
m_upTime(0.0f)
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
	m_dt = a_dt;
	m_upTime += m_dt;

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
	unsigned short connectionsSize;
	std::string s;

	m_peer->GetConnectionList(NULL, &connectionsSize);
	s = "People: " + std::to_string(connectionsSize) + "/" + std::to_string(m_currentRoomSize);

	

	ImGui::Text(s.c_str());
	ImGui::Text(std::to_string(m_upTime).c_str());
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
				//Recieved a message from a client
				ClientMessage rs;
				RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);

				//send to all other clients
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_SEND_MESSAGE);
				bsOut.Write<ClientMessage>(rs);
				m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_packet->systemAddress, true);
				
				//Print message to server aswell
				std::string space = ": ";
				m_serverMessages.push_back(rs.name + space + rs.message);

			}break;
			//--------------------------------------------------------------------------
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			{
				m_serverMessages.push_back("Another client has disconnected");
			}break;
			//--------------------------------------------------------------------------
			case ID_REMOTE_CONNECTION_LOST:
			{
				m_serverMessages.push_back("Another client has lost the connection");
			}break;
			//--------------------------------------------------------------------------
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
			{
				//Recieve name on clients connection
				ClientMessage rs;
				RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				
				//Display Name server
				std::string s = " has Connected\n";
				m_serverMessages.push_back(rs.name + s);

				//Send name to all other clients
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_REMOTE_NEW_INCOMING_CONNECTION);
				bsOut.Write<ClientMessage>(rs);
				m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_packet->systemAddress, true);
			}break;
			//--------------------------------------------------------------------------

			case ID_NEW_INCOMING_CONNECTION:
			{
				m_serverMessages.push_back("A connection is incoming");
			}break;
			//--------------------------------------------------------------------------
			case ID_DISCONNECTION_NOTIFICATION:
			{
				m_serverMessages.push_back("A client has disconnected");
			}break;
			case ID_CONNECTION_LOST:
			{
				m_serverMessages.push_back("A client lost the connection");
			}break;
		}
	}
}

void ServerState::DisplayServerMessages()
{
	for (int i = 0; i < m_serverMessages.size(); i++)
	{
		ImGui::Text(m_serverMessages[i].c_str());
	}
}

//RakNet::RakString rs;
//RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
//bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
//bsIn.Read(rs);