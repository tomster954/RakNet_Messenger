#include "PlayStates/MainMenuState.h"
#include "PlayStates/ClientState.h"

#include <imgui.h>
#include "imgui_impl_glfw.h"

#include <glfw3.h>

#include <string>
//#include <stdio.h>

MainMenuState::MainMenuState() :
m_clientState(ClientState()),
m_isServer(NULL),
m_setUpComplete(false)
{

}

MainMenuState::~MainMenuState()
{

}

void MainMenuState::SetUpServer()
{
	//TODO Set up code for the server
	ImGui::Text("Server Set Up");
	ImGui::Separator();
	
}

void MainMenuState::SetUpClient()
{
	ImGui::Text("Client Set Up");
	ImGui::Separator();

	//IP Setup
	strcpy_s(m_serverIPBuff, "127.0.0.1");
	ImGui::Text("Enter a server IP or leave blank for 127.0.0.1:");
	ImGui::InputText("ServerIP", m_serverIPBuff, 16);

	//Name Setup
	ImGui::Text("Enter your name:");
	ImGui::InputText("Name", m_userNameBuff, 16);

	//If Done button is pressed the setup is complete
	if (ImGui::Button("Done", ImVec2(100, 40)))
	{
		//If no server IP was put in set the default
		if (m_serverIPBuff[0] == 0) strcpy_s(m_serverIPBuff, "127.0.0.1");
		
		//If the first two character of the name are 0 set the name to default
		if (m_userNameBuff[0] == 0 && m_userNameBuff[1] == 0)
			strcpy_s(m_userNameBuff, "Anonymous User");

		m_setUpComplete = true; 
		m_clientState.Initialise(m_serverIPBuff, m_userNameBuff);
	}
	
}

void MainMenuState::Update(float a_dt)
{
	
}

void MainMenuState::Draw(GLFWwindow* a_pWindow)
{
		//If Setup isnt complete
		if (!m_setUpComplete)
			DrawSetUp(a_pWindow);
		
		if (m_setUpComplete && m_isServer)
		{
			//server state
		}
		if (m_setUpComplete && !m_isServer)
		{
			m_clientState.Draw(a_pWindow);
		}
		
		
}

void MainMenuState::DrawSetUp(GLFWwindow* a_pWindow)
{
	int width;
	int height;

	glfwGetWindowSize(a_pWindow, &width, &height);

	bool show = true;

	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Once);
	//BEGIN
	ImGui::Begin("SetUp", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	
		ImGui::Text("Choose to be a client or server");

		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Client", ImVec2(100, 40)) && m_choseServerOrClient == false)
		{
			m_isServer = false;
			m_choseServerOrClient = true;
		}

		ImGui::SameLine(0, 100);

		if (ImGui::Button("Server", ImVec2(100, 40)) && m_choseServerOrClient == false)
		{
			m_isServer = true;
			m_choseServerOrClient = true;
		}

		//will setup either the server or client if the player has selected an option
		if (m_choseServerOrClient == true)
		{
			if (m_isServer){ SetUpServer(); }
			if (!m_isServer){ SetUpClient(); }
		}

	ImGui::End();
	//END
}

//ImGui::InputText("Message", m_messageBuff, 255);
//m_userMessage = m_messageBuff;