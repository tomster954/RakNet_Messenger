#include "PlayStates/ClientState.h"

#include <imgui.h>

#include "ClientMessageStruct.h"

#include "MessageIdentifiers.h"
#include "RakNetStatistics.h"
#include "BitStream.h"
#include "MyMessageIdentifiers.h"

#include <glfw3.h>

#include <string>
#include <stdio.h>

const struct TxtColours
{
	ImColor green = ImColor(100, 255, 100, 255);

};

ClientState::ClientState():
m_clientMessage(ClientMessage())
{
	strcpy_s(m_connectionTxt, "Error Connecting");
}

ClientState::~ClientState()
{

}

void ClientState::Initialise(RakNet::RakPeerInterface *a_peer, char a_serverIP[], char a_userName[], GLFWwindow *a_pWindow)
{
	strcpy_s(m_clientMessage.name, a_userName);
	strcpy_s(m_clientMessage.message, "No Message");
	
	m_peer = a_peer;
	strcpy_s(m_serverIPBuff, a_serverIP);
	m_pWindow = a_pWindow;

	RakNet::SocketDescriptor sd;
	m_peer->Startup(1, &sd, 1);
	m_peer->Connect(m_serverIPBuff, 60000, 0, 0);
	strcpy_s(m_connectionTxt, "Connecting...");
}

void ClientState::Update(float a_dt)
{
	CheckPackets();
}

void ClientState::Draw()
{
	glfwGetWindowSize(m_pWindow, &m_windowWidth, &m_windowHeight);

	//Setting up next window
	ImGui::SetNextWindowSize(ImVec2((float)m_windowWidth, (float)m_windowHeight), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Once);

	//BEGIN
	ImGui::Begin("Messenger", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		DrawHeader();
		//TODO: Draw text box

	ImGui::End();
	//END
}

void ClientState::DrawHeader()
{
	DrawConnectionStats();

	ImGui::TextColored(TxtColours().green, "Name:");		ImGui::SameLine();	ImGui::Text(m_clientMessage.name);

	ImGui::TextColored(TxtColours().green, "Server IP:");	ImGui::SameLine();	ImGui::Text(m_serverIPBuff);	ImGui::SameLine();
	ImGui::Dummy(ImVec2(100, 10));	ImGui::SameLine();		ImGui::TextColored(TxtColours().green, "Connection: ");		ImGui::SameLine(); ImGui::Text(m_connectionTxt);

	ImGui::Separator();

	TextBoxInput();
}

void ClientState::DrawConnectionStats()
{
	RakNet::SystemAddress address;
	unsigned short connections;

	m_peer->GetConnectionList(&address, &connections);
	
	if (connections > 0)
	{
		m_peer->GetStatistics(address, &m_connectionStats);
		RakNet::StatisticsToString(&m_connectionStats, m_connectionStatsStrBuff, 0);
		ImGui::Text(m_connectionStatsStrBuff);
	}
}

void ClientState::TextBoxInput()
{
	int width;
	int height;

	int textBoxHeight = 50;

	glfwGetWindowSize(m_pWindow, &width, &height);

	ImGui::Dummy(ImVec2(width, height - 140 ));
	

	ImGui::InputTextMultiline("", m_clientMessageBuff, sizeof(m_clientMessageBuff), ImVec2(width * 0.8f, textBoxHeight), ImGuiInputTextFlags_CtrlEnterForNewLine);
	ImGui::SameLine();
	
	if (ImGui::Button("Send", ImVec2(width * 0.19, textBoxHeight)))
	{
		//TODO Send the message
		RakNet::SystemAddress address;
		unsigned short numOfCons;

		m_peer->GetConnectionList(&address, &numOfCons);
		if (numOfCons > 1)
		{
			printf("client has more than one connection");
			abort();
		}
		else
		{
			//Send name and message
			strcpy_s(m_clientMessage.message, m_clientMessageBuff);
			memset(&m_clientMessageBuff[0], 0, sizeof(m_clientMessageBuff));

			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_SEND_MESSAGE);
			bsOut.Write<ClientMessage>(m_clientMessage);
			m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address, false);


		}
	}
}

void ClientState::CheckPackets()
{
	for (m_packet = m_peer->Receive(); m_packet; m_peer->DeallocatePacket(m_packet), m_packet = m_peer->Receive())
	{
		switch (m_packet->data[0])
		{
			case ID_SEND_MESSAGE:
			{
				//TODFO:: READ MESSAGE SENT FROM SERVER
				ClientMessage rs;
				//Recieved a message from the server
				RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				
				std::string space = ": ";
				printf(rs.name);
				printf(rs.message);
				
				//m_serverMessages.push_back(rs.name + space + rs.message); 
			}break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				//When the client is accepted
				strcpy_s(m_connectionTxt, "Connected");
				printf("Conneted");
				
				//Send Name
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_REMOTE_NEW_INCOMING_CONNECTION);
				bsOut.Write(m_clientMessage.name);
				m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_packet->systemAddress, false);
			}break;
			case ID_CONNECTION_ATTEMPT_FAILED:
			{
				strcpy_s(m_connectionTxt, "Connection Failed... (Wrong IP)");
				printf("Connection Failed");
			}break;
			//--------------------------------------------------------------------------
			case ID_NO_FREE_INCOMING_CONNECTIONS:
			{
				strcpy_s(m_connectionTxt, "The server is full...");
				printf("server full");
			}break;
			//--------------------------------------------------------------------------
			case ID_DISCONNECTION_NOTIFICATION:
			{
				strcpy_s(m_connectionTxt, "You were disconnected...");
				printf("Disconnected");
			}break;
			//--------------------------------------------------------------------------
			case ID_CONNECTION_LOST:
			{
				strcpy_s(m_connectionTxt, "Connection lost..");
				printf("connection lost");
			}break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
			{
				RakNet::RakString rs;
				RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				printf("%s%s", rs.C_String(), " has connected \n");
			}break;
		}
	}
}