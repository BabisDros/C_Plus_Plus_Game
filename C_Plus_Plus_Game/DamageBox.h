#pragma once
#include "CollisionObject.h"
#include <map>
/// <summary>
/// "DamageBox is the effective area of a weapon. Anything that comes into contact with it takes damage."
/// </summary>
class DamageBox:public CollisionObject
{
	//Parent's Looking direction:Left:-1,Right:1
	int m_parentDirection = 0;
	bool m_parentIsPlayer = false;
	bool m_canActivelyBeMirrored ;
	int m_damageToInflict = 0;

public:	
	DamageBox() {};
	DamageBox(int damage, bool canBecomeMirrored = true) : m_canActivelyBeMirrored(canBecomeMirrored),m_damageToInflict(damage)
	{ init(); };
	
	bool m_diesOnTouch = false;
	bool m_canMove = true;
	int m_draw_direction;
	void init() override;
	void draw() override;
	void update(float dt) override;
	void setParentIsPlayer(bool isPlayer);
	void setBrush(const graphics::Brush& brush);
	void checkForCollisions(const std::list<CollisionObject*> containerToScan);
	void checkForCollisions(CollisionObject* player);
	void setDamageToInflict(int damage);
	void pushPlayer();
	
	void setParentDirection(const int& direction);
	void setPosition(float x, float y, float width, float height);
};

