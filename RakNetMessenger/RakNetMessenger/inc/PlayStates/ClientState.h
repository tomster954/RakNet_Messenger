//-----------------------------------------------------------------------------
//Author: Tom Solarino
//Description: This is the client state, handles typing messages ect.
//-----------------------------------------------------------------------------

#ifndef CLIENT_STATE
#define CLIENT_STATE
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"

#include "ClientMessageStruct.h"
#include <vector>

struct GLFWwindow;

class ClientState
{
public:
	ClientState();
	~ClientState();

	void Initialise(RakNet::RakPeerInterface *a_peer, char a_serverIP[], char a_userName[], GLFWwindow *a_pWindow);

	void Update(float a_dt);			//This is the main update function
	
	void Draw();						//This is the main draw function
	void DrawHeader();					//This is the header at the top showing name ect.
	void DrawConnectionStats();			//Draws the connection Stats to the server
	void TextBoxInput();				//This is the messages input box
	void DisplayMessages();				//This is the box where all the messages are displayed
	void DisplayOptionsDropDown();		//This is the drop down option at the top of the screen

	void CheckPackets();				//Checks the recived packets

private:
	GLFWwindow *m_pWindow;							//The window pointer
	
	RakNet::RakPeerInterface *m_peer;				//The peer
	RakNet::Packet *m_packet;						//The Packet recieved
	RakNet::RakNetStatistics m_connectionStats;		//This is RakNets Connections stats

	ClientMessage m_clientMessage;					//The clients current message

	char m_serverIPBuff[16];						//The buff for the server IP
	char m_connectionStatsStrBuff[255];				//This is for the connection stats to the server
	char m_clientMessageBuff[255];					//This is the buff for the clients message to everyone
	char m_connectionTxt[32];						//This is the connection status displayed at top of clients screen

	int m_windowWidth;								//Window width
	int m_windowHeight;								//Window height
	
	int m_headerHeight;								//The height of the header
	int m_displayMessageBoxHeight;					//The height of the display message box
	int m_inputBoxHeight;							//The height of the input message box ans send button

	std::vector<ClientMessage> m_allDisplayMessages;//Vector of client messages that gets displayed
	int m_displaymessagesPrevCount;					//Count the prevoise frames message count
	
	bool m_scrollToBottom;							//If true will scroll to the bottom of messages
	bool m_displayConnectionStats;					//If set to true will display the server stats in the header
};
#endif