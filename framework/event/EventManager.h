#pragma once
#include "framework/event/EventDefines.h"

class EventManager
{

public:
	EventManager();
	~EventManager();

	bool addEventInfo(EventInfo* info);
	bool removeEventInfo(std::string name);

	void setFocus(const bool& val) { m_hasFocus = val; };

	template<typename T>
	void addCallback(const std::string& name, void(T::*func)(EventInfo*), T* instance)
	{
		auto temp = std::bind(func, instance, std::placeholders::_1);
		m_callbackMap[m_currentState].emplace(name, temp); //emplace返回值 pair<该key对应的iterator，true/false（该key已经存在）>
	}

	//注册一个匿名事件侦听
	template<typename T>
	void addCallback(const sf::Event::EventType& eventType, const uint32& code, void(T::*func)(EventInfo*), T* instance)
	{
		std::string callbackName = ntstr(eventType) + "_" + ntstr(code) + ":" + ntstr(func);

		EventInfo* eventInfo = new EventInfo(callbackName);
		eventInfo->bind(eventType, code);
		addEventInfo(eventInfo);

		auto temp = std::bind(func, instance, std::placeholders::_1);
		m_callbackMap[m_currentState].emplace(callbackName, temp);
	}

	bool removeCallback(const std::string& name);

	template<typename T>
	bool removeCallback(const sf::Event::EventType& eventType, const uint32& code, void(T::*func)(EventInfo*))
	{
		std::string callbackName = ntstr(eventType) + "_" + ntstr(code) + ":" + ntstr(func);
		return removeCallback(callbackName);
	}
	void handleEvent(sf::Event& evt);
	void update();

	void setCurrentState(const uint8& state);

	sf::Vector2i getMousePos(sf::RenderWindow* window = nullptr);
	bool isKeyPressed(sf::Keyboard::Key code) const;
	bool isJoystickPressed(sf::Joystick::KeyCode code) const;

private:
	void loadBindings();
	inline void handleStateEvent(uint8 state, sf::Event& inputEvt);
	inline void handleStateUpdate(uint8 state);
	inline void invokeCallback(uint8 state, EventInfo* evtData);

private:
	StateEventMap m_eventMap;
	StateEventCallbackMap m_callbackMap;
	
	bool m_hasFocus;
	uint8 m_currentState;

	std::array<std::pair<uint8, uint8>, sf::Joystick::Axis::ArraySize> m_joystickAxisBound;
	static std::array<std::pair<uint8, uint8>, sf::Joystick::Axis::ArraySize> m_joystickAxisToKey;
	std::array<bool, sf::Keyboard::KeyCount> m_KeyPressed;
	std::array<bool, sf::Keyboard::KeyCount> m_keyStateChanged;
	std::array<bool, sf::Joystick::KeyCode::JoyKey_ArraySize> m_joystickKeyPressed;
	std::array<bool, sf::Joystick::KeyCode::JoyKey_ArraySize> m_joystickKeyStateChanged;
};

