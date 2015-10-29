//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Description: Is called in the Application class
//-----------------------------------------------------------------------------

#ifndef STARTUP
#define STARTUP
//#define GLFW_INCLUDE_GLU
#include "PlayStates\MainMenuState.h"

struct GLFWwindow;

class Application
{
public:
	Application();
	~Application();

	void SetUpGLFW();

	void Update();
	void Draw();

	void Run();
private:

	float m_currentTime = 0.0f;
	float m_deltaTime = 0.0f;
	float m_lastTime = 0.0f;
	
	GLFWwindow* m_pWindow;

	MainMenuState m_mainMenuState;
};
#endif