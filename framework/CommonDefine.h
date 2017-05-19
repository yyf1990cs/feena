#pragma once

class GameWindow;
class EventManager;
struct SharedContext
{
	SharedContext()
		: window(nullptr)
		, eventManager(nullptr)
	{
	}

	GameWindow* window;
	EventManager* eventManager;
};

