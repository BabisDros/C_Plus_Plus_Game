#pragma once
#include <sgg/graphics.h>
#include "GameObject.h"

class HealthUI :public GameObject
{
protected:
	graphics::Brush m_fillBrush,m_valueBrush;
	std::string m_healthTxt;
	float m_pos_x = 10.0f;
	float m_pos_y = 0.0f;
	float m_width = 0.5f;
	float m_height = 0.2f;
	
	float m_fontSize = 1;
	float m_proportionalWidth = m_width;

	float m_duration = 0.3f;
	float m_startTime = 0.0f;
public:
	HealthUI() { init(); };
	HealthUI(float x, float y,float width, float height)
		: m_pos_x(x), m_pos_y(y),m_width(width), m_height(height) { init(); };
	~HealthUI() {  };

	void init() override;
	void draw() override {}
	void setPosition(float x, float y);
	void setSize(float width, float height);
	void updateUIOnDamage(const int& initialHealth, const  int& currentHealth);
	
};

