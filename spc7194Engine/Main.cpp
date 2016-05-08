#include "Engine.h"

int main()
{
	Engine myEngine;

	if (!myEngine.init()) 
	{
		return -1;
	}

	if (!myEngine.bufferModels())
	{
		return -1;
	}

	if (!myEngine.useShaders())
	{
		return -1;
	}

	myEngine.gameLoop();

	return 0;
}