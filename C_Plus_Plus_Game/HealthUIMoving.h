#pragma once
#include "HealthUI.h"
//add a ui of this type if you want to appear above object and follow it
class HealthUIMoving :public HealthUI
{
	//adjust offset to appear above gameobject. 
	float m_offsetY = 1.f;
public:
	HealthUIMoving() :HealthUI() { init(); }
	HealthUIMoving(float x, float y, float width, float height) :HealthUI(x, y, width, height) { init(); }

	void init() override;
	void draw() override;	
	void disableAfterElapsedTime();
};
