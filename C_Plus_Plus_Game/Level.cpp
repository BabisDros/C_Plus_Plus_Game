#include "Level.h"
#include "GameState.h"
#include "util.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <sstream>

void Level::init()
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("temp_background2.png"); //? Make it not TOO big and try powers of 2 for given dimensions

	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->init();

	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->init();

	read(m_blocks);	//! 

	m_block_brush.outline_opacity = 0.0f;	//? texturing
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.1f, 1.0f, 0.1f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);

}

void Level::draw()
{
	float w = m_state->getCanvasWidth(); 
	float h = m_state->getCanvasHeight();

	float offset_x = m_state->m_global_offset_x + w * 0.5f;
	float offset_y = m_state->m_global_offset_y + h * 0.5f;
	/* background is moving */
	graphics::drawRect(offset_x, offset_y, w / 0.5f, h, m_brush_background); //! make w * 2.0f and h * into var for direct access from/to init()

	//? order of draw() matters, if overllaping last goes on top
	if (m_state->getPlayer()->isActive()) //? draws player
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
	float p = 0.5f + fabs(cos(graphics::getGlobalTime() / 10000.0f));	//! breaks when paused, needs personal timer, cause global

	SETCOLOR(m_brush_background.fill_color, p, p, 1.0f);	//? change light

	checkCollisions();
	if (m_state->getPlayer()->isActive())	
	{
		m_state->getPlayer()->update(dt);
	}

	GameObject::update(dt);
}

Level::Level(const std::string& name) : GameObject(name)
{
}

//? Should get replaced with generic static and/or dynamic draw
void Level::drawBlock(int i)
{
	Box& box = m_blocks[i];
	char& tag = m_block_names[i];

	float x = box.m_pos_x + m_state->m_global_offset_x;
	float y = box.m_pos_y + m_state->m_global_offset_y;


	m_block_brush.texture = m_state->getFullAssetPath(std::get<2>(m_objects_data.at(tag)));
	graphics::drawRect(x, y, std::get<0>(m_objects_data.at(tag)), std::get<1>(m_objects_data.at(tag)), m_block_brush);

	if (m_state->m_debugging) graphics::drawRect(x, y, std::get<0>(m_objects_data.at(tag)), std::get<1>(m_objects_data.at(tag)), m_block_brush_debug);

}

void Level::pausedDraw()	//! make it better than a greyed out screen
{
	graphics::Brush paused_brush;
	paused_brush.fill_opacity = 0.75f;
	paused_brush.outline_opacity = 0.f;
	SETCOLOR(paused_brush.fill_color, 0.25f, 0.25f, 0.25f);

	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(), 
		m_state->getCanvasHeight(), paused_brush);
}

void Level::read(std::vector<Box> &m_blocks)
{
	std::ifstream myfile(m_state->getFullDataPath(m_name) + ".txt");
	std::string line;

	if (myfile.is_open())
	{
		std::getline(myfile, line);
		while (line[0] != '$')	//? get to metadata point
		{
			std::getline(myfile, line);
		}
		std::getline(myfile, line);

		while (line[0] != '$')	//? end of metadata
		{
			std::string s;
			std::stringstream ss(line);
			std::vector<std::string> v;
			bool t_bool;
			while (std::getline(ss, s, ' '))
			{
				v.push_back(s);
			}

			if (v[4] == "1")
			{
				t_bool = true;
			}
			else
			{
				t_bool = false;
			}

			std::tuple <float, float, std::string, bool> data = std::make_tuple(std::stof(v[1]), std::stof(v[2]), v[3], t_bool);	//? data values (x, y, texture, is Destructible
			m_objects_data.insert({v[0][0], data});
			std::getline(myfile, line);
		}

		float x, y = 0;
		while (myfile)
		{ 
			std::getline(myfile, line); //? not '$'
			if (line == "$") break;	//? no more lines on the level
			std::cout << line;
			x = -m_state->getCanvasWidth() * 0.5f; //! this (0.5f) need to be var, check draw
			for (char ch : line)
			{
				if (ch == '|') break;	//? end of line
				for (auto itr = m_objects_data.begin(); itr != m_objects_data.end(); ++itr)
				{
					if (itr->first == ch)
					{
						m_blocks.push_back(Box(x, y, std::get<0>(itr->second), std::get<1>(itr->second)));
						m_block_names.push_back(itr->first);
					}
				}
				x++;
			}
			y++;
			std::cout << "\n";
		}
	}
}

void Level::checkCollisions()
{
	/*	//? Sitting around currently
	for (auto& box : m_blocks)
	{
		if (m_state->getPlayer()->intersect(box))
		{
		}
	}*/

	for (auto& block : m_blocks)
	{
		if (!m_state->getPlayer()->intersectTypeY(block))
		{
			float offset;
			if (offset = m_state->getPlayer()->intersectSideways(block))	//? Does not go in if 0
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
			float offset;
			if (offset = m_state->getPlayer()->intersectY(block))	//? Does not go in if 0
			{			
				m_state->getPlayer()->m_pos_y += offset;
				if (offset > 0)
				{
					m_state->getPlayer()->m_collidingUp = true;	
				}
				else
				{
					m_state->getPlayer()->m_collidingUp = false;
				}
				//? add sound event
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
