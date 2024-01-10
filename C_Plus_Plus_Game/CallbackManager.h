#pragma once
#include "CstmCallback.h"
class CallbackManager
{
	static CallbackManager* s_unique_instance;
public:
	CstmCallback m_playerIsDamaged;
	CstmCallback m_pointsChanged;
	static CallbackManager* getInstance();
};
