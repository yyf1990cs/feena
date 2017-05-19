#pragma once
#include "framework/GameWindow.h"
#include "framework/state/BaseState.h"
#include "framework/state/StateManager.h"

class Game
{
public:
	Game();
	~Game();

	void update();
	void render();
	GameWindow* getWindow();

	sf::Time getElapsed() { return m_elapsed; }

	void lateUpdate();

private:
	void restartClock();

private:
	GameWindow m_window;
	GAME_STATE m_state;
	StateManager m_stateManager;

	SharedContext m_context;

	sf::Clock m_clock;
	sf::Time m_elapsed;

	sf::Text m_fpsText;
	sf::Font m_fpsFont;

	uint32 m_msPrev;
	uint32 m_msCur;
	uint32 m_drawTimes;
};

