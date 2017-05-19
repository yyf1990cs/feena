#include "stdafx.h"
#include "StateManager.h"
#include "framework/event/EventManager.h"
#include "StateIntro.h"
#include "StateMainManu.h"
#include "StateGaming.h"
#include "StatePaused.h"


StateManager::StateManager(SharedContext* shared)
	:m_shared(shared)
{
	registerState<StateIntro>(GAME_STATE_INTRO);
 	registerState<StateMainManu>(GAME_STATE_MAIN_MENU);
	registerState<StateGaming>(GAME_STATE_GAMING);
	registerState<StatePaused>(GAME_STATE_PAUSED);
}


StateManager::~StateManager()
{
	for (auto& it : m_states)
	{
		it.second->onDestroy();
		delete it.second;
	}
}


void StateManager::draw()
{
	if (m_states.empty())
		return;

	if (m_states.back().second->isTransparent() && m_states.size() > 1) //假如最后一个状态是过度，而且当前有多个状态
	{
		//找到栈中最老的一个过度状态
		auto it = m_states.end();
		--it;
		while (it != m_states.begin())
		{
			if (!it->second->isTransparent())
			{
				break;
			}
			--it;
		}
		for (; it != m_states.end(); ++it)//从这个状态开始从底层向上全部draw一遍
		{
			it->second->draw();
		}
	}
	else
	{
		m_states.back().second->draw();
	}
}

void StateManager::update(const sf::Time& time)
{
	if (m_states.empty())
		return;

	if (m_states.back().second->isTranscendent() && m_states.size() > 1)
	{
		auto it = m_states.end();
		--it;
		while (it != m_states.begin())
		{
			if (!it->second->isTranscendent())
			{
				break;
			}
			--it;
		}
		for (; it != m_states.end(); ++it)
		{
			it->second->update(time);
		}
	}
	else
	{
		m_states.back().second->update(time);
	}
}

bool StateManager::hasState(const uint8& state) const
{
	for (auto it = m_states.cbegin(); it != m_states.cend(); ++it)
	{
		if (it->first == state) //存在，而且不是需要移除的状态
		{
			auto removed = std::find(m_toRemove.cbegin(), m_toRemove.cend(), state);
			if (removed == m_toRemove.end())
			{
				return true;
			}
			return false;
		}
	}
	return false;
}


void StateManager::remove(const uint8& state)
{
	m_toRemove.emplace_back(state);
}

void StateManager::processRequests()
{
	while (m_toRemove.begin() != m_toRemove.end())
	{
		removeState(*m_toRemove.begin());
		m_toRemove.erase(m_toRemove.begin());
	}
}


void StateManager::switchTo(const uint8& state)
{
	m_shared->eventManager->setCurrentState(state);
	for (auto it = m_states.begin(); it != m_states.end(); ++it)
	{
		if (it->first == state)
		{
			m_states.back().second->deactivate();
			uint8 tempStateType = it->first;
			BaseState* tempState = it->second;
			m_states.erase(it);
			m_states.emplace_back(tempStateType, tempState);
			tempState->activate();
			return;
		}
	}

	//没找到该类型
	if (!m_states.empty())
	{
		m_states.back().second->deactivate();
	}
	createState(state);
	m_states.back().second->activate();
}


///////////////////////////////PRIVATAE///////////////////////////////////////////

void StateManager::createState(const uint8& type)
{
	auto newState = m_stateFactory.find(type);
	if (newState == m_stateFactory.end())
		return;

	BaseState* state = newState->second();
	m_states.emplace_back(type,state);
	state->onCreate();
}


void StateManager::removeState(const uint8& type)
{
	for (auto it = m_states.begin(); it != m_states.end(); ++it)
	{
		if (it->first == type)
		{
			it->second->onDestroy();
			delete it->second;
			m_states.erase(it);
			return;
		}
	}
}