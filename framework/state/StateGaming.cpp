#include "stdafx.h"
#include "StateGaming.h"
#include "biz/data/Hero.h"


StateGaming::StateGaming(StateManager* manager)
	:ListenerState(manager)
	, m_world(nullptr)
{
}


StateGaming::~StateGaming()
{
}


void StateGaming::onCreate()
{
	m_diabloTexture.loadFromFile("./resource/diablo.png");
	m_diabloTexture.setSmooth(true);
	m_diablo.setTexture(m_diabloTexture);
	sf::Vector2u size = m_diabloTexture.getSize();
	m_diablo.setOrigin((float)size.x / 2, (float)size.y);
	m_diablo.setPosition(200, 100);

	m_diabloTexture2.loadFromFile("./resource/diablo.png");
	m_diabloTexture2.setSmooth(false);
	m_greenDiablo.setTexture(m_diabloTexture2);
	//m_greenDiablo.setColor(sf::Color(0x00FF00FF));
	m_greenDiablo.setOrigin((float)size.x / 2, (float)size.y);
	m_greenDiablo.setPosition(100, 100);

	sf::Texture *texture = new sf::Texture();
	texture->loadFromFile("./resource/dust.png");
	m_bg = sf::Sprite(*texture);

	m_world = new GameWorld();

	GameObjInfo heroInfo;
	heroInfo.polygon = { { -35 * CENTI_PIXEL, -100 * CENTI_PIXEL }, { 35 * CENTI_PIXEL, -100 * CENTI_PIXEL }, { 35 * CENTI_PIXEL, 0 }, { -35 * CENTI_PIXEL, 0 } };
	heroInfo.type = GAME_OBJECT_CHAR;
	m_hero = std::make_shared<Hero>(heroInfo);
	m_hero->moveTo(100 * CENTI_PIXEL, 0 * CENTI_PIXEL);
	m_world->addObject(m_hero);

	GameObjInfo blockInfo;
	blockInfo.polygon = { { 0, 200 * CENTI_PIXEL }, { 100 * CENTI_PIXEL, 0 }, { 600 * CENTI_PIXEL, 500 * CENTI_PIXEL }, { 0, 500 * CENTI_PIXEL } };
	blockInfo.type = GAME_OBJECT_BLOCK;
	m_blocker = std::make_shared<StaticBlocker>(blockInfo);
	m_blocker->moveTo(0 * CENTI_PIXEL, 100 * CENTI_PIXEL);
	m_world->addObject(m_blocker);

	GameObjInfo block2;
	block2.polygon = { { 0, 0 }, { 400 * CENTI_PIXEL, 70 * CENTI_PIXEL }, {400 * CENTI_PIXEL, 140 * CENTI_PIXEL}, {0, 70 * CENTI_PIXEL} };
	block2.type = GAME_OBJECT_BLOCK;
	StaticBlocker::Ptr bl2 = std::make_shared<StaticBlocker>(block2);
	bl2->moveTo(330 * CENTI_PIXEL, 125 * CENTI_PIXEL);
	m_world->addObject(bl2);
}

void StateGaming::onDestroy()
{
	
}

void StateGaming::activate()
{
	EventManager* evtMgr = m_stateManager->getContext()->eventManager;
	addListener(sf::Event::JoystickButtonPressed, &StateGaming::onJoystickPressed);
	addListener(sf::Event::JoystickButtonReleased, &StateGaming::onJoystickReleased);
	addListener(sf::Event::KeyPressed, &StateGaming::onKeyPressed);
	addListener(sf::Event::KeyReleased, &StateGaming::onKeyReleased);
}
void StateGaming::deactivate()
{
	EventManager* evtMgr = m_stateManager->getContext()->eventManager;
	removeListener(sf::Event::JoystickButtonPressed, &StateGaming::onJoystickPressed);
	removeListener(sf::Event::JoystickButtonReleased, &StateGaming::onJoystickReleased);
}

void StateGaming::update(const sf::Time& delta)
{
	m_elapsed = delta;
	m_world->tick(delta.asSeconds());
}
void StateGaming::draw()
{
	sf::RenderWindow* window = m_stateManager->getContext()->window->getWindow();
	window->draw(m_bg);

	auto sList = m_world->getStaticList();
	for (const auto& obj : sList)
	{
		const auto& list = obj->getRendererList();
		for (const auto* renderer : list)
		{
			window->draw(*renderer);
		}
	}
	auto dList = m_world->getDynamicList();
	for (const auto& obj : dList)
	{
		const auto& list = obj->getRendererList();
		for (const auto* renderer : list)
		{
			window->draw(*renderer);
		}
	}

	window->draw(m_diablo);
	window->draw(m_greenDiablo);
}

