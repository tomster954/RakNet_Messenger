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

ClientState::ClientState() :
m_clientMessage(ClientMessage()),
m_displaymessagesPrevCount(0),
m_headerHeight(50),
m_displayMessageBoxHeight(300),
m_inputBoxHeight(50)
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
	int padding = 70;
	m_displayMessageBoxHeight = m_windowHeight - (m_headerHeight + m_inputBoxHeight + padding);

	CheckPackets();
}

void ClientState::Draw()
{
	glfwGetWindowSize(m_pWindow, &m_windowWidth, &m_windowHeight);

	//Setting up next window
	ImGui::SetNextWindowSize(ImVec2((float)m_windowWidth, (float)m_windowHeight), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Once);

	//BEGIN
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	if (ImGui::Begin("Messenger", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar))
	{
		DisplayOptionsDropDown();

		DrawHeader();

		DisplayMessages();

		TextBoxInput();

	}
	ImGui::End();
	ImGui::PopStyleVar();
	//END
	
}

void ClientState::DrawHeader()
{
	//BEGIN
	ImGui::BeginChild("Header", ImVec2((float)m_windowWidth - 15, m_headerHeight), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		ImGui::TextColored(TxtColours().green, "Your Name:");		ImGui::SameLine();	ImGui::Text(m_clientMessage.name);

		ImGui::TextColored(TxtColours().green, "Server IP:");	ImGui::SameLine();	ImGui::Text(m_serverIPBuff);	ImGui::SameLine();
		ImGui::Dummy(ImVec2(100, 10));	ImGui::SameLine();		ImGui::TextColored(TxtColours().green, "Connection: ");		ImGui::SameLine(); ImGui::Text(m_connectionTxt);

		if (m_displayConnectionStats)
			DrawConnectionStats();

	ImGui::EndChild();
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

void ClientState::DisplayMessages()
{
	//BEGIN
	ImGui::BeginChild("Display messages", ImVec2((float)m_windowWidth - 15, m_displayMessageBoxHeight), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		//Loops through all messages to display
		for (int i = 0; i < m_allDisplayMessages.size(); i++)
		{
			//Name : Text
			ImGui::TextColored(ImVec4(.8, 1, .8, 1), m_allDisplayMessages[i].name); ImGui::SameLine();
			ImGui::TextColored(ImVec4(.8, 1, .8, 1), ":"); ImGui::SameLine();
			ImGui::Text(m_allDisplayMessages[i].message);
		}

		//If true will scroll to the bottom of the messages
		if (m_scrollToBottom)
		{
			ImGui::SetScrollY(ImGui::GetScrollMaxY());
			m_scrollToBottom = false;
		}
		//If the are more messages set scroll down to true
		if (m_displaymessagesPrevCount != m_allDisplayMessages.size()) m_scrollToBottom = true;
		
		//setting the message count
		m_displaymessagesPrevCount = m_allDisplayMessages.size();

	
	ImGui::EndChild();
	//END
}

void ClientState::TextBoxInput()
{
	int width;
	int height;

	glfwGetWindowSize(m_pWindow, &width, &height);

	//Checking if the user pressed enter or pressed send
	float sendMessage = false;

	if (ImGui::InputTextMultiline("", m_clientMessageBuff, sizeof(m_clientMessageBuff), ImVec2(width * 0.8f, m_inputBoxHeight), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AlwaysInsertMode))
	{
		sendMessage = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Send", ImVec2(width * 0.18, m_inputBoxHeight)))
	{
		sendMessage = true;
	}

	//If the send button is pressed
	if (sendMessage)
	{
		//TODO:: Check that there is and actually message

		//Getting the server adress
		RakNet::SystemAddress address[1];
		unsigned short numOfCons;
		m_peer->GetConnectionList(address, &numOfCons);
		
		//Copy message buffer into the clients message
		strcpy_s(m_clientMessage.message, m_clientMessageBuff);
		memset(&m_clientMessageBuff[0], 0, sizeof(m_clientMessageBuff));

		//Send name and message
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_SEND_MESSAGE);
		bsOut.Write<ClientMessage>(m_clientMessage);
		m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address[0], false);

		//Display message to this client
		std::string s = "You";

		ClientMessage messBuff = m_clientMessage;
		strcpy_s(messBuff.name, sizeof(messBuff.name), s.c_str());

		m_allDisplayMessages.push_back(messBuff);
	}
}

void ClientState::DisplayOptionsDropDown()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("ServerStats",NULL, m_displayConnectionStats)) 
			{
				m_displayConnectionStats = !m_displayConnectionStats;
				
				if (m_displayConnectionStats)
					m_headerHeight += 40;
				else
					m_headerHeight -= 40;
			}
			if (ImGui::MenuItem("IP")) 
			{
				//TODO:: add a way of changing the ip if possible
			}
			if (ImGui::MenuItem("Name")) 
			{
				//TODO abilty to change name :)
			}
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("People"))
		{
			//TODO::Actually display connected people
			ImGui::MenuItem("Person1");
			ImGui::MenuItem("Person2");
			ImGui::MenuItem("Person3");

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
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
				ClientMessage rs;
				//Recieved a client message from the server
				RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				
				//Display the clients message to this client

				m_allDisplayMessages.push_back(rs);

			}break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				//When the client is accepted
				strcpy_s(m_connectionTxt, "Connected \n");

				//Send Name to server
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_REMOTE_NEW_INCOMING_CONNECTION);
				bsOut.Write<ClientMessage>(m_clientMessage);
				m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_packet->systemAddress, false);
			}break;
			case ID_CONNECTION_ATTEMPT_FAILED:
			{
				strcpy_s(m_connectionTxt, "Connection Failed...(Wrong IP)");
			}break;
			//--------------------------------------------------------------------------
			case ID_NO_FREE_INCOMING_CONNECTIONS:
			{
				strcpy_s(m_connectionTxt, "The server is full...");
			}break;
			//--------------------------------------------------------------------------
			case ID_DISCONNECTION_NOTIFICATION:
			{
				strcpy_s(m_connectionTxt, "You were disconnected...");
			}break;
			//--------------------------------------------------------------------------
			case ID_CONNECTION_LOST:
			{
				strcpy_s(m_connectionTxt, "Connection lost..");
			}break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
			{
				//Recieve connections name
				ClientMessage rs;
				RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(rs);
				
				//Display connections name
				std::string s = "has Connected";

				ClientMessage messBuff = rs;
				strcpy_s(messBuff.message, s.c_str());

				m_allDisplayMessages.push_back(messBuff);
			}break;
		}
	}
}