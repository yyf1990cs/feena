#include "stdafx.h"
#include "Hero.h"


Hero::Hero(const GameObjInfo& info)
	:Unit(info)
{
}


Hero::~Hero()
{
}


bool Hero::tick(float delta)
{
	return Unit::tick(delta);
}