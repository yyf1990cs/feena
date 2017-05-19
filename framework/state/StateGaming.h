#pragma once
#include "framework/state/ListenerState.h"
#include "biz/data/GameWorld.h"
#include "biz/data/Hero.h"
#include "biz/data/StaticBlocker.h"

class StateGaming :
	public ListenerState
{
public:
	StateGaming(StateManager* manager);
	virtual ~StateGaming();

	virtual void onCreate() override;
	virtual void onDestroy() override;

	virtual void activate() override;
	virtual void deactivate() override;

	virtual void update(const sf::Time& delta) override;
	virtual void draw() override;

	//listneer
private:
	void onJoystickPressed(EventInfo* info);
	void onJoystickReleased(EventInfo* info);
	void onKeyPressed(EventInfo* info);
	void onKeyReleased(EventInfo* info);

private:
	void saveAndQuit();
	void pause();
	void jump();
	void checkMove();
	void leftBlock();
	void rightBlock();
	void upBlock();
	void downBlock();
	void reset();

private:
	sf::Texture m_diabloTexture;
	sf::Texture m_diabloTexture2;
	sf::Sprite m_diablo;
	//sf::Vector2f m_increment;
	sf::Sprite m_greenDiablo;

	GameWorld* m_world;
	Hero::Ptr m_hero;
	StaticBlocker::Ptr m_blocker;

	sf::Time m_elapsed;

	sf::Sprite m_bg;
};

