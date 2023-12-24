#include "Level.h"
#include "GameState.h"
#include "util.h"
#include "Player.h"
#include <iostream>
#include <fstream>


void Level::init()
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("temp_background2.png"); // Make it not TOO big and try powers of 2 for given dimensions

	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->init();

	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->init();

	
	// Template of how we will level data
	// currently makes ceiling and right wall

	std::ifstream myfile(m_state->getFullDataPath(m_name) + ".txt");
	std::string line;
	if (myfile.is_open())
	{
		float x, y = 0;
		std::cout << "Level layout\n";
		while (myfile)
		{
			std::getline(myfile, line);
			std::cout << line;
			x = -m_state->getCanvasWidth() * 0.5f; // this need to be var, check draw
			for (char ch : line)
			{
				if (ch != ' ')
				{
					m_blocks.push_back(Box(x, y, 1, 1));
				}
				x++;
			}
			y++;
			std::cout << "\n";
		}
	}

	m_blocks.push_back(Box(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2 + 2, 1, 1));
/* Getting idea of location based on canvas
	m_blocks.push_back(Box(m_state->getCanvasWidth(), m_state->getCanvasHeight() / 2, 1, 1));
	m_blocks.push_back(Box(0, m_state->getCanvasHeight() / 2, 1, 1));

	m_blocks.push_back(Box(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight(), 1, 1));
	m_blocks.push_back(Box(m_state->getCanvasWidth() / 2, 0, 1, 1));
*/
	m_blocks.push_back(Box(5, 6, 1, 1)); // small parkour
	m_blocks.push_back(Box(4, 6, 1, 1));
	m_blocks.push_back(Box(3, 6, 1, 1));
	m_blocks.push_back(Box(3, 5, 1, 1));
	m_blocks.push_back(Box(2, 6, 1, 1));
	m_blocks.push_back(Box(1, 6, 1, 1));
	m_blocks.push_back(Box(0, 6, 1, 1));
	m_blocks.push_back(Box(0, 5, 1, 1));
	m_blocks.push_back(Box(0, 4, 1, 1));
	m_blocks.push_back(Box(-1, 4, 1, 1));
	m_blocks.push_back(Box(-2, 4, 1, 1));
	m_blocks.push_back(Box(-3, 4, 1, 1));
	m_blocks.push_back(Box(-3, 4, 1, 1));
	m_blocks.push_back(Box(-3, 3, 1, 1));
	m_blocks.push_back(Box(-3, 2, 1, 1));
	m_blocks.push_back(Box(-2, 2, 1, 1));

	m_block_brush.outline_opacity = 0.0f;	// texturing
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.1f, 1.0f, 0.1f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);

}

void Level::draw()
{
	float w = m_state->getCanvasWidth() ; 
	float h = m_state->getCanvasHeight();
	//========================================================================================
	float offset_x = m_state->m_global_offset_x + w * 0.5f;
	float offset_y = m_state->m_global_offset_y + h * 0.5f;
	/* background is moving */
	if (m_state->m_camera_follow_player)
	{
		//graphics::drawRect(m_state->getPlayer()->m_pos_x, offset_y, w * 2.0f, h, m_brush_background);
		graphics::drawRect(offset_x, offset_y, w * 2.0f, h, m_brush_background); // make w * 2.0f and h * into var for direct access from/to init()
	}
	else /* background is static */
	{
		graphics::drawRect(w * 0.5f, h * 0.5f, w, h, m_brush_background);
	}

	//order of draw() matters, if overllaping last goes on top
	if (m_state->getPlayer()->isActive()) // draws player
	{
		m_state->getPlayer()->draw();
	}
/* Not used currently
	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->draw();

	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->draw();*/

	for (int i = 0; i < m_blocks.size(); i++)
	{
		drawBlock(i);
	}

	if (m_state->m_paused) pausedDraw();
}

void Level::update(float dt)
{
	float p = 0.5f + fabs(cos(graphics::getGlobalTime() / 1000.0f));	// breaks when paused, needs personal timer, cause global

	SETCOLOR(m_brush_background.fill_color, p, p, 1.0f);	// change light

	if (m_state->getPlayer()->isActive())	
	{
		m_state->getPlayer()->update(dt);
	}

	checkCollisions();
	
	GameObject::update(dt);

}

Level::Level(const std::string& name) : GameObject(name)
{

}

//new
void Level::drawBlock(int i)
{
	Box& box = m_blocks[i];
	//std::string& name = m_block_names[i];	// for textures

	float x = box.m_pos_x + m_state->m_global_offset_x;
	float y = box.m_pos_y + m_state->m_global_offset_y;

	m_block_brush.texture = m_state->getFullAssetPath("crate.png");

	graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush);

	if (m_state->m_debugging)
		graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush_debug);

}

void Level::pausedDraw()
{
	graphics::Brush paused_brush;
	paused_brush.fill_opacity = 0.75f;
	paused_brush.outline_opacity = 0.f;
	SETCOLOR(paused_brush.fill_color, 0.25f, 0.25f, 0.25f);

	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(), 
		m_state->getCanvasHeight(), paused_brush);
}

void Level::checkCollisions()
{
	for (auto& box : m_blocks)
	{
		if (m_state->getPlayer()->intersect(box))
		{
		//	printf("*");

		}
	}
	for (auto& block : m_blocks)
	{
		if (!m_state->getPlayer()->intersectTypeY(block))
		{
			float offset = 0.0f;
			if (offset = m_state->getPlayer()->intersectSideways(block))
			{
				m_state->getPlayer()->m_pos_x += offset;

				m_state->getPlayer()->m_vx = 0.0f;
				break;
			}
		}
	}

	for (auto& block : m_blocks)
	{
		if (m_state->getPlayer()->intersectTypeY(block))
		{ 
			float offset = 0.0f;
			if (offset = m_state->getPlayer()->intersectY(block))
			{
				m_state->getPlayer()->m_pos_y += offset;

				// add sound event
				//if (m_state->getPlayer()->m_vy > 1.0f)
				//	graphics::playSound(m_state->getFullAssetPath("Metal2.wav"), 1.0f);

				m_state->getPlayer()->m_vy = 0.0f;
				break;
			}
		}
	}
}

Level::~Level()
{
	for (auto p_gob : m_static_objects)
		if (p_gob) delete p_gob;

	for (auto p_gob : m_dynamic_objects)
		if (p_gob) delete p_gob;
}
