//-----------------------------------------------------------------------------
//Author: Tom Solarino
//Description: This is the main menu where you select setting and input,
//				also sets up raknet.
//-----------------------------------------------------------------------------

#ifndef MAIN_MENU_STATE
#define MAIN_MENU_STATE

#include "PlayStates\ClientState.h"
#include "PlayStates\ServerState.h"

#include "RakPeerInterface.h"

#include <string>

//Pre Defs
struct GLFWwindow;

class MainMenuState
{
public:
	//Public Funcs
	MainMenuState();
	~MainMenuState();

	void Initialise(GLFWwindow* a_pWindow);

	void Update(float a_dt);
	
	void Draw();
	void DrawSetUp();

	bool IsServer(){ return m_isServer; }

	//Public Vars
	bool m_setUpComplete;			//Will be true once entire setup is complete

private:
	//Private Funcs
	void SetUpServer();
	void SetUpClient();

	//Private Vars
	GLFWwindow* m_pWindow;
	ClientState m_clientState;
	ServerState m_serverState;

	RakNet::RakPeerInterface *m_peer;
	RakNet::Packet *m_packet;

	char m_serverIPBuff[16];
	char m_userNameBuff[16];

	int m_serverRoomSize;

	bool m_isServer;				//Will be true if the user chose server
	bool m_choseServerOrClient;		//Will be true once the player has chosen either client or server
};
#endif