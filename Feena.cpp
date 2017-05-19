// Feena.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Game.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Game newGame;

	while (!newGame.getWindow()->isDone())
	{
		//game loop
		//newGame.handleInput();
		newGame.update();
		newGame.render();
//		sf::sleep(sf::seconds(0.2f));
		newGame.lateUpdate();
	}

	return 0;
}

