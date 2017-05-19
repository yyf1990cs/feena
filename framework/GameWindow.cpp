#include "stdafx.h"
#include "framework/GameWindow.h"

GameWindow::GameWindow()
	:m_isFullscreen(false)
	, m_isDone(false)
{
	setup("new window", sf::Vector2u(1280,720));
}

GameWindow::GameWindow(const std::string& title, const sf::Vector2u& size)
	:m_isFullscreen(false)
	, m_isDone(false)
{
	setup(title, size);
}


GameWindow::~GameWindow()
{
	destroy();
}


void GameWindow::setup(const std::string& l_title, const sf::Vector2u& l_size)
{
	m_windowTitle = l_title;
	m_windowSize = l_size;
	create();

	m_isFocused = true;
	m_eventManager.addCallback(sf::Event::Closed, 0, &GameWindow::close, this);
	m_eventManager.addCallback(sf::Event::KeyPressed, sf::Keyboard::F5, &GameWindow::toggleFullscreen, this);
	m_eventManager.addCallback(sf::Event::JoystickButtonPressed, -1, &GameWindow::changeResolution, this);
}

void GameWindow::create()
{
	std::vector<sf::VideoMode> modes(sf::VideoMode::getFullscreenModes());
	auto style = m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Close | sf::Style::Titlebar;
	m_window.create({ m_windowSize.x, m_windowSize.y, 32 }, m_windowTitle, style);
	m_window.setFramerateLimit(60);
}

void GameWindow::destroy()
{
	m_window.close();
}


void GameWindow::update()
{
	sf::Event e;
	while (m_window.pollEvent(e))
	{
		if (e.type == sf::Event::LostFocus)
		{
			m_isFocused = false;
			m_eventManager.setFocus(false);
		}
		else if (e.type == sf::Event::GainedFocus)
		{
			m_isFocused = true;
			m_eventManager.setFocus(true);
		}
		m_eventManager.handleEvent(e);
	}
	m_eventManager.update();
}


void GameWindow::toggleFullscreen(EventInfo* info)
{
	m_isFullscreen = !m_isFullscreen; 
	destroy();
	create();
}

void GameWindow::changeResolution(EventInfo* info)
{

}


void GameWindow::close(EventInfo* details)
{
	m_isDone = true;
}

void GameWindow::beginDraw()
{
	m_window.clear(sf::Color(0x000000FF));
}

void GameWindow::endDraw()
{
	m_window.display();
}

void GameWindow::draw(sf::Drawable& l_drawable)
{
	m_window.draw(l_drawable);
}