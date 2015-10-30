//-----------------------------------------------------------------------------
//Author: Tom Solarino
//Description: This is the client state, handles typing messages ect.
//-----------------------------------------------------------------------------

#ifndef CLIENT_STATE
#define CLIENT_STATE
struct GLFWwindow;

class ClientState
{
public:
	ClientState();
	~ClientState();

	void Initialise(char a_serverIP[], char a_userName[]);

	void Update(float a_dt);
	void Draw(GLFWwindow* a_pWindow);

private:
	char m_serverIPBuff[16];
	char m_userNameBuff[16];

};
#endif