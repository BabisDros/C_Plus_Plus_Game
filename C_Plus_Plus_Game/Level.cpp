#include "Level.h"
#include "GameState.h"
#include "util.h"
#include "Player.h"

//new
void Level::CheckCollisions()
{ // need to take apropriate actions
	for (auto& box : m_blocks)
	{
		if (m_state->getPlayer()->intersect(box))
		{
			printf("*");

		}
	}
}



void Level::init()
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("temp_background2.png"); // Make it not TOO big and try powers of 2 for given dimensions

	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->init();

	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->init();

	//new
	//add random boxes to vectors

}

void Level::draw()
{
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();

	/* background is static */
	//graphics::drawRect(w * 0.5f, h * 0.5f , w * 2.0f, h,m_brush_background); 

	/* background is moving */
	float offset_x = m_state->m_global_offset_x + w * 0.5f;
	float offset_y = m_state->m_global_offset_y + h * 0.5f;
	graphics::drawRect(offset_x, offset_y, w * 2.0f, h, m_brush_background);


	//order of draw() matters, if overllaping last goes on top
	if (m_state->getPlayer()->isActive()) // draws player
	{
		m_state->getPlayer()->draw();
	}

	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->draw();

	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->draw();

}

void Level::update(float dt)
{
	float p = 0.5f + fabs(cos(graphics::getGlobalTime() / 1000.0f));

	SETCOLOR(m_brush_background.fill_color, p, p, 1.0f);	// change light

	if (m_state->getPlayer()->isActive())
	{
		m_state->getPlayer()->update(dt);
	}

	CheckCollisions();
	
	GameObject::update(dt);
}

Level::Level(const std::string& name)
{
}

Level::~Level()
{
	for (auto p_gob : m_static_objects)
		if (p_gob) delete p_gob;

	for (auto p_gob : m_dynamic_objects)
		if (p_gob) delete p_gob;
}
