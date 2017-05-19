#include "stdafx.h"
#include "StateIntro.h"


StateIntro::StateIntro(StateManager* manager)
	:ListenerState(manager)
{
}


StateIntro::~StateIntro()
{
}

void StateIntro::onCreate()
{
	m_timePassed = 0;

	sf::Vector2u windowSize = m_stateManager->getContext()->window->getWindowSize();

	m_introTexture.loadFromFile("./resource/intro.png");
	m_introSprite.setTexture(m_introTexture);
	m_introSprite.setOrigin(m_introTexture.getSize().x / 2.0f, m_introTexture.getSize().y / 2.0f);
	m_introSprite.setPosition(windowSize.x / 2.0f, 0);

	m_font.loadFromFile("./resource/simhei.ttf");
	m_text.setFont(m_font);
	m_text.setString("Press SPACE to continue.");
	m_text.setCharacterSize(15);

	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
	m_text.setPosition((float)windowSize.x / 2, (float)windowSize.y * 3 / 4);

}

void StateIntro::onDestroy()
{
}

void StateIntro::activate()
{
	addCodeListener(sf::Event::EventType::KeyPressed, sf::Keyboard::Space, &StateIntro::Continue);
	addCodeListener(sf::Event::EventType::JoystickButtonPressed, sf::Joystick::Start, &StateIntro::Continue);
}

void StateIntro::deactivate()
{
	removeCodeListener(sf::Event::EventType::KeyPressed, sf::Keyboard::Space, &StateIntro::Continue);
	removeCodeListener(sf::Event::EventType::JoystickButtonPressed, sf::Joystick::Start, &StateIntro::Continue);
}

void StateIntro::update(const sf::Time& delta)
{
	if (m_timePassed < 5.0f)
	{
		m_timePassed += delta.asSeconds();
		m_introSprite.setPosition(m_introSprite.getPosition().x, m_introSprite.getPosition().y + (48 * delta.asSeconds()));
	}
}
void StateIntro::draw()
{
	sf::RenderWindow* window = m_stateManager->getContext()->window->getWindow();
	window->draw(m_introSprite);
	if (m_timePassed >= 5.0f)
	{
		window->draw(m_text);
	}
}

void StateIntro::Continue(EventInfo* detals)
{
	if (m_timePassed >= 1.0f)
	{
		m_stateManager->switchTo(GAME_STATE_MAIN_MENU);
		m_stateManager->remove(GAME_STATE_INTRO);
	}
}