#include "stdafx.h"
#include "GameWorld.h"


GameWorld::GameWorld()
	:m_delta(0)
{
}


GameWorld::~GameWorld()
{
}


void GameWorld::tick(float delta)
{
	uint32 ms = get_ms_time();
	m_delta = delta;
	static sf::Vector2i gravityValocity = sf::Vector2i(0, GRAVITY_ACC);
	bool changed;
	for (GameObject::Ptr& gameObject : m_staticList)
	{
		gameObject->tick(delta);
	}
	for (GameObject::Ptr& gameObject : m_dynamicList)
	{
		if (false == gameObject->getBody().getLandStable())
			gameObject->getBody().addNatureValocity(gravityValocity * delta);
		changed = gameObject->tick(delta);
		if (!changed)
			continue;
		checkCollision(gameObject.get());
	}
	//std::cout << "GameWorld::tick " << (get_ms_time() - ms) << std::endl;

}


void GameWorld::checkCollision(GameObject* obj)
{
	for (GameObject::Ptr& gameObject : m_staticList)
	{
		if (!(obj->getBody().getAABB() ^ gameObject->getBody().getAABB()))
			continue;
		relocate(*gameObject, *obj);
	}
}


void GameWorld::relocate(const GameObject& blocker, GameObject& obj)
{
	const sf::Vector2i& valocity = obj.getBody().getLastValocity();
	double valoAngle = atan2(valocity.y, valocity.x);
	AABB objAABB = obj.getBody().getAABB() - valocity;
	const AABB& blockAABB = blocker.getBody().getAABB();

	std::vector<sf::Vector2i> objPoints;
	objPoints.reserve(obj.getBody().getPolygon().points.size());
	const std::vector < const Point >& blockPoints = blocker.getBody().getPolygon().points;


	//TODO 找出距离垂直速度线较近的一半点
	if (valocity.x > 0)
	{
		if (valocity.y < 0)
		{
			objPoints.emplace_back(objAABB.lowerBound);
		}
		objPoints.emplace_back(objAABB.upperBound.x, objAABB.lowerBound.y);
		objPoints.emplace_back(objAABB.upperBound);
		if (valocity.y > 0)
		{
			objPoints.emplace_back(objAABB.lowerBound.x, objAABB.upperBound.y);
		}
	}
	else if (valocity.x < 0)
	{
		if (valocity.y > 0)
		{
			objPoints.emplace_back(objAABB.upperBound);
		}
		objPoints.emplace_back(objAABB.lowerBound.x, objAABB.upperBound.y);
		objPoints.emplace_back(objAABB.lowerBound);
		if (valocity.y > 0)
		{
			objPoints.emplace_back(objAABB.upperBound.x, objAABB.lowerBound.y);
		}
	}
	else
	{
		if (valocity.y > 0)
		{
			objPoints.emplace_back(objAABB.upperBound);
			objPoints.emplace_back(objAABB.lowerBound.x, objAABB.upperBound.y);
		}
		else if (valocity.y < 0)
		{
			objPoints.emplace_back(objAABB.lowerBound);
			objPoints.emplace_back(objAABB.upperBound.x, objAABB.lowerBound.y);
		}
	}

	uint8 blockerPointCount = blockPoints.size();

	sf::Vector2i crossPoint;
	sf::Vector2i tempPoint;
	uint64 lowestDistance = INT64_MAX;
	const Edge* landEdge = nullptr;
	sf::Vector2i realOffset(0,0);
	bool bottomEdge(false);
	bool tempBottomEdge(false);
	bool stopedByBlockPoint(false);
	//将移动物体的需要判断的点的运动轨迹与被撞物体计算相交
	for (const sf::Vector2i& objPoint : objPoints)
	{
		for (uint8 i = 0; i < blockerPointCount; ++i)
		{
			const Point* targetP1 = &blockPoints[i];
			if (angleEqual(targetP1->fromOf->getAngle(), valoAngle))
			{
				continue;
			}
			const Point* targetP2 = targetP1->next;
			tempPoint = objPoint + valocity;
			if (isSegmentCross(objPoint, tempPoint, *targetP1, *targetP2, crossPoint))  
			{
				uint64 distanceSqrt = getP2PDistanceSqrt(objPoint, crossPoint);
				tempBottomEdge = (objPoint.y == objAABB.upperBound.y);
				if (distanceSqrt < lowestDistance)
				{
					bottomEdge = tempBottomEdge;
   					lowestDistance = distanceSqrt;
					realOffset = crossPoint - objPoint;
					landEdge = targetP1->fromOf;
				}
			}
		}
	}
	//将被撞物体的各个顶点与移动物体计算相交
	for (const Point& blockPoint : blockPoints)
	{
		if (angleEqual(blockPoint.fromOf->getAngle(),valoAngle))
		{
			continue;
		}
		for (uint8 i = 0; i < objPoints.size() - 1; ++i)
		{
			sf::Vector2i& targetP1 = objPoints[i];
			sf::Vector2i& targetP2 = objPoints[i+1];
			tempPoint = blockPoint - valocity;
			if (isSegmentCross(blockPoint, tempPoint, targetP1, targetP2, crossPoint))
			{
				uint64 distanceSqrt = getP2PDistanceSqrt(blockPoint, crossPoint);
				tempBottomEdge = (targetP1.y == objAABB.upperBound.y && targetP2.y == objAABB.upperBound.y);
				if (distanceSqrt < lowestDistance)
				{
					bottomEdge = tempBottomEdge;
					lowestDistance = distanceSqrt;
					realOffset = blockPoint - crossPoint;
					landEdge = blockPoint.fromOf;
					stopedByBlockPoint = true;
				}
			}
		}
	}

	if (lowestDistance != INT64_MAX)
	{
		sf::Vector2i tmpOffset;
		if (valocity.y < 0)
		{
			tmpOffset.y = realOffset.y + CENTI_PIXEL;
			tmpOffset.x = tmpOffset.y * (realOffset.x / realOffset.y);
		}
		else if (valocity.y > 0)
		{
			tmpOffset.y = realOffset.y - CENTI_PIXEL;
			tmpOffset.x = tmpOffset.y * (realOffset.x / realOffset.y);
		}
		else
		{
			if (valocity.x > 0)
			{
				tmpOffset.x = realOffset.x - CENTI_PIXEL;
				tmpOffset.y = tmpOffset.x * (realOffset.y / realOffset.x);
			}
			else if (valocity.x < 0)
			{
				tmpOffset.x = realOffset.x + CENTI_PIXEL;
				tmpOffset.y = tmpOffset.x * (realOffset.y / realOffset.x);
			}
		}

		objAABB += tmpOffset;
		obj.move(tmpOffset);
		obj.getBody().clearNatureValocity(valocity);
		obj.getBody().setInertiaValue(0);
		if (!bottomEdge)
			return;

		obj.setLand(landEdge);
		bool landStable(true);
		if (!stopedByBlockPoint && (landEdge->getAngle() < -LANDABLE_RATIO || landEdge->getAngle() > LANDABLE_RATIO))
			landStable = false;
		if (objAABB.lowerBound.x < landEdge->from->x || objAABB.upperBound.x > landEdge->to->x)
			landStable = false;
		obj.setLandStable(landStable);
	}
}


