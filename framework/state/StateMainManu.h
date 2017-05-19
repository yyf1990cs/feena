#pragma once
#include "framework\state\ListenerState.h"
class StateMainManu :
	public ListenerState
{
public:
	StateMainManu(StateManager* manager);
	virtual ~StateMainManu();

	virtual void onCreate() override;
	virtual void onDestroy() override;

	virtual void activate() override;
	virtual void deactivate() override;

	virtual void update(const sf::Time& delta) override;
	virtual void draw() override;

private:
	void mouseClick(EventInfo* detail);
	void JoystickClick(EventInfo* detail);
	void mouseHoverMenu(EventInfo* detail);
	void onMenuSelected();

private:
	sf::Text m_text;
	sf::Font m_font;
	
	sf::Vector2f m_buttonSize;
	sf::Vector2f m_buttonPos;
	uint32 m_buttonPadding;

	std::array<sf::RectangleShape, 3> m_rects;
	std::array<sf::Text,3> m_labels;
	sf::RectangleShape m_selectRect;
	uint8 m_selectIndex;
};

