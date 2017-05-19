#pragma once
#include "biz/data/base/GameObject.h"

class Unit : public GameObject
{
public:
	using Ptr = std::shared_ptr < Unit >;
public:
	Unit(const GameObjInfo& info);
	virtual ~Unit();

	virtual bool tick(float delta) override;
	virtual void setLand(const Edge* edge) override;

	virtual void jump();

	void setHorizontal(CONTROLLER_DIRECTION direction);
	void setVertical(CONTROLLER_DIRECTION direction);

private:
	void loadAssets();
	void applyPosition();
	
protected:
	bool m_inited;
	bool m_isJumping;
	bool m_jumpInitFaceLeft;
	bool m_faceLeft;
	bool m_faceChanged;

	Atlas* m_atlas;
	SkeletonData* m_skeletonData;
	spine::SkeletonDrawable* m_skeleton;
	sf::ConvexShape m_shape;

	CONTROLLER_DIRECTION m_horizontalDirection;
	CONTROLLER_DIRECTION m_verticalDirection;

	bool m_horizontalChanged;
	bool m_verticalChanged;
};

