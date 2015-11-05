//-----------------------------------------------------------------------------
//Author: Tom Solarino
//Description: This is the client state, handles typing messages ect.
//-----------------------------------------------------------------------------

#ifndef CLIENT_STATE
#define CLIENT_STATE
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"

#include "ClientMessageStruct.h"

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

	void CheckPackets();

private:
	GLFWwindow *m_pWindow;
	
	RakNet::RakPeerInterface *m_peer;
	RakNet::Packet *m_packet;
	RakNet::RakNetStatistics m_connectionStats;

	ClientMessage m_clientMessage;

	char m_serverIPBuff[16];
	char m_userNameBuff[16];
	char m_connectionStatsStrBuff[255];
	char m_clientMessageBuff[255];

	int m_windowWidth;
	int m_windowHeight;

	char m_connectionTxt[32];

};
#endif