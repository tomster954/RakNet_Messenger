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

	void Update(float a_dt);
	
	void Draw();
	void DrawHeader();
	void DrawConnectionStats();
	void TextBoxInput();
	void DisplayServerMessages();

	void CheckPackets();

private:
	GLFWwindow *m_pWindow;
	
	RakNet::RakPeerInterface *m_peer;
	RakNet::Packet *m_packet;
	RakNet::RakNetStatistics m_connectionStats;

	ClientMessage m_clientMessage;

	char m_serverIPBuff[16];			//The buff for the server IP
	char m_connectionStatsStrBuff[255]; //This is for the connection stats to the server
	char m_clientMessageBuff[255];		//This is the clients message to everyone
	char m_connectionTxt[32];			//This is the connection status displayed at top of clients screen

	int m_windowWidth;
	int m_windowHeight;

	std::vector<std::string> m_displayMessages; //Veector of client messages that gets displayed
	int m_displaymessagesPrevCount;				//Count the prevoise frames message count
	bool m_scrollToBottom;						//If true will scroll to the bottom of messages
};
#endif