#include "PlayStates/ClientState.h"

#include <imgui.h>
//#include "imgui_impl_glfw.h"

#include <glfw3.h>

#include <string>
ClientState::ClientState()
{

}

ClientState::~ClientState()
{

}

void ClientState::Initialise(char a_serverIP[], char a_userName[])
{
	strcpy_s(m_userNameBuff, a_userName);
	strcpy_s(m_serverIPBuff, a_serverIP);
}

void ClientState::Update(float a_dt)
{

}

void ClientState::Draw(GLFWwindow *a_pWindow)
{
	int width;
	int height;

	glfwGetWindowSize(a_pWindow, &width, &height);

	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Once);

	//BEGIN
	ImGui::Begin("Messenger", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	ImGui::Text("Name:");
	ImGui::Text(m_userNameBuff);
	ImGui::Text("Server IP:");
	ImGui::Text(m_serverIPBuff);
	ImGui::Separator();
	//Client state

	ImGui::End();
	//END
}