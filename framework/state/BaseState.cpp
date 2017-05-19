#include "stdafx.h"
#include "BaseState.h"
#include "framework/event/EventManager.h"
#include "framework/state/StateManager.h"//��Ϊ���������BaseState����ǰ�������ģ������������������ſ���ʹ��


BaseState::BaseState(StateManager* manager)
	: m_stateManager(manager)
	, m_eventManager(manager->getContext()->eventManager)
	, m_transcendent(false)
	, m_transparent(false)
{
}


BaseState::~BaseState()
{
}
