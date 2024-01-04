#pragma once
#include <string>
#include <sgg/graphics.h>
#include "GameObject.h"
#include "Util.h"
#include "GameState.h"
class HealthUI :public GameObject
{
private:
	graphics::Brush m_fill;
	float m_pos_x = 10.0f;
	float m_pos_y = 0.0f;
	float m_width = 0.5f;
	float m_height = 0.2f;

	//adjust offset to appear above gameobject
	float m_offsetY = 1.f;
	float m_proportionalWidth = m_width;

	float m_duration = 0.3f;
	float m_startTime=0.0f;
public:
	HealthUI() { init(); };
	HealthUI(float x, float y)
		: m_pos_x(x), m_pos_y(y) { init(); };
	~HealthUI() {  };

	void init() override
	{
		setActive(false);
		SETCOLOR(m_brush.fill_color, 1.0, 0.0, 0.0);//background red
		SETCOLOR(m_fill.fill_color, 1.0, 0.5, 0.0);//orange
		setCustomBrushProperties(&m_brush, 0.5f, 0.5f);
		setCustomBrushProperties(&m_fill, 0.5f, 0.5f);
	}

	void draw() override
	{
		if (isActive())
		{
			float positionCorrection = (m_width - m_proportionalWidth)/2;
			graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y - m_offsetY, m_width, m_height, m_brush);
			graphics::drawRect(m_pos_x + m_state->m_global_offset_x- positionCorrection, m_pos_y + m_state->m_global_offset_y - m_offsetY, m_proportionalWidth, m_height, m_fill);
			disableAfterElapsedTime();
		}
		
		if (m_state->m_debugging)
		{
			debugDraw(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y - m_offsetY, m_width, m_height);
		}
	}
	void setPosition(float x, float y)
	{
		m_pos_x = x;
		m_pos_y = y;
	}

	void updateUIOnDamage(const int& damage, const float& initialHealth, const  float& currentHealth)
	{
		m_startTime = *m_state->getPausableClock();

		//reduces fill size, so that the red background appears
		m_proportionalWidth = currentHealth * m_width / initialHealth;
		setActive(true);	
	}
	
	void disableAfterElapsedTime()
	{
		if (*m_state->getPausableClock() - m_startTime > m_duration)
		{
			setActive(false);		
		}
	}
};

