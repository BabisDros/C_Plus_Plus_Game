#pragma once
#include <sgg/graphics.h>
#include "CollisionObject.h"

/// <summary>
/// "DamageBox is the effective area of a weapon. Anything that comes into contact with it takes damage."
/// </summary>
class DamageBox:public CollisionObject
{
public:	
	DamageBox() { init(); };
	DamageBox(float x, float y, float w, float h) :CollisionObject(x, y, w, h)
	{
		//best to initialize object when created
		init(); 
	}

	void setPosition(float x, float y, float w, float h)
	{
		m_pos_x = x;
		m_pos_y = y;
		m_width = w;
		m_height = h;
	}
	int damage = 0;
	//gets Parent's Looking direction:Left:-1,Right:1
	int* m_parentDirection = 0;
	void init() override;
	void draw() override;
	void update(float dt) override;
	void setBrush(const graphics::Brush& brush) {	m_brush = brush; }
	void checkForCollisions();
};

