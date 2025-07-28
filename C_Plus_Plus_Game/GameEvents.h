#pragma once
#include "Manager.h"
#include "CstmCallback.h"
class GameEvents: public Manager<GameEvents>
{
public:
	CstmCallback<int,int> m_playerHealthChanged;
	CstmCallback<float,float> m_playerMoved;
	CstmCallback<> m_playHurtFx;
	CstmCallback<> m_playerDied;
	//int:no of lives. bool: setValue (e.g,pass true in trigger if you want to set it to the given lives value)
	CstmCallback<int,bool> m_playerLivesChanged;
	//int: points. bool: setValue (e.g true if want to set it to the given point value)
	CstmCallback<int, bool> m_pointsChanged;
	CstmCallback<float,float> m_enemyDied;
};
