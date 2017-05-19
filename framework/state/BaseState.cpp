#include "stdafx.h"
#include "BaseState.h"
#include "framework/event/EventManager.h"
#include "framework/state/StateManager.h"//因为这个类是在BaseState里面前向声明的，所以这里必须包进来才可以使用


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
