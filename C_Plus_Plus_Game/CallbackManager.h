#pragma once
#include "CstmCallback.h"
class CallbackManager
{
	static CallbackManager* s_unique_instance;
public:
	CstmCallback<int,int> m_playerIsDamaged;
	CstmCallback<float,float> m_playerMoved;
	CstmCallback<> m_playHurtFx;
	CstmCallback<> m_playerDied;

	CstmCallback<int> m_pointsChanged;
	CstmCallback<float,float> m_enemyDied;
	static CallbackManager* getInstance();
};
