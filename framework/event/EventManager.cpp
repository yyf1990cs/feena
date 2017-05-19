#include "stdafx.h"
#include "framework/event/EventManager.h"

enum
{
	PRECISION_OFFSET = 3,
	RT_LT_LOWER_BOUND = 50,
	RT_LT_UPPER_BOUND = 50,
	STICK_LOWER_BOUND = 40,
	STICK_UPPER_BOUND = 40,
};

std::array<std::pair<uint8, uint8>, sf::Joystick::Axis::ArraySize> EventManager::m_joystickAxisToKey =
{
	std::make_pair<uint8, uint8>(sf::Joystick::KeyCode::LStick_Right, sf::Joystick::KeyCode::LStick_Left),	///< The X axis
	std::make_pair<uint8, uint8>(sf::Joystick::KeyCode::LStick_Down, sf::Joystick::KeyCode::LStick_Up),		///< The Y axis
	std::make_pair<uint8, uint8>(sf::Joystick::KeyCode::LT, sf::Joystick::KeyCode::RT),						///< The Z axis
	std::make_pair<uint8, uint8>(sf::Joystick::KeyCode::RStick_Down, sf::Joystick::KeyCode::RStick_Up),	///< The R axis
	std::make_pair<uint8, uint8>(sf::Joystick::KeyCode::RStick_Right, sf::Joystick::KeyCode::RStick_Left),		///< The U axis
	std::make_pair<uint8, uint8>(sf::Joystick::KeyCode::Unknow_key1, sf::Joystick::KeyCode::Unknow_key2),	///< The V axis
	std::make_pair<uint8, uint8>(sf::Joystick::KeyCode::Arrow_Right, sf::Joystick::KeyCode::Arrow_Left),	///< The X axis of the point-of-view hat
	std::make_pair<uint8, uint8>(sf::Joystick::KeyCode::Arrow_Up, sf::Joystick::KeyCode::Arrow_Down)		///< The Y axis of the point-of-view hat
};

EventManager::EventManager()	
	:m_hasFocus(true)
{
	m_joystickKeyPressed.fill(false);
	m_joystickKeyStateChanged.fill(false);
	m_KeyPressed.fill(false);
	m_keyStateChanged.fill(false);
	m_joystickAxisBound.fill(std::make_pair(STICK_LOWER_BOUND, STICK_UPPER_BOUND));
	m_joystickAxisBound[sf::Joystick::Axis::Z] = std::make_pair(RT_LT_LOWER_BOUND, RT_LT_UPPER_BOUND);
	loadBindings();
}


EventManager::~EventManager()
{
	for (auto& events : m_eventMap)
	{
		for (auto& it : events)
		{
			delete it.second;
			it.second = nullptr;
		}
	}
}


bool EventManager::addEventInfo(EventInfo* data)
{
	EventMap& events = m_eventMap[m_currentState];
	if (events.find(data->name) != events.cend())
	{
		return false;
	}

	return events.emplace(data->name, data).second;
}

bool EventManager::removeEventInfo(std::string name)
{
	EventMap& events = m_eventMap[m_currentState];
	auto it = events.find(name);
	if (it == events.cend())
	{
		return false;
	}

	delete it->second;
	events.erase(it);
	return true;
}

bool EventManager::removeCallback(const std::string& name)
{
	auto it = m_callbackMap[m_currentState].find(name);
	if (it == m_callbackMap[m_currentState].cend())
		return false;

	m_callbackMap[m_currentState].erase(it);
	return true;
}

sf::Vector2i EventManager::getMousePos(sf::RenderWindow* window /* = nullptr */)
{
	return window ? sf::Mouse::getPosition(*window) : sf::Mouse::getPosition();
}


void EventManager::handleEvent(sf::Event& inputEvt)
{
	handleStateEvent(GAME_STATE_NONE, inputEvt);
	handleStateEvent(m_currentState, inputEvt);
}

