//-----------------------------------------------------------------------------
//Author Tom Solarino
//Description: My message struct, messages are sent with a name and the massage
//-----------------------------------------------------------------------------
#ifndef CLIENT_MESSAGE_STRUCT
#define CLIENT_MESSAGE_STRUCT
//#include "RakString.h"
#include <string>

struct ClientMessage
{
	char name[16];
	char message[255];
};

#endif