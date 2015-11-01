#include "PlayStates/ClientState.h"

#include <imgui.h>

#include "MessageIdentifiers.h"
#include "BitStream.h"

#include <glfw3.h>

#include <string>

const struct TxtColours
{
	ImColor green = ImColor(100, 255, 100, 255);

};

ClientState::ClientState()
{
	strcpy_s(m_connectionTxt, "Error Connecting");
}

ClientState::~ClientState()
{

}

void ClientState::Initialise(RakNet::RakPeerInterface *a_peer, char a_serverIP[], char a_userName[], GLFWwindow *a_pWindow)
{
	m_peer = a_peer;
	strcpy_s(m_userNameBuff, a_userName);
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
	ImGui::TextColored(TxtColours().green, "Name:");		ImGui::SameLine();	ImGui::Text(m_userNameBuff);

	ImGui::TextColored(TxtColours().green, "Server IP:");	ImGui::SameLine();	ImGui::Text(m_serverIPBuff);	ImGui::SameLine();
	ImGui::Dummy(ImVec2(100, 10));	ImGui::SameLine();		ImGui::TextColored(TxtColours().green, "Connection: ");		ImGui::SameLine(); ImGui::Text(m_connectionTxt);

	ImGui::Separator();
}

void ClientState::CheckPackets()
{
	for (m_packet = m_peer->Receive(); m_packet; m_peer->DeallocatePacket(m_packet), m_packet = m_peer->Receive())
	{
		switch (m_packet->data[0])
		{
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				//When the client is accepted
				strcpy_s(m_connectionTxt, "Connected");
				printf("Conneted");
				
				//Send Name
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_REMOTE_NEW_INCOMING_CONNECTION);
				bsOut.Write(m_userNameBuff);
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