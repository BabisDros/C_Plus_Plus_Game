#include "Level.h"
#include "GameState.h"
#include "util.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <sstream>

void Level::init()
{
	m_brush.outline_opacity = 0.0f;
	m_brush.texture = m_state->getFullAssetPath("temp_background2.png"); //? Make it not TOO big and try powers of 2 for given dimensions

	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->init();

	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->init();

	read();	//! 
}

void Level::draw()
{
	float w = m_state->getCanvasWidth(); 
	float h = m_state->getCanvasHeight();

	float offset_x = m_state->m_global_offset_x + w * 0.5f;
	float offset_y = m_state->m_global_offset_y + h * 0.5f;
	/* background is moving */
	graphics::drawRect(offset_x, offset_y, w / 0.5f, h, m_brush); //! make w * 2.0f and h * into var for direct access from/to init()

	//? order of draw() matters, if overllaping last goes on top
	if (m_state->getPlayer()->isActive()) //? draws player
	{
		m_state->getPlayer()->draw();
	}
/* Not used currently
	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->draw();*/

	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->draw();

	for (int i = 0; i < m_blocks.size(); i++)
	{
		m_blocks[i]->draw();
	}

	if (m_state->m_paused) pausedDraw();
}

void Level::update(float dt)
{
	float p = 0.5f + fabs(cos(*m_state->getPausableClock() / 10000.0f));

	SETCOLOR(m_brush.fill_color, p, p, 1.0f);	//? change light

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


void Level::pausedDraw()	//! make it better than a greyed out screen
{
	graphics::Brush paused_brush;
	paused_brush.fill_opacity = 0.75f;
	paused_brush.outline_opacity = 0.f;
	SETCOLOR(paused_brush.fill_color, 0.25f, 0.25f, 0.25f);

	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(), 
		m_state->getCanvasHeight(), paused_brush);
}

void Level::read()
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

			t_bool = v[4] == "1";
			std::tuple <float, float, const std::string, bool> data = std::make_tuple(std::stof(v[1]), std::stof(v[2]), v[3], t_bool);	//? data values (x, y, texture, is IDestructible
			
			m_objects_data.insert({v[0][0], data});
			std::getline(myfile, line);
		}
		std::cout << "Level layout\n";
		float x, y = 0;
		bool destructible=false;
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
						destructible = std::get<3>(itr->second);
						if (destructible)
						{
							m_dynamic_objects.push_back(new CrateDestructible(30,x + std::get<0>(itr->second) / 2.f, y + std::get<1>(itr->second) / 2.f, std::get<0>(itr->second), std::get<1>(itr->second),
								&std::get<2>(itr->second), destructible));
						}
						
						else
						{
							m_blocks.push_back(new LevelBox(x + std::get<0>(itr->second) / 2.f, y + std::get<1>(itr->second) / 2.f, std::get<0>(itr->second), std::get<1>(itr->second),
								&std::get<2>(itr->second), destructible));
						}
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
		if (!m_state->getPlayer()->intersectTypeY(*block))
		{
			float offset;
			if (offset = m_state->getPlayer()->intersectSideways(*block))	//? Does not go in if 0
			{
				m_state->getPlayer()->m_pos_x += offset;

				m_state->getPlayer()->m_vx = 0.0f;
				break;
			}
		}
	}

	for (auto& block : m_blocks)
	{
		if (m_state->getPlayer()->intersectTypeY(*block))
		{ 
			float offset;
			if (offset = m_state->getPlayer()->intersectY(*block))	//? Does not go in if 0
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
std::vector<LevelBox*> Level::getBlocks() const
{
	return m_blocks;
}
std::list<GameObject*> Level::getDynamicObjects() const
{
	return m_dynamic_objects;
}

template <typename Container>
void Level::destroyGameObjects(Container& myContainer)
{
	for (auto p_gob : myContainer)
	{
		if (p_gob)
		{
			delete p_gob;
		}
	}
}

Level::~Level()
{
	destroyGameObjects(m_static_objects);
	destroyGameObjects(m_dynamic_objects);
	destroyGameObjects(m_blocks);
}


