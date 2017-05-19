#pragma once
#include "framework\state\ListenerState.h"

class StatePaused :
	public ListenerState
{
public:
	StatePaused(StateManager* manager);
	virtual ~StatePaused();

	virtual void onCreate() override;
	virtual void onDestroy() override;

	virtual void activate() override;
	virtual void deactivate() override;

	virtual void update(const sf::Time& delta) override;
	virtual void draw() override;

	void unpause(EventInfo* detail);

private:
	sf::Font m_font;
	sf::Text m_text;
	sf::RectangleShape m_rect;
};

