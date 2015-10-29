#include "PlayStates/MainMenuState.h"

#include <imgui.h>
#include "imgui_impl_glfw.h"

#include <glfw3.h>

MainMenuState::MainMenuState()
{

}

MainMenuState::~MainMenuState()
{

}

void MainMenuState::Initialise()
{

}

void MainMenuState::Update(float a_dt)
{

}

void MainMenuState::Draw(GLFWwindow* a_pWindow)
{
	int width;
	int height;

	glfwGetWindowSize(a_pWindow, &width, &height);
	
	bool show = true;

	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiSetCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_Once);
	//BEGIN
	//----------------------------------------------------------------------------
	ImGui::Begin("Server or Client?", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

		ImGui::Text("Choose to be a client or server", ImGuiAlign_Center | ImGuiAlign_Right);

		ImGui::Separator();
		ImGui::Spacing();
		if (ImGui::Button("Client", ImVec2(100, 40)))
		{
			//TODO: Do client stuff
		}
		ImGui::SameLine(0, 100);
		if (ImGui::Button("Server", ImVec2(100, 40)))
		{
			//TODO: Do client stuff
		}

	//END
	//----------------------------------------------------------------------------
	ImGui::End();
}

//ImGui::InputText("Message", m_messageBuff, 255);
//m_userMessage = m_messageBuff;