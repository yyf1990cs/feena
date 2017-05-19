#include "stdafx.h"
#include "StatePaused.h"


StatePaused::StatePaused(StateManager* manager)
	:ListenerState(manager)
{
}


StatePaused::~StatePaused()
{
}

void StatePaused::onCreate()
{
	setTransparent(true);

	m_font.loadFromFile("./resource/simhei.ttf");
	m_text.setFont(m_font);
	m_text.setString("GAME IS PAUSED");
	m_text.setCharacterSize(14);
	m_text.setStyle(sf::Text::Bold);

	sf::Vector2u windowSize = m_stateManager->getContext()->window->getWindowSize();
	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.width / 2, textRect.height / 2);
	m_text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	m_rect.setSize(sf::Vector2f(windowSize));
	m_rect.setPosition(0, 0);
	m_rect.setFillColor(sf::Color(0x000000aa));

}

void StatePaused::onDestroy()
{
}

void StatePaused::activate()
{
	addCodeListener(sf::Event::KeyPressed, sf::Keyboard::Escape, &StatePaused::unpause);
	addCodeListener(sf::Event::JoystickButtonPressed, sf::Joystick::Back, &StatePaused::unpause);
}

void StatePaused::deactivate()
{
	removeCodeListener(sf::Event::KeyPressed, sf::Keyboard::Escape, &StatePaused::unpause);
	removeCodeListener(sf::Event::JoystickButtonPressed, sf::Joystick::Back, &StatePaused::unpause);
}

void StatePaused::update(const sf::Time& delta)
{

}

void StatePaused::draw()
{
	sf::RenderWindow* window = m_stateManager->getContext()->window->getWindow();
	window->draw(m_rect);
	window->draw(m_text);
}

void StatePaused::unpause(EventInfo* detail)
{
	m_stateManager->switchTo(GAME_STATE_GAMING);
}

