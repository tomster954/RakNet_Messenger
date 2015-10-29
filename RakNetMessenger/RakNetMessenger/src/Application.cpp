#include "Application.h"

#include <imgui.h>
#include "imgui_impl_glfw.h"

#include <glfw3.h>
#include <iostream>

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

Application::Application() :
m_currentTime(0.0f),
m_deltaTime(0.0f),
m_lastTime(0.0f),
m_mainMenuState(MainMenuState())
{
	SetUpGLFW();

	ImGui_ImplGlfw_Init(m_pWindow, true);
	
	m_mainMenuState.Initialise();

	Run();
}

Application::~Application()
{
	ImGui_ImplGlfw_Shutdown();
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void Application::SetUpGLFW()
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
		return;
	}

	m_pWindow = glfwCreateWindow(640, 480, "Chat room", NULL, NULL);

	if (!m_pWindow)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
		return;
	}
	glfwMakeContextCurrent(m_pWindow);

	//Setting the background to white
	glClearColor(255, 255, 255, 255);
}

void Application::Run()
{
	while (!glfwWindowShouldClose(m_pWindow))
	{
		if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)	glfwSetWindowShouldClose(m_pWindow, true);

		int width, height;
		glfwGetFramebufferSize(m_pWindow, &width, &height);
		glViewport(0, 0, width, height);
		
		Update();
		Draw();

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}
}

void Application::Update()
{
	m_currentTime = (float)glfwGetTime();
	m_deltaTime = m_currentTime - m_lastTime; // prev of last frame
	m_lastTime = m_currentTime;

	m_mainMenuState.Update(m_deltaTime);
}

void Application::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Render new frame
	ImGui_ImplGlfw_NewFrame();

		m_mainMenuState.Draw(m_pWindow);

	//Render IMGUI
	ImGui::Render();
}