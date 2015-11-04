//-----------------------------------------------------------------------------
//Author: Tom Solarino
//Description: This is the client state, handles typing messages ect.
//-----------------------------------------------------------------------------

#ifndef SERVER_STATE
#define SERVER_STATE
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"

#include <vector>
struct GLFWwindow;

class ServerState
{
public:
	ServerState();
	~ServerState();
	
	void Initialise(GLFWwindow *a_pWindow, RakNet::RakPeerInterface *a_peer, int a_totalMaxClients, int a_currentRoomSize);

	void Update(float a_dt);
	void Draw();
	void DrawHeader();

	

private:
	//Private Funcs
	void CheckPackets();
	void DisplayServerMessages();

	GLFWwindow *m_pWindow;
	RakNet::RakPeerInterface *m_peer;
	RakNet::Packet *m_packet;

	char m_serverIPBuff[16];
	char m_userNameBuff[16];
	char m_serverStatsStrBuff[255];
	char m_connectionTxt[32];

	int m_windowWidth;
	int m_windowHeight;

	int m_totalMaxClients;
	int m_currentRoomSize;

	
	std::vector<std::string> m_serverMessages;

	RakNet::RakNetStatistics m_serverStats;
private:

};
#endif