void StateGaming::onJoystickPressed(EventInfo* info)
{
	switch (info->detail.code)
	{
		case sf::Joystick::A:
			{
				jump();
			}
			break;
		case sf::Joystick::B:
			{
				
			}
			break;
		case sf::Joystick::JoyKey_X:
			{

			}
			break;
		case sf::Joystick::JoyKey_Y:
			{

			}
			break;
		case sf::Joystick::LB:
			{

			}
			break;
		case sf::Joystick::RB:
			{

			}
			break;
		case sf::Joystick::Back:
			{
				pause();
			}
			break;
		case sf::Joystick::Start:
			{
				reset();
			}
			break;
		case sf::Joystick::LStick:
			{

			}
			break;
		case sf::Joystick::RStick:
			{

			}
			break;
		case sf::Joystick::LT:
			{

			}
			break;
		case sf::Joystick::RT:
			{

			}
			break;
		case sf::Joystick::LStick_Left:
			{
				checkMove();
			}
			break;
		case sf::Joystick::LStick_Right:
			{
				checkMove();
			}
			break;
		case sf::Joystick::LStick_Up:
			{

			}
			break;
		case sf::Joystick::LStick_Down:
			{

			}
			break;
		case sf::Joystick::RStick_Left:
			{

			}
			break;
		case sf::Joystick::RStick_Right:
			{

			}
			break;
		case sf::Joystick::RStick_Up:
			{

			}
			break;
		case sf::Joystick::RStick_Down:
			{

			}
			break;
		case sf::Joystick::Arrow_Left:
			{
				leftBlock();
			}
			break;
		case sf::Joystick::Arrow_Right:
			{
				rightBlock();
			}
			break;
		case sf::Joystick::Arrow_Up:
			{
				upBlock();
			}
			break;
		case sf::Joystick::Arrow_Down:
			{
				downBlock();
			}
			break;
		default:
			break;
	}
}

void StateGaming::onJoystickReleased(EventInfo* info)
{
	switch (info->detail.code)
	{
		case sf::Joystick::LStick_Left:
			{
// 				std::cout << "release LStick_Left" << std::endl;
				checkMove();
			}
			break;
		case sf::Joystick::LStick_Right:
			{
// 				std::cout << "release LStick_Right" << std::endl;
				checkMove();
			}
			break;
		default:
			break;
	}
}

void StateGaming::onKeyPressed(EventInfo* info)
{
	trace_("pressed");
	switch (info->detail.code)
	{
		case sf::Keyboard::A:
			{
				checkMove();
			}
			break;
		case sf::Keyboard::D:
			{
				checkMove();
			}
			break;
		case sf::Keyboard::Space:
			{
				jump();
			}
			break;
	}
}

void StateGaming::onKeyReleased(EventInfo* info)
{
	switch (info->detail.code)
	{
		case sf::Keyboard::A:
			{
				checkMove();
			}
			break;
		case sf::Keyboard::D:
			{
				checkMove();
			}
			break;
	}
}

void StateGaming::leftBlock()
{
	m_blocker->moveTo(m_blocker->getRoot().x - 20 * CENTI_PIXEL, m_blocker->getRoot().y);
}
void StateGaming::rightBlock()
{
	m_blocker->moveTo(m_blocker->getRoot().x + 20 * CENTI_PIXEL, m_blocker->getRoot().y);
}
void StateGaming::upBlock()
{
	m_blocker->moveTo(m_blocker->getRoot().x, m_blocker->getRoot().y - 20 * CENTI_PIXEL);
}
void StateGaming::downBlock()
{
	m_blocker->moveTo(m_blocker->getRoot().x, m_blocker->getRoot().y + 20 * CENTI_PIXEL);
}
void StateGaming::reset()
{
	m_hero->moveTo(100 * CENTI_PIXEL, 0 * CENTI_PIXEL);
}

void StateGaming::saveAndQuit()
{
	m_stateManager->switchTo(GAME_STATE_MAIN_MENU);
}


void StateGaming::pause()
{
	m_stateManager->switchTo(GAME_STATE_PAUSED);
}

void StateGaming::jump()
{
	m_hero->jump();
}

void StateGaming::checkMove()
{
	if (
		(m_eventManager->isJoystickPressed(sf::Joystick::LStick_Left) && !m_eventManager->isJoystickPressed(sf::Joystick::LStick_Right)) ||
		(m_eventManager->isKeyPressed(sf::Keyboard::A) && !m_eventManager->isKeyPressed(sf::Keyboard::D))
		)
	{
		trace_("move left");
		m_hero->getBody().setControlValue(-100 * CENTI_PIXEL);
	}
	else if (
		(!m_eventManager->isJoystickPressed(sf::Joystick::LStick_Left) && m_eventManager->isJoystickPressed(sf::Joystick::LStick_Right)) ||
		(!m_eventManager->isKeyPressed(sf::Keyboard::A) && m_eventManager->isKeyPressed(sf::Keyboard::D))
		)
	{
		trace_("move right");
		m_hero->getBody().setControlValue(100 * CENTI_PIXEL);
	}
	else
	{
		trace_("move stop");
		m_hero->getBody().setControlValue(0);
	}
}

