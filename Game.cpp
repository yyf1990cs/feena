#include "stdafx.h"
#include "Game.h"
#include "Biz/GameDefine.h"

Game::Game()
	:m_window("Test", sf::Vector2u(800, 400))
	, m_stateManager(&m_context)
	, m_msPrev(0)
	, m_msCur(0)
	, m_drawTimes(0)
{

	m_fpsFont.loadFromFile("./resource/simhei.ttf");

	m_fpsText.setFillColor(sf::Color::Red);
	m_fpsText.setFont(m_fpsFont);
	m_fpsText.setCharacterSize(12);

	m_context.window = &m_window;
	m_context.eventManager = m_window.getEventManager();
	m_stateManager.switchTo(GAME_STATE_INTRO);
}


Game::~Game()
{
}

void Game::update()
{
	m_window.update();
	m_stateManager.update(m_elapsed);

	m_msCur = get_ms_time();
	if (m_msPrev + 1000 <= m_msCur)
	{
		m_fpsText.setString(ntstr(m_drawTimes));
		m_drawTimes = 0;
		m_msPrev = m_msCur;
	}
	m_drawTimes++;
}



void Game::render()
{
	m_window.beginDraw();
	m_stateManager.draw();
	m_window.draw(m_fpsText);
	m_window.endDraw();
}


void Game::lateUpdate()
{
	m_stateManager.processRequests();
	restartClock();
}

GameWindow* Game::getWindow()
{
	return &m_window;
}


void Game::restartClock()
{
	m_elapsed = m_clock.restart();
}


