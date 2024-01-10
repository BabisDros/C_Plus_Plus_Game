#pragma once
#include "CstmCallback.h"
class CallbackManager
{
	static CallbackManager* s_unique_instance;
public:
	CstmCallback<int,int> m_playerIsDamaged;
	CstmCallback<int,int> m_pointsChanged;
	CstmCallback<float,float> m_enemyDied;
	static CallbackManager* getInstance();
};
