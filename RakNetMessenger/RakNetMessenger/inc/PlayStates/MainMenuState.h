//-----------------------------------------------------------------------------
//Author: Tom Solarino
//Description: This is the main menu where you select setting and input
//-----------------------------------------------------------------------------

#ifndef MAIN_MENU_STATE
#define MAIN_MENU_STATE
#include <string>
struct GLFWwindow;

class MainMenuState
{
public:
	MainMenuState();
	~MainMenuState();

	void Initialise(); 

	void Update(float a_dt);
	void Draw(GLFWwindow* a_pWindow);

private:
	std::string m_userMessage;
	char m_messageBuff[255];
};
#endif