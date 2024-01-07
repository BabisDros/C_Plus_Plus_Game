#pragma once
#include "HealthUI.h"

//add a ui of this type so that it appears fixed on position of your choice
class HealthUIFixed :public HealthUI
{
public:
	HealthUIFixed() :HealthUI() { init(); }
	HealthUIFixed(float x, float y, float width, float height) :HealthUI(x, y, width, height ) { init(); }

	void init() override;
	void draw() override;
};