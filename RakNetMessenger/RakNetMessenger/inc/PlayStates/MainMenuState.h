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
	MainMenuState();
	~MainMenuState();

	void Initialise(GLFWwindow* a_pWindow);

	void Update(float a_dt);				//Main Update function
	
	void Draw();							//Main Drawfunction
	void DrawServerOrClientSelection();		//Is the initial option to be server or client

	bool IsServer(){ return m_isServer; }

	
private:

	bool m_setUpComplete;				//Will be true once entire setup is complete ie done is pressed

	//Private Funcs
	void SetUpServer();					//Will set up the required componets for the server
	void SetUpClient();					//Will set up the required componets for the client

	//Private Vars
	GLFWwindow* m_pWindow;				//The main window
	ClientState m_clientState;			//The client state
	ServerState m_serverState;			//The server state

	RakNet::RakPeerInterface *m_peer;	//The peer
	RakNet::Packet *m_packet;			//The Rak packets

	char m_serverIPBuff[16];			//buff for the server ip input
	char m_userNameBuff[16];			//Buff for the users name input

	int m_windowWidth;					//Window width
	int m_windowHeight;					//Window heigth
	int m_serverRoomSize;				//The maxinum amount of people connecting to the server

	bool m_isServer;					//Will be true if the user chose server
	bool m_canChooseServerOrClient;
};
#endif