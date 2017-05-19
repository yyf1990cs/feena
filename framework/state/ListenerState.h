#pragma once
#include "framework/state/StateManager.h"
#include "framework/event/EventManager.h"

class ListenerState :
	public BaseState
{
public:
	ListenerState(StateManager* manager);
	virtual ~ListenerState();

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void activate() = 0;
	virtual void deactivate() = 0;

	virtual void update(const sf::Time& delta) = 0;
	virtual void draw() = 0;

// 	template<typename T>
// 	void addEventListener(const std::string& name, void(T::*func)(EventInfo*))
// 	{
// 		m_eventManager->addCallback(name, func, this);
// 	}
// 	template<typename T>
// 	void addEventListener(const sf::Event::EventType& eventType, void(T::*func)(EventInfo*), uint32 code = -1)
// 	{
// 		auto temp = std::function<void(EventInfo*)>(std::bind(func, this, std::placeholders::_1));
// 		m_eventManager->addCallback<T, extendClassType>(eventType, code, func, temp);
// 	}

// 	void removeEventListener(const std::string& name)
// 	{
// 		m_eventManager->removeCallback(name);
// 	}
// 	template<typename T>
// 	void removeEventListener(const sf::Event::EventType& eventType, void(T::*func)(EventInfo*), uint32 code = -1)
// 	{
// 		m_eventManager->removeCallback<T>(eventType, code , func);
// 	}


};

#define addListener(eType,eFunc) m_eventManager->addCallback(eType,-1,eFunc,this)
#define addCodeListener(eType,eCode,eFunc) m_eventManager->addCallback(eType,eCode,eFunc,this)
#define addNamedListener(eName, eFunc) m_eventManager->addCallback(eName,eFunc,this)

#define removeListener(eType,eFunc) m_eventManager->removeCallback(eType,-1,eFunc)
#define removeCodeListener(eType,eCode,eFunc) m_eventManager->removeCallback(eType,eCode,eFunc)
#define removeNamedListener(eName) m_eventManager->removeCallback(eName)