void EventManager::handleStateEvent(uint8 state, sf::Event& inputEvt)
{
	//handling SFML event
	EventMap& events = m_eventMap[state];
	for (auto& it : events)
	{
		EventInfo* evtInfo = it.second;
		if (inputEvt.type != sf::Event::EventType::JoystickMoved)
		{//common event handler
			if (evtInfo->type != inputEvt.type)
				continue;
			if (inputEvt.type == sf::Event::EventType::KeyPressed)
			{
				if (inputEvt.key.code < 0)
					return;
				if (m_KeyPressed[inputEvt.key.code])
					continue;
				if (evtInfo->code == -1 || evtInfo->code == inputEvt.key.code)
				{
					m_keyStateChanged[inputEvt.key.code] = true;
					evtInfo->detail.code = inputEvt.key.code;
					invokeCallback(state, evtInfo);
					continue;
				}
			}
			else if (inputEvt.type == sf::Event::EventType::KeyReleased)
			{
				if (inputEvt.key.code < 0)
					return;
				if (!m_KeyPressed[inputEvt.key.code])
					continue;
				if (evtInfo->code == -1 || evtInfo->code == inputEvt.key.code)
				{
					m_keyStateChanged[inputEvt.key.code] = true;
					evtInfo->detail.code = inputEvt.key.code;
					invokeCallback(state, evtInfo);
					continue;
				}
			}
			else if (inputEvt.type == sf::Event::EventType::JoystickButtonPressed || inputEvt.type == sf::Event::EventType::JoystickButtonReleased)
			{
				if (evtInfo->code == -1 || evtInfo->code == inputEvt.joystickButton.button)
				{
					evtInfo->detail.code = inputEvt.joystickButton.button;
					invokeCallback(state, evtInfo);
					continue;
				}
			}
			else if (inputEvt.type == sf::Event::EventType::MouseButtonPressed || inputEvt.type == sf::Event::EventType::MouseButtonReleased)
			{
				if (evtInfo->code == -1 || evtInfo->code == inputEvt.mouseButton.button)
				{
					evtInfo->detail.mouse.x = inputEvt.mouseButton.x;
					evtInfo->detail.mouse.y = inputEvt.mouseButton.y;
					evtInfo->detail.code = inputEvt.mouseButton.button;
					invokeCallback(state, evtInfo);
					continue;
				}
			}
			else if (inputEvt.type == sf::Event::EventType::MouseMoved)
			{
				evtInfo->detail.mouse.x = inputEvt.mouseMove.x;
				evtInfo->detail.mouse.y = inputEvt.mouseMove.y;
				invokeCallback(state, evtInfo);
				continue;
			}
			else
			{
				if (inputEvt.type == sf::Event::EventType::MouseWheelMoved || inputEvt.type == sf::Event::EventType::MouseWheelScrolled)
				{
					evtInfo->detail.mouseWheelDelta = inputEvt.mouseWheel.delta;
				}
				else if (inputEvt.type == sf::Event::EventType::Resized)
				{
					evtInfo->detail.size.x = inputEvt.size.width;
					evtInfo->detail.size.y = inputEvt.size.height;
				}
				else if (inputEvt.type == sf::Event::EventType::TextEntered)
				{
					evtInfo->detail.textEntered = inputEvt.text.unicode;
				}
				invokeCallback(state, evtInfo);
				continue;
			}
		}
	}

	for (uint32 changeIndex = 0; changeIndex < sf::Keyboard::KeyCount; ++changeIndex)
	{
		if (m_keyStateChanged[changeIndex])
		{
			m_KeyPressed[changeIndex] = !m_KeyPressed[changeIndex];
			m_keyStateChanged[changeIndex] = false;
		}
	}
}

void  EventManager::update()
{
	if (!m_hasFocus)
		return;
	bool changed(false);
	for (uint8 i = 0; i < sf::Joystick::Axis::ArraySize; ++i)
	{
		if (i == sf::Joystick::Axis::V)
			continue;
		uint8 joyKeyCode(sf::Joystick::KeyCode::JoyKey_ArraySize);
		float position = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis(i));
		if (_abs(position) < PRECISION_OFFSET)
			position = 0;
		if (position >= 0)
		{
			joyKeyCode = m_joystickAxisToKey[i].first;
			float val = _abs(position);
			if (!m_joystickKeyPressed[joyKeyCode] && val > m_joystickAxisBound[i].second)
			{
				changed = m_joystickKeyStateChanged[joyKeyCode] = true;
				m_joystickKeyPressed[joyKeyCode] = true;
			}
			else if (m_joystickKeyPressed[joyKeyCode] && val < m_joystickAxisBound[i].first)
			{
				changed = m_joystickKeyStateChanged[joyKeyCode] = true;
				m_joystickKeyPressed[joyKeyCode] = false;
			}
		}
		if (position <= 0)
		{
			joyKeyCode = m_joystickAxisToKey[i].second;
			float val = _abs(position);
			if (!m_joystickKeyPressed[joyKeyCode] && val > m_joystickAxisBound[i].second)
			{
				changed = m_joystickKeyStateChanged[joyKeyCode] = true;
				m_joystickKeyPressed[joyKeyCode] = true;
			}
			else if (m_joystickKeyPressed[joyKeyCode] && val < m_joystickAxisBound[i].first)
			{
				changed = m_joystickKeyStateChanged[joyKeyCode] = true;
				m_joystickKeyPressed[joyKeyCode] = false;
			}
		}
	}
	
	
	if (!changed)
		return;

	handleStateUpdate(GAME_STATE_NONE);
	handleStateUpdate(m_currentState);

	array_clear(m_joystickKeyStateChanged);
}

