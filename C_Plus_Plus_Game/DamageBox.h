#pragma once
#include <sgg/graphics.h>
#include "box.h"
#include "GameObject.h"
#include "GameState.h"
#include "Level.h"
#include "IDestructible.h"
/// <summary>
/// "DamageBox is the effective area of a weapon. Anything that comes into contact with it takes damage."
/// </summary>
class DamageBox:public GameObject, public Box
{
public:	
	DamageBox() { init(); };
	DamageBox(float x, float y, float w, float h) :Box(x, y, w, h) 
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
	//gets Parent's Looking direction:Left:-1,Right:1
	int* m_parentDirection = 0;
	void init() override;
	void draw() override;
	void update(float dt) override;
	void setBrush(graphics::Brush brush) {	m_brush = brush; }
private:
	bool damageOnce = false;
};

