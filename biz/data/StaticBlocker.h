#pragma once
#include "biz\data\base\GameObject.h"
class StaticBlocker :
	public GameObject
{
public:
	using Ptr = std::shared_ptr < StaticBlocker >;
public:
	StaticBlocker(const GameObjInfo info);
	virtual ~StaticBlocker();

	virtual bool tick(float delta) override;

private:
	sf::ConvexShape m_shape;
};

