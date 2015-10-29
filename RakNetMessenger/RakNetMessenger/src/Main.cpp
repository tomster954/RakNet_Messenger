//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Description: This is the main function
//-----------------------------------------------------------------------------
#include "Application.h"

int main(int argc, char *argv[]) 
{
	Application *app = new Application();
	app->Run();
	delete app;
}