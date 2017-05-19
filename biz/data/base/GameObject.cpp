#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject(const GameObjInfo& info)
	:m_type(info.type)
	, m_body(info.polygon)
	, m_root(0,0)
{
}


GameObject::~GameObject()
{
}


bool GameObject::tick(float delta)
{
	m_root = m_body.getRoot();
	if (m_type == GAME_OBJECT_CHAR)
		return m_body.tick(delta) || m_body.tick(delta);
	return false;
}

void GameObject::setLand(const Edge* edge)
{
	m_body.setLand(edge);
}
