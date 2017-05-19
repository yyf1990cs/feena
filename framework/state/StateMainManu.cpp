#include "stdafx.h"
#include "StateMainManu.h"


StateMainManu::StateMainManu(StateManager* stateManager)
	:ListenerState(stateManager)
	, m_selectIndex(0)
{
}


StateMainManu::~StateMainManu()
{
}


void StateMainManu::onCreate()
{
	sf::Vector2u windowSize = m_stateManager->getContext()->window->getWindowSize();

	m_font.loadFromFile("./resource/simhei.ttf");
	m_text.setFont(m_font);
	m_text.setString("MAIN MENU");
	m_text.setCharacterSize(20);


	sf::FloatRect rect = m_text.getLocalBounds();
	m_text.setOrigin(rect.left + rect.width / 2, rect.top + rect.height / 2);
	m_text.setPosition((float)windowSize.x / 2, (float)windowSize.y / 2 - 100);

	m_buttonSize = sf::Vector2<float>(300.0f, 32.0f);
	m_buttonPos = sf::Vector2<float>((float)windowSize.x / 2, (float)windowSize.y / 2 - 50);
	m_buttonPadding = 4;

	std::array<std::string, 3> str{ "PLAY" , "CONFIG", "EXIT"};
	for (uint32 i = 0; i < 3; ++i)
	{
		sf::Vector2<float> buttonPosition(m_buttonPos.x, m_buttonPos.y + i * (m_buttonSize.y + m_buttonPadding));
		m_rects[i].setSize(m_buttonSize);
		m_rects[i].setFillColor(sf::Color::Red);
		m_rects[i].setOrigin(m_buttonSize.x / 2, m_buttonSize.y / 2);
		m_rects[i].setPosition(buttonPosition);

		m_labels[i].setFont(m_font);
		m_labels[i].setString(str[i]);
		m_labels[i].setCharacterSize(12);

		rect = m_labels[i].getLocalBounds();
		m_labels[i].setFillColor(sf::Color::Black);
		m_labels[i].setOrigin(rect.left + rect.width / 2, rect.top + rect.height / 2);
		m_labels[i].setPosition(buttonPosition);
	}

	m_selectRect.setSize(m_buttonSize);
	m_selectRect.setFillColor(sf::Color::Yellow);
	m_selectRect.setOrigin(m_buttonSize.x / 2, m_buttonSize.y / 2);
	m_selectRect.setPosition(m_buttonPos.x, m_buttonPos.y);
}


void StateMainManu::onDestroy()
{
}

void StateMainManu::activate()
{
	if (m_stateManager->hasState(GAME_STATE_GAMING) && m_labels[0].getString() == "PLAY")
	{
		m_labels[0].setString("RESUME");
		sf::FloatRect rect = m_labels[0].getLocalBounds();
		m_labels[0].setOrigin(rect.left + rect.width / 2, rect.top + rect.height / 2);
	}


	addCodeListener(sf::Event::MouseButtonPressed, sf::Mouse::Left, &StateMainManu::mouseClick);
	addListener(sf::Event::MouseMoved, &StateMainManu::mouseHoverMenu);
	addListener(sf::Event::JoystickButtonPressed, &StateMainManu::JoystickClick);
}

void StateMainManu::deactivate()
{
	removeCodeListener(sf::Event::MouseButtonPressed, sf::Mouse::Left, &StateMainManu::mouseClick);
	removeListener(sf::Event::MouseMoved, &StateMainManu::mouseHoverMenu);
	removeListener(sf::Event::JoystickButtonPressed, &StateMainManu::JoystickClick);
}

void StateMainManu::mouseClick(EventInfo* info)
{
	sf::Vector2i mousePos = info->detail.mouse;

	float halfX = m_buttonSize.x / 2.0f;
	float halfY = m_buttonSize.y / 2.0f;
	for (uint32 i = 0; i < 3; ++i)
	{
		if (mousePos.x >= m_rects[i].getPosition().x - halfX && mousePos.x < m_rects[i].getPosition().x + halfX
			&&
			mousePos.y >= m_rects[i].getPosition().y - halfY && mousePos.y < m_rects[i].getPosition().y + halfY)
		{
			onMenuSelected();
		}
	}
}

void StateMainManu::onMenuSelected()
{
	if (m_selectIndex == 0)
	{
		m_stateManager->switchTo(GAME_STATE_GAMING);
	}
	else if (m_selectIndex == 1)
	{
		//config state
	}
	else if (m_selectIndex == 2)
	{
		m_stateManager->getContext()->window->close();
	}
}

void StateMainManu::JoystickClick(EventInfo* info)
{
	switch (info->detail.code)
	{
		case sf::Joystick::Arrow_Down:
		case sf::Joystick::LStick_Down:
		case sf::Joystick::RStick_Down:
			{
				if (m_selectIndex < 2)
				{
					++m_selectIndex;
					m_selectRect.setPosition(m_buttonPos.x, m_buttonPos.y + m_selectIndex * (m_buttonSize.y + m_buttonPadding));
				}
			}
			break;
		case sf::Joystick::Arrow_Up:
		case sf::Joystick::LStick_Up:
		case sf::Joystick::RStick_Up:
			{
				if (m_selectIndex > 0)
				{
					--m_selectIndex;
					m_selectRect.setPosition(m_buttonPos.x, m_buttonPos.y + m_selectIndex * (m_buttonSize.y + m_buttonPadding));
				}
			}
			break;
		case sf::Joystick::B:
			{
				onMenuSelected();
			}
			break;
		default:
			break;
	}
}


void StateMainManu::mouseHoverMenu(EventInfo* info)
{
	sf::Vector2i mousePos = info->detail.mouse;

	float halfX = m_buttonSize.x / 2.0f;
	float halfY = m_buttonSize.y / 2.0f;
	for (uint32 i = 0; i < 3; ++i)
	{
		if (mousePos.x >= m_rects[i].getPosition().x - halfX && mousePos.x < m_rects[i].getPosition().x + halfX
			&&
			mousePos.y >= m_rects[i].getPosition().y - halfY && mousePos.y < m_rects[i].getPosition().y + halfY)
		{
			m_selectIndex = i;
			m_selectRect.setPosition(m_buttonPos.x, m_buttonPos.y + m_selectIndex * (m_buttonSize.y + m_buttonPadding));
		}
	}
}

void StateMainManu::draw()
{
	sf::RenderWindow* window = m_stateManager->getContext()->window->getWindow();
	window->draw(m_text);
	for (uint32 i = 0; i < 3; ++i)
	{
		window->draw(m_rects[i]);
	}
	window->draw(m_selectRect);
	for (uint32 i = 0; i < 3; ++i)
	{
		window->draw(m_labels[i]);
	}
}


void StateMainManu::update(const sf::Time& delta)
{

}