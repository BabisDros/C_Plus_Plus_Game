#pragma once
#include <string>
#include <sgg/graphics.h>
#include "GameObject.h"
#include "Util.h"
#include "GameState.h"
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

	void init() override
	{		
		SETCOLOR(m_brush.fill_color, 1.0, 0.0, 0.0);//background red
		SETCOLOR(m_fillBrush.fill_color, 0.0, 1.0, 0.0);//green
		SETCOLOR(m_valueBrush.fill_color, 1.0f, 1.0f, 1.0f);
		setCustomBrushProperties(&m_brush, 0.5f, 0.5f);
		setCustomBrushProperties(&m_fillBrush, 0.5f, 0.5f);
		setCustomBrushProperties(&m_valueBrush, 0.8f, 0.8f);
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

	void updateUIOnDamage(const int& initialHealth, const  int& currentHealth)
	{
		m_startTime = *m_state->getPausableClock();
		//reduces fill size once, so that the red background appears
		m_proportionalWidth = currentHealth * m_width / initialHealth;
		m_healthTxt = std::to_string(currentHealth);

		setActive(true);	
	}
	
};

