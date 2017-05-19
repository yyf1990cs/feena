#include "stdafx.h"
#include "Unit.h"


Unit::Unit(const GameObjInfo& info)
	:GameObject(info)
	, m_skeletonData(nullptr)
	, m_skeleton(nullptr)
	, m_atlas(nullptr)
	, m_isJumping(false)
	, m_faceLeft(false)
	, m_jumpInitFaceLeft(false)
	, m_faceChanged(false)
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


	loadAssets();
}


Unit::~Unit()
{
	SkeletonData_dispose(m_skeletonData);
	Atlas_dispose(m_atlas);
	delete m_skeleton;
}


bool Unit::tick(float delta)
{
	//更新骨骼动画
	m_skeleton->update(delta);
	//更新上一帧计算完毕的位置
	applyPosition();

	//进入下一帧预期的位置,返回true后可能会被WORLD矫正
	return GameObject::tick(delta);
}

void Unit::jump()
{
	if (m_body.getLand() != nullptr)
	{
		m_isJumping = true;
		m_jumpInitFaceLeft = m_faceLeft;
		m_body.addNatureValocity(sf::Vector2i(0, -JUMP_RISE_SPEED));
		m_body.setInertiaValue(m_body.getControlValue());
	}
	//AnimationState_setAnimationByName(m_skeleton->state, 0, "Jump", true);
}

void Unit::setHorizontal(CONTROLLER_DIRECTION direction)
{
	if (m_horizontalDirection != direction)
	{
		horizontalChanged = true;
		m_horizontalDirection = direction;
	}
}

void Unit::setVertical(CONTROLLER_DIRECTION direction)
{

}


void Unit::setLand(const Edge* edge)
{
	if (m_isJumping)
	{
		m_isJumping = false;
	}
	GameObject::setLand(edge);
}


////////////////////////////////PRIVATE//////////////////////////////////////////
void Unit::loadAssets()
{
	m_atlas = Atlas_createFromFile("./resource/spine/xiaoheiren.atlas", 0);
	m_skeletonData = spine::SkeletonDrawable::readSkeletonBinaryData("./resource/spine/xiaoheiren.skel", m_atlas, 0.5f);

	m_skeleton = new spine::SkeletonDrawable(m_skeletonData);
	m_skeleton->timeScale = 1;
	

	AnimationState_setAnimationByName(m_skeleton->state, 0, "animation", true);
	//AnimationState_addAnimationByName(m_skeleton->state, 1, "gungrab", false, 2);
	m_inited = true;
	m_rendererList.emplace_back(m_skeleton);
}

void Unit::applyPosition()
{
	if (!m_inited)
		return;
	Skeleton* skeleton = m_skeleton->skeleton;
	skeleton->x = (float)m_root.x/ CENTI_PIXEL;
	skeleton->y = (float)m_root.y/ CENTI_PIXEL;

	int ctrlValue = m_body.getControlValue();
	bool tempFace(m_faceLeft);
	if (ctrlValue < 0)
	{
		tempFace = true;
	}
	else if (ctrlValue > 0)
	{
		tempFace = false;
	}

	if (tempFace != m_faceLeft)
	{
		m_faceChanged = true;
		m_faceLeft = tempFace;
		m_skeleton->skeleton->flipX = m_faceLeft;

		if (m_isJumping)
		{
			m_body.setInertiaValue(m_body.getInertiaValue() / 2);
			m_faceChanged = false;
		}
	}

	

	Skeleton_updateWorldTransform(skeleton);
	m_shape.setPosition(m_root.x / CENTI_PIXEL, m_root.y / CENTI_PIXEL);
}