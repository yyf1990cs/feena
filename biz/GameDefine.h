#pragma once

enum GAME_OBJECT_TYPE
{
	GAME_OBJECT_NONE = 0,
	GAME_OBJECT_BLOCK = 1,
	GAME_OBJECT_CHAR = 2,
};

enum CONTROLLER_DIRECTION
{
	HORIZONTAL_NONE,
	HORIZONTAL_LEFT,
	HORIZONTAL_RIGHT,
	VERTICAL_NONE,
	VERTICAL_UP,
	VERTICAL_DOWN,
};


struct GameObjInfo
{
	GameObjInfo()
	//	:polygon({ { 0, 0 }, { 50 * CENTI_PIXEL, 100 * CENTI_PIXEL }, { -50 * CENTI_PIXEL, 100 * CENTI_PIXEL } })
	{
	}

	GAME_OBJECT_TYPE type;
	std::vector<sf::Vector2i> polygon;
};