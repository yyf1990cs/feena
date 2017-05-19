#pragma once
#include "framework/state/StateDefines.h"

// struct EventInfo
// {
// 	EventInfo(){ code = 0; }
// 	EventInfo(int val) { code = val; }
// 
// 	union
// 	{
// 		int code;
// 	};
// };
// 
// using Events = std::vector < std::pair<sf::Event::EventType, EventInfo> > ;


struct EventInfo
{
	EventInfo(const std::string& bindName)
		:name(bindName)
	{
		clear();
	}
	void bind(const sf::Event::EventType& targetType, const int32& targetCode)
	{
		type = targetType;
		code = targetCode;
	}

	void clear()
	{
		detail.size = sf::Vector2i(0, 0);
		detail.textEntered = 0;
		detail.mouse = sf::Vector2i(0, 0);
		detail.mouseWheelDelta = 0;
		detail.code = -1;
	}



	std::string name;
	sf::Event::EventType type;
	int32 code;
	//data
	struct EventDataDetail
	{
		sf::Vector2i size;
		uint32 textEntered;
		sf::Vector2i mouse;
		int32 mouseWheelDelta;
		int32 code;
	};
	EventDataDetail detail;
};

// struct Binding
// {
// 	Binding(const std::string& name)
// 		:name(name)
// 		, data(name)
// 		, count(0)
// 	{
// 
// 	}
// 
// 	void bind(const sf::Event::EventType& type, const int32& code)
// 	{
// 		details.type = type;
// 		details.code = code;
// 	}
// 
// 	std::string name;
// 	int32 count; //count of event that are "happening".
// 
// 	EventData data;
// };


using EventMap = std::unordered_map < std::string, EventInfo* >;
using StateEventMap = std::array < EventMap, GAME_STATE_ARRAY_SIZE >;

template<typename T>
struct EventCallback
{
	using Function = void(T::*)();

	std::string m_name;
	T* callbackInstance; //pointer to instance
	void(T::*_callback)();
	void call()
	{
		callbackInstance->*_callback();
	}
};


using EventCallbackMap = std::unordered_map < std::string, std::function<void(EventInfo*)> >;
using StateEventCallbackMap = std::array < EventCallbackMap, GAME_STATE_ARRAY_SIZE >;