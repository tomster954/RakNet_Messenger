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
m_setUpComplete(false),
m_canChooseServerOrClient(true)
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
	ImGui::SetNextWindowSize(ImVec2(m_windowWidth, m_windowHeight), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Once);

	//BEGIN
	if (ImGui::Begin("Server Setup", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders))
	{
		if (ImGui::Button("<-Back", ImVec2(50, 20)))
			m_canChooseServerOrClient = true;
		
		ImGui::SameLine();
		ImGui::Text("SERVER SETUP");
		ImGui::Separator();

		ImGui::TextColored(ImVec4(0.5, 1, 0.5, 1),"Room Size:");
		ImGui::SliderInt("People", &m_serverRoomSize, 1, 10);

		//If Done button is pressed the setup is complete
		if (ImGui::Button("Done", ImVec2(100, 40)))
		{
			m_serverState.Initialise(m_pWindow, m_peer, 10, m_serverRoomSize);
			m_setUpComplete = true;
		}
	}
	ImGui::End();
}

void MainMenuState::SetUpClient()
{
	ImGui::SetNextWindowSize(ImVec2(m_windowWidth, m_windowHeight), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Once);
	
	//BEGIN
	if (ImGui::Begin("Client Setup", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders))
	{
		if (ImGui::Button("<-Back", ImVec2(50, 20)))
			m_canChooseServerOrClient = true;

		ImGui::SameLine();
		ImGui::Text("CLIENT SETUP");

		ImGui::Separator();

		//IP Setup
		ImGui::TextColored(ImVec4(0.5,1,0.5,1),"Enter Server IP");
		ImGui::InputText("ServerIP", m_serverIPBuff, 16, ImGuiInputTextFlags_AutoSelectAll);

		//Name Setup
		ImGui::TextColored(ImVec4(0.5, 1, 0.5, 1), "Enter A Name:");
		ImGui::InputText("Name", m_userNameBuff, 16, ImGuiInputTextFlags_AutoSelectAll);

		//If Done button is pressed the setup is complete
		if (ImGui::Button("FINISH", ImVec2(100, 40)))
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
	
	//END
	ImGui::End();
}

void MainMenuState::Update(float a_dt)
{
	glfwGetWindowSize(m_pWindow, &m_windowWidth, &m_windowHeight);

	if (m_setUpComplete && !m_isServer)
		m_clientState.Update(a_dt);

	if (m_setUpComplete && m_isServer)
		m_serverState.Update(a_dt);

}

void MainMenuState::Draw()
{
	if (m_canChooseServerOrClient)
		DrawServerOrClientSelection();
		
	if (m_setUpComplete && m_isServer)
		m_serverState.Draw();

	if (m_setUpComplete && !m_isServer)
		m_clientState.Draw();	

	//will setup either the server or client if the player has selected an option
	if (!m_canChooseServerOrClient && !m_setUpComplete && m_isServer){ SetUpServer(); }
	if (!m_canChooseServerOrClient && !m_setUpComplete && !m_isServer){ SetUpClient(); }
		
}

void MainMenuState::DrawServerOrClientSelection()
{
	ImGui::SetNextWindowSize(ImVec2(m_windowWidth, m_windowHeight), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Once);
	//BEGIN
	ImGui::Begin("Main Menu", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ShowBorders);
	{
		if (ImGui::Button("CLIENT", ImVec2(m_windowWidth *0.48f, m_windowHeight*0.93f)))
		{
			m_canChooseServerOrClient = false;
			m_isServer = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("SERVER", ImVec2(m_windowWidth *0.48f, m_windowHeight*0.93f)))
		{
			m_canChooseServerOrClient = false;
			m_isServer = true;
		}
	}
	ImGui::End();
	//END
}