#pragma once
#include "biz/data/base/physics/PhysicalBody.h"

class GameObject
{
public:
	using Ptr = std::shared_ptr < GameObject > ;

public:
	GameObject(const GameObjInfo& info);
	virtual ~GameObject();

	virtual bool tick(float delta);
	virtual void setLand(const Edge* edge);
	void setLandStable(bool val){ m_body.setLandStable(val); }
	bool getLandStable() const { return m_body.getLandStable(); }
	virtual const std::vector<sf::Drawable*>& getRendererList() const { return m_rendererList; };

	PhysicalBody& getBody() { return m_body; }
	const PhysicalBody& getBody() const { return m_body; }

	const GAME_OBJECT_TYPE& getType() const { return m_type; }

	const sf::Vector2i& getRoot() const { return m_root; }

	void move(const sf::Vector2i v) { moveTo(m_root + v); };
	void move(int32 x, int32 y) { moveTo(m_root.x + x, m_root.y + y); };
	void moveTo(sf::Vector2i v) { m_root = v; m_body.moveTo(v); }
	void moveTo(int32 x, int32 y) { m_root.x = x; m_root.y = y; m_body.moveTo(x, y); }
protected:
	GAME_OBJECT_TYPE m_type;
	PhysicalBody m_body;
	sf::Vector2i m_root;
	std::vector<sf::Drawable*> m_rendererList;
};
