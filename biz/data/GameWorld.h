#pragma once
#include "biz/data/base/GameObject.h"

class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	void tick(float delta);
	
	template<typename T>
	void addObject(const T& obj)
	{
		typename GameObject::Ptr ptr = std::dynamic_pointer_cast<GameObject>(obj);
		switch (ptr->getType())
		{
			case GAME_OBJECT_BLOCK:
				m_staticList.emplace_back(ptr);
				break;
			case GAME_OBJECT_CHAR:
				m_dynamicList.emplace_back(ptr);
				break;
			default:
				FORCE_CRASH(true);
				break;
		}
	};

	template<typename T>
	void removeObject(const T& obj)
	{
		GameObject::Ptr ptr = std::dynamic_pointer_cast<GameObject>(obj);
		auto it = std::find(m_dynamicList.cbegin(), m_dynamicList.cend(), ptr);
		if (it != m_dynamicList.cend())
		{
			m_dynamicList.erase(it);
			return;
		}
		
		it = std::find(m_staticList.cbegin(), m_staticList.cend(), ptr);
		if (it != m_staticList.cend())
		{
			m_staticList.erase(it);
			return;
		}

		FORCE_CRASH(true);
	};

	const std::list<GameObject::Ptr>& getDynamicList() const { return m_dynamicList; }
	const std::list<GameObject::Ptr>& getStaticList() const { return m_staticList; }


private:
	void checkCollision(GameObject* obj);
	void relocate(const GameObject& blocker, GameObject& obj);

private:
	
	float m_delta;
	std::list<GameObject::Ptr> m_dynamicList;
	std::list<GameObject::Ptr> m_staticList;
};