void EventManager::handleStateUpdate(uint8 state)
{
	bool needCheckStick(false);
	EventMap& events = m_eventMap[state];

	for (uint8 code = sf::Joystick::LT; code < sf::Joystick::JoyKey_ArraySize; ++code)
	{
		if (m_joystickKeyStateChanged[code])
		{
			for (auto& it : events)
			{
				EventInfo* evtInfo = it.second;
				switch (evtInfo->type)
				{
					case sf::Event::JoystickButtonPressed:
						{
							if (m_joystickKeyPressed[code] && (evtInfo->code == -1 || evtInfo->code == code))
							{
								evtInfo->detail.code = code;
								invokeCallback(state, evtInfo);
							}
						}
						break;
					case sf::Event::JoystickButtonReleased:
						{
							if (!m_joystickKeyPressed[code] && (evtInfo->code == -1 || evtInfo->code == code))
							{
								evtInfo->detail.code = code;
								invokeCallback(state, evtInfo);
							}
						}
						break;
				}
			}
		}
	}

}

void EventManager::invokeCallback(uint8 state, EventInfo* evtInfo)
{
	auto callItr = m_callbackMap[state].find(evtInfo->name);
	if (callItr != m_callbackMap[state].end())
	{
		callItr->second(evtInfo);
	}
	evtInfo->clear();
}


void EventManager::setCurrentState(const uint8& state)
{
	m_currentState = state;
}


bool EventManager::isKeyPressed(sf::Keyboard::Key code) const 
{
	if (code != sf::Keyboard::Unknown)
		return !m_keyStateChanged[code] ? m_KeyPressed[code] : !m_KeyPressed[code];
	return false;
}
bool EventManager::isJoystickPressed(sf::Joystick::KeyCode code) const
{ 
	if (code > 0 && code < sf::Joystick::JoyKey_ArraySize)
		return !m_joystickKeyStateChanged[code] ? m_joystickKeyPressed[code] : !m_joystickKeyPressed[code];
	return false;
}

////////////////////////////////PRIVATE//////////////////////////////////////////

void EventManager::loadBindings()
{
	std::string delimiter = ":";

	std::ifstream bindings;
	bindings.open("./config/joystick.config");
	if (!bindings.is_open())
	{
		std::cout << "Failed loading key config." << std::endl;
		return;
	}

	std::string line;
	while (std::getline(bindings, line))
	{
		string_trim(line);
		if (line.size() == 0 || 35 == line.at(0))
		{
			continue;
		}
		std::stringstream keystream(line);
		std::string callbackName;
		keystream >> callbackName;
		EventInfo* evtInfo = new EventInfo(callbackName);
		while (!keystream.eof())
		{
			std::string keyval;
			keystream >> keyval;
			int start = 0;
			int end = keyval.find(delimiter);
			if (end == std::string::npos)
			{
				delete evtInfo;
				evtInfo = nullptr;
				break;
			}

			sf::Event::EventType type = sf::Event::EventType(stoi(keyval.substr(start, end - start)));
			int code = stoi(keyval.substr(end + delimiter.length(), keyval.find(delimiter, end + delimiter.length())));
			evtInfo->bind(type, code);
		}

		if (evtInfo != nullptr && !addEventInfo(evtInfo))
		{
			delete evtInfo;
		}
		evtInfo = nullptr;
	}
	bindings.close();
}