#pragma once

class StateManager;
class BaseState
{
public:
	BaseState(StateManager* manager);
	virtual ~BaseState();

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void activate() = 0;
	virtual void deactivate() = 0;

	virtual void update(const sf::Time& delta) = 0;
	virtual void draw() = 0;

	void setTransparent(const bool& val) { m_transparent = val; };
	bool isTransparent() const { return m_transparent; }

	void setTranscendent(const bool& val) { m_transcendent = val; }
	bool isTranscendent() const { return m_transcendent; }

	StateManager* getStateManager() { return m_stateManager; }

protected:
	StateManager* m_stateManager;
	EventManager* m_eventManager;

private:
	bool m_transparent;
	bool m_transcendent;
};


using StateContainer = std::vector < std::pair<uint8, BaseState*> >;
using StateTypeContainer = std::vector < uint8 >;
using StateFactory = std::unordered_map < uint8, std::function<BaseState*(void)> >;