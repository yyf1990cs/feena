#pragma once
#include "BaseState.h"

struct SharedContext;
class StateManager
{
public:
	StateManager(SharedContext* shared);
	~StateManager();

	void update(const sf::Time& time);
	void draw();

	void processRequests();

	SharedContext* getContext() { return m_shared; };
	bool hasState(const uint8& state) const;

	void switchTo(const uint8& state);
	void remove(const uint8& state);

private:
	void createState(const uint8& state);
	void removeState(const uint8& state);

	template<typename T>
	void registerState(const uint8& state);

private:
	SharedContext* m_shared;
	StateContainer m_states;
	StateTypeContainer m_toRemove;
	StateFactory m_stateFactory;
};

template<typename T>
void StateManager::registerState(const uint8& state)
{
	m_stateFactory[state] = [this]()->BaseState*
	{
		return new T(this);
	};
}