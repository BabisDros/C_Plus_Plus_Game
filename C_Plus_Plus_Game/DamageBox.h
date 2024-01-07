#pragma once
#include <sgg/graphics.h>
#include "CollisionObject.h"

/// <summary>
/// "DamageBox is the effective area of a weapon. Anything that comes into contact with it takes damage."
/// </summary>
class DamageBox:public CollisionObject
{
public:	
	DamageBox() { init(); }
	DamageBox(float x, float y, float w, float h) :CollisionObject(x, y, w, h)
	{
		//best to initialize object when created
		init();
	}
	DamageBox(bool canBecomeMirrored) { canBeMirrored = canBecomeMirrored; }
	void setPosition(float x, float y, float width, float height)
	{
		m_pos_x = x;
		m_pos_y = y;
		m_width = width;
		m_height = height ;
	}
	int m_damage = 0;
	//gets Parent's Looking direction:Left:-1,Right:1
	int* m_parentDirection = 0;
	bool canBeMirrored=true;
	void init() override;
	void draw() override;
	void update(float dt) override;
	void setBrush(const graphics::Brush& brush) {	m_brush = brush; }
	void checkForCollisions();
};

