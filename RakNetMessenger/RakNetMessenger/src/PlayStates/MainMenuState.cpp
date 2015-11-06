#include "PlayStates/MainMenuState.h"
#include "PlayStates/ClientState.h"

#include "RakNetTypes.h"  // MessageID
#include "RakPeer.h"

#include <imgui.h>
#include "imgui_impl_glfw.h"

#include <glfw3.h>

#include <string>

MainMenuState::MainMenuState() :
m_clientState(ClientState()),
m_serverState(ServerState()),
m_isServer(NULL),
m_setUpComplete(false)
{

	//set name and ip to a default value
	strcpy_s(m_serverIPBuff, "127.0.0.1");
	strcpy_s(m_userNameBuff, "Anonymous User");
	m_serverRoomSize = 5;
}

MainMenuState::~MainMenuState()
{

}

void MainMenuState::Initialise(GLFWwindow* a_pWindow)
{
	m_pWindow = a_pWindow;

	m_peer = RakNet::RakPeerInterface::GetInstance();
}

void MainMenuState::SetUpServer()
{
	//TODO Set up code for the server
	ImGui::Text("Server Set Up");
	ImGui::Separator();
	
	ImGui::Text("Room Size:");
	ImGui::SliderInt("People", &m_serverRoomSize, 1, 10);

	//If Done button is pressed the setup is complete
	if (ImGui::Button("Done", ImVec2(100, 40)))
	{
		m_serverState.Initialise(m_pWindow, m_peer, 10, m_serverRoomSize);
		m_setUpComplete = true;
	}
}

void MainMenuState::SetUpClient()
{
	ImGui::Text("Client Setup");
	ImGui::Separator();

	//IP Setup
	ImGui::Text("Enter a server IP or leave blank for 127.0.0.1:");
	ImGui::InputText("ServerIP", m_serverIPBuff, 16, ImGuiInputTextFlags_AutoSelectAll);

	//Name Setup
	ImGui::Text("Enter your name:");
	ImGui::InputText("Name", m_userNameBuff, 16, ImGuiInputTextFlags_AutoSelectAll);

	//If Done button is pressed the setup is complete
	if (ImGui::Button("Done", ImVec2(100, 40)))
	{
		//If no server IP was put in set the default
		if (m_serverIPBuff[0] == 0) strcpy_s(m_serverIPBuff, "127.0.0.1");
		
		//If the first two character of the name are 0 set the name to default
		if (m_userNameBuff[0] == 0 && m_userNameBuff[1] == 0)
			strcpy_s(m_userNameBuff, "Anonymous User");

		m_clientState.Initialise(m_peer, m_serverIPBuff, m_userNameBuff, m_pWindow);
		m_setUpComplete = true;
	}
}

void MainMenuState::Update(float a_dt)
{
	if (m_setUpComplete && !m_isServer)
		m_clientState.Update(a_dt);

	if (m_setUpComplete && m_isServer)
		m_serverState.Update(a_dt);

}

void MainMenuState::Draw()
{
		//If Setup isnt complete
		if (!m_setUpComplete)
			DrawSetUp();
		
		if (m_setUpComplete && m_isServer)
		{
			m_serverState.Draw();
		}
		if (m_setUpComplete && !m_isServer)
		{
			m_clientState.Draw();
		}
		
		
}

void MainMenuState::DrawSetUp()
{
	int width;
	int height;

	glfwGetWindowSize(m_pWindow, &width, &height);

	bool show = true;

	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Once);
	//BEGIN
	ImGui::Begin("Main Menu", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	
		ImGui::Text("Choose to be a client or server");

		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Client", ImVec2(100, 40)))
		{
			m_isServer = false;
		}

		ImGui::SameLine(0, 100);

		if (ImGui::Button("Server", ImVec2(100, 40)))
		{
			m_isServer = true;
		}

		//will setup either the server or client if the player has selected an option
		if (m_isServer){ SetUpServer(); }
		if (!m_isServer){ SetUpClient(); }

	ImGui::End();
	//END
}