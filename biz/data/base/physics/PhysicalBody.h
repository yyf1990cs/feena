#pragma once
#include "PhysicalDefine.h"

class PhysicalBody
{
public:
	PhysicalBody(const std::vector<sf::Vector2i>& polygon);
	virtual ~PhysicalBody();

	bool tick(float delta);// = true

	void addNatureValocity(const sf::Vector2i v);
	void clearNatureValocity(const sf::Vector2i v);
	void removeNatureValocity(const sf::Vector2i v);
	void move(const sf::Vector2i v);
	void move(int32 x, int32 y);
	void moveTo(const sf::Vector2i v);
	void moveTo(int32 x, int32 y);
	//void setAABB(const sf::Vector2u& size);

	const AABB& getAABB() const { return m_aabb; }
	inline const sf::Vector2i getRoot() const { return m_root; }
	const sf::Vector2i& getNatureValocity() const { return m_natureValocity; }
	const sf::Vector2i getControlValocity() const;
	void setControlValue(int32 v);
	const sf::Vector2i& getLastValocity() const { return m_lastTickLinearValocity; }
	const int32& getAngularValocity() const { return m_angularVelocity; }
	const sf::Vector2i& getMechanismValocity() const { return m_mechanismValocity; }
	const void setMechanismValocity(const sf::Vector2i val) { m_mechanismValocity = val; }
	const ConvexPolygon& getPolygon() const { return m_polygon; }
	const int32 getControlValue() const { return m_contorlValue; }
	const void setInertiaValue(int32 val) { m_inertiaValue = val; }
	const int32 getInertiaValue() const { return m_inertiaValue; }

	const Edge* getLand() const { return m_land; }
	void setLand(const Edge* val){ m_land = val; };
	void setLandStable(bool val){ m_isStableLand = val; }
	bool getLandStable() const { return m_isStableLand; }

private:
	void refreshAABB();

private:
	sf::Vector2i m_root;
	AABB m_aabb;
	int32 m_angularVelocity;
	int32 m_contorlValue;
	int32 m_inertiaValue;//跳跃中的水平惯性速度，左右摇摆则速度减半，碰撞到物体则减为0
	sf::Vector2i m_natureValocity;
	sf::Vector2i m_mechanismValocity;
	sf::Vector2i m_lastTickLinearValocity;
	ConvexPolygon m_polygon;
	bool m_blockControl;
	const Edge* m_land;
	bool m_isStableLand;
	bool m_useNatrueTick;
};

