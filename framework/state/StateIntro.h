#pragma once
#include "framework\state\ListenerState.h"

class StateIntro :
	public ListenerState
{
public:
	StateIntro(StateManager* manager);
	virtual ~StateIntro();

	virtual void onCreate() override;
	virtual void onDestroy() override;

	virtual void activate() override;
	virtual void deactivate() override;

	virtual void update(const sf::Time& delta) override;
	virtual void draw() override;

	void Continue(EventInfo* detals);
	
private:
	sf::Texture m_introTexture;
	sf::Sprite m_introSprite;
	sf::Text m_text;
	sf::Font m_font;
	float m_timePassed;
};

