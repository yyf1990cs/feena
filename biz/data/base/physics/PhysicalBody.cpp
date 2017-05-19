#include "stdafx.h"
#include "PhysicalBody.h"


PhysicalBody::PhysicalBody(const std::vector<sf::Vector2i>& polygon)
	:m_land(nullptr)
	, m_polygon(polygon)
	, m_root(0,0)
	, m_angularVelocity(0)
	, m_natureValocity(0,0)
	, m_contorlValue(0)
	, m_lastTickLinearValocity(0,0)
	, m_blockControl(false)
	, m_useNatrueTick(false)
	, m_isStableLand(false)
	, m_inertiaValue(0)
{
	FORCE_CRASH(m_polygon.size() < 3);
	refreshAABB();
}


PhysicalBody::~PhysicalBody()
{
}


bool PhysicalBody::tick(float delta)
{
	m_lastTickLinearValocity = m_useNatrueTick ? (m_natureValocity + m_mechanismValocity) *delta : (getControlValocity() + m_mechanismValocity) * delta;
	m_lastTickLinearValocity.x += int(m_inertiaValue * delta);
	m_useNatrueTick = !m_useNatrueTick;
	if (m_lastTickLinearValocity.x != 0 || m_lastTickLinearValocity.y != 0)
	{
		move(m_lastTickLinearValocity);
		return true;
	}
	return false;
}


void PhysicalBody::addNatureValocity(const sf::Vector2i v)
{
	m_natureValocity += v;
	if (m_natureValocity.y < 0)
	{
		m_land = nullptr;
		m_isStableLand = false;
	}
}

void PhysicalBody::setControlValue(int32 v)
{
	m_contorlValue = v;
}

void PhysicalBody::clearNatureValocity(const sf::Vector2i v)
{
	m_natureValocity.x = 0;
	m_natureValocity.y = 0;
}

void PhysicalBody::removeNatureValocity(const sf::Vector2i v)
{
	m_natureValocity -= v;
}

void PhysicalBody::move(const sf::Vector2i v)
{
	m_aabb += v;
	m_root += v;

	for (auto& p : m_polygon.points)
	{
		p += v;
	}

	if (m_land != nullptr)
	{
		const Edge* leftEdge = nullptr;
		const Edge* rightEdge = nullptr;
		if (m_land->from->x > m_aabb.lowerBound.x || m_land->to->x < m_aabb.upperBound.x) //地线的两头
		{
			if (true == m_isStableLand)
			{
				m_isStableLand = false;
			}
		}
		else if (false == m_isStableLand)
		{
			m_isStableLand = true;
		}
	}
}

void PhysicalBody::move(int32 x, int32 y)
{
	move(sf::Vector2i(x, y));
}

void PhysicalBody::moveTo(const sf::Vector2i v)
{
	move(v - m_root);
}

void PhysicalBody::moveTo(int32 x, int32 y)
{
	moveTo(sf::Vector2i(x, y));
}


const sf::Vector2i PhysicalBody::getControlValocity() const
{
	if (m_land == nullptr)
	{
		return sf::Vector2i(0, 0);
	}
	if (m_contorlValue == 0)
	{
		return sf::Vector2i();
	}
	if (m_isStableLand == false)
	{
		return sf::Vector2i(m_contorlValue, 0);
	}
	double landAngle = m_land->getAngle();
	float landRatio = m_land->getRatio();
	//std::cout << landAngle << std::endl;
	if (landRatio == 0)//平地
	{
		return sf::Vector2i(m_contorlValue, 0);
	}
	else if (landRatio < 0)
	{
		if (m_contorlValue < 0)//往回走的下坡
		{
			return sf::Vector2i(m_contorlValue, int(double(m_contorlValue) * landRatio));
		}
		else//往前走的上坡
		{
			return sf::Vector2i(int(double(m_contorlValue) * cos(landAngle)), int(double(m_contorlValue) * sin(landAngle)));
		}
	}
	else
	{
		if (m_contorlValue < 0)//往回走的上坡
		{
			return sf::Vector2i(int(double(m_contorlValue) * cos(landAngle)), int(double(m_contorlValue) * sin(landAngle)));
		}
		else//往前走的下坡
		{
			return sf::Vector2i(m_contorlValue, int(double(m_contorlValue) * landRatio));
		}
	}
}

////////////////////////////////////////PRIVATE//////////////////////////////////
void PhysicalBody::refreshAABB()
{
	m_aabb.lowerBound = m_polygon.points[0];
	m_aabb.upperBound = m_polygon.points[0];
	for (const sf::Vector2i& v : m_polygon.points)
	{
		if (m_aabb.lowerBound.x > v.x)
			m_aabb.lowerBound.x = v.x;
		if (m_aabb.lowerBound.y > v.y)
			m_aabb.lowerBound.y = v.y;
		if (m_aabb.upperBound.x < v.x)
			m_aabb.upperBound.x = v.x;
		if (m_aabb.upperBound.y < v.y)
			m_aabb.upperBound.y = v.y;
	}
}
