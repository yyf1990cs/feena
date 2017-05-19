#include "stdafx.h"
#include "StaticBlocker.h"


StaticBlocker::StaticBlocker(const GameObjInfo info)
	:GameObject(info)
{
	m_shape.setFillColor(sf::Color(randomU(0xFFFFFF) << 8 | 0xFF));
	uint32 size = m_body.getPolygon().size();
	m_shape.setPointCount(size);
	for (uint32 i = 0; i < size; ++i)
	{
		const sf::Vector2i& v = m_body.getPolygon().points[i];
		m_shape.setPoint(i, cp2p(v));
	}

	m_rendererList.emplace_back(&m_shape);
}


StaticBlocker::~StaticBlocker()
{
}

bool StaticBlocker::tick(float delta)
{
	//更新上一帧计算完毕的位置
	m_shape.setPosition((float)m_root.x / CENTI_PIXEL, (float)m_root.y / CENTI_PIXEL);
	//进入下一帧预期的位置,返回true后可能会被WORLD矫正
	return GameObject::tick(delta);
}