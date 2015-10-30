//-----------------------------------------------------------------------------
//Author: Tom Solarino
//Description: This is the main menu where you select setting and input
//-----------------------------------------------------------------------------

#ifndef MAIN_MENU_STATE
#define MAIN_MENU_STATE

#include "PlayStates\ClientState.h"

#include <string>

//Pre Defs
struct GLFWwindow;

class MainMenuState
{
public:
	//Public Funcs
	MainMenuState();
	~MainMenuState();

	void Update(float a_dt);
	
	void Draw(GLFWwindow* a_pWindow);
	void DrawSetUp(GLFWwindow* a_pWindow);

	bool IsServer(){ return m_isServer; }

	//Public Vars
	bool m_setUpComplete;			//Will be true once entire setup is complete

private:
	//Private Funcs
	void SetUpServer();
	void SetUpClient();

	//Private Vars
	ClientState m_clientState;

	char m_serverIPBuff[16];
	char m_userNameBuff[16];

	bool m_isServer;				//Will be true if the user chose server
	bool m_choseServerOrClient;		//Will be true once the player has chosen either client or server
};
#endif