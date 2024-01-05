#pragma once
#include <string>
#include <sgg/graphics.h>
#include "GameObject.h"
#include "Util.h"
#include "GameState.h"
class HealthUI :public GameObject
{
protected:
	graphics::Brush m_fill;
	float m_pos_x = 10.0f;
	float m_pos_y = 0.0f;
	float m_width = 0.5f;
	float m_height = 0.2f;

	
	float m_proportionalWidth = m_width;

	float m_duration = 0.3f;
	float m_startTime = 0.0f;
public:
	HealthUI() { init(); };
	HealthUI(float x, float y,float width, float height)
		: m_pos_x(x), m_pos_y(y),m_width(width), m_height(height) { init(); };
	~HealthUI() {  };

	void init() override
	{					
		SETCOLOR(m_brush.fill_color, 1.0, 0.0, 0.0);//background red
		SETCOLOR(m_fill.fill_color, 0.0, 1.0, 0.0);//green
		setCustomBrushProperties(&m_brush, 0.5f, 0.5f);
		setCustomBrushProperties(&m_fill, 0.5f, 0.5f);
	}

	void draw() override {}


	void setPosition(float x, float y)
	{
		m_pos_x = x;
		m_pos_y = y;
	}
	void setSize(float width, float height)
	{
		m_width = width;
		m_height = height;
	}

	void updateUIOnDamage(const float& initialHealth, const  float& currentHealth)
	{
		m_startTime = *m_state->getPausableClock();
		//reduces fill size, so that the red background appears
		m_proportionalWidth = currentHealth * m_width / initialHealth;
		setActive(true);	
	}
	
};

