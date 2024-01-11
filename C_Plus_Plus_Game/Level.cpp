#pragma once
#include "Level.h"
#include "GameState.h"
#include "util.h"
#include "Player.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "LevelBox.h"
#include "CrateDestructible.h"
#include "box.h"
#include "CallbackManager.h"
#include "ParticleManager.h"
#include <filesystem> // to read sprites for animation 
#include <thread>
//#include <algorithm>
//#include <execution>

void Level::init()
{
	m_brush.outline_opacity = 0.0f;
	m_brush.texture = m_state->getFullAssetPath("temp_background2.png"); //? Make it not TOO big and try powers of 2 for given dimensions
	read();
	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->init();

	for (auto p_gob : m_destructible_objects)
		if (p_gob) p_gob->init();
	ParticleManager::getInstance()->init();
	readSprites("fireball", m_fireball_sprites);
}

void Level::draw()
{
	float w = m_state->getCanvasWidth(); 
	float h = m_state->getCanvasHeight();

	float offset_x = m_state->m_global_offset_x + w * 0.5f;
	float offset_y = m_state->m_global_offset_y + h * 0.5f;
	/* background is moving */
	graphics::drawRect(offset_x, offset_y, w / 0.5f, h / 0.5f, m_brush); //! make w * 2.0f and h * into var for direct access from/to init()
	
/* Not used currently
	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->draw();*/

	for (int i = 0; i < m_blocks.size(); i++)
	{
		m_blocks[i]->draw();
	}

	for (auto p_gob : m_destructible_objects)
		if (p_gob->isActive()) p_gob->draw();
	//? order of draw() matters, if overllaping last goes on top
	ParticleManager::getInstance()->draw();
	if (m_state->getPlayer()->isActive()) //? draws player
	{
		m_state->getPlayer()->draw();
	}
}

void Level::update(float dt)
{
	//float p = 0.5f + fabs(cos(*m_state->getPausableClock() / 10000.0f));
	ParticleManager::getInstance()->update(dt);
	//SETCOLOR(m_brush.fill_color, p, p, 1.0f);	//? change light
	if (m_state->getPlayer()->intersect((*m_level_end))) 
	{ 
		m_state->goNextLevel = true; // level finished
		CallbackManager::getInstance()->m_pointsChanged.trigger(100);
	}	

	if (m_state->getPlayer()->isActive())	
	{
		m_state->getPlayer()->update(dt);
	}


//	for (auto p_gob : m_destructible_objects)
//		if (p_gob->isActive()) p_gob->update(dt);

	std::vector<std::thread> mythreads;
	auto middle = m_destructible_objects.begin();

	std::advance(middle, m_destructible_objects.size() / 2);
	std::thread t1(&Level::updateDynamicBounded, this, m_destructible_objects.begin(), m_destructible_objects.end(), dt);
	++middle;
	updateDynamicBounded (middle, m_destructible_objects.end(), dt);
	t1.join();/**/
}

Level::Level(const std::string& name) : GameObject(name) {}



void Level::read()
{
	srand((unsigned)time(NULL));

	std::ifstream myfile(m_state->getFullDataPath(m_name) + ".txt");
	std::string line, title;
	std::vector <std::string> data(std::max(m_terrain_titles.size(), m_enemy_titles.size()), "");	// the list is as big as the base with most data
	int x, y=0;
	y = -m_state->getCanvasHeight() * 0.5f;
	if (myfile.is_open())
	{
		while(true)
		{ 
			ignoreEmptyLine(myfile, line);
			if (line[0] == '$') break;
			if (line == "Terrain")
			{
				saveObjectData(m_terrain_data, m_terrain_titles, myfile, line);
			}
			else if (line == "Enemy")
			{
				saveObjectData(m_enemy_data, m_enemy_titles, myfile, line);
			}
			else if (line == "Level")
			{
				std::cout << "Level layout\n";
				std::getline(myfile, line);
				while (line[0] !='$')
				{ 
					std::cout << line;
					x = -m_state->getCanvasWidth() * 0.5f; //! this (0.5f) need to be var, check draw
					bool tag_found, destructible;
					for (char ch : line)
					{
						tag_found = false;
						if (ch == ' ') { x++; continue; } // empty space
						if (ch == '|') break;	//? end of line
						if (ch == 'P')	// Player
						{
							m_player_start_x = x + 1 / 2.f;
							m_player_start_y = y + 1 / 2.f;
							x++;
							continue;
						}
						for (auto itr = m_terrain_data.begin(); itr != m_terrain_data.end(); ++itr)
						{
							if (itr->first == ch)
							{
								destructible = (itr->second)[3] == "1";
								if (destructible)
								{
									int number = rand() % 100;	// either 0, 1, 2 | no loot, health, double points
									Loot loot;
									if (number < 40) loot = Nothing;
									else if (number < 80) loot = Extra_loot;
									else loot = Health_pack;

									m_destructible_objects.push_back(new CrateDestructible(std::stof((itr->second)[4]),x + std::stof((itr->second)[0]) / 2.f, y + std::stof((itr->second)[1]) / 2.f, std::stof((itr->second)[0]), std::stof((itr->second)[1]),
										&(itr->second)[2], destructible, loot));
									tag_found = true;
									break;
								}
								else
								{
									m_blocks.push_back(new LevelBox(x + std::stof((itr->second)[0]) / 2.f, y + std::stof((itr->second)[1]) / 2.f, std::stof((itr->second)[0]), std::stof((itr->second)[1]),
										&(itr->second)[2], destructible));
									if (itr->first == m_level_end_tag) m_level_end = m_blocks.back();
									tag_found = true;
									break;
								}
							}
						}
						if (!tag_found)	// make enemy
						{
							for (auto itr = m_enemy_data.begin(); itr != m_enemy_data.end(); ++itr)
							{
								if (itr->first == ch)
								{
									bool jumping = itr->second[6] == "1";
									bool ranged = itr->second[4] == "1";
									bool body_damage = itr->second[5] == "1";
									int stick_wall;
									if ((itr->second)[8] == "") stick_wall = 0;
									else stick_wall = std::stoi((itr->second)[8]);

									if (itr->second[9] != "")	// It has a movement value
									{
										m_destructible_objects.push_back(new Enemy("", x + std::stof((itr->second)[0]) / 2.f, y + std::stof((itr->second)[1]) / 2.f, // name, pos_x/y
											std::stof((itr->second)[0]), std::stof((itr->second)[1]), &(itr->second)[2], std::stof((itr->second)[3]),	// width, height, texture, hp
										ranged, body_damage, jumping, std::stof((itr->second)[7]), stick_wall, std::stof((itr->second)[9]), std::stof((itr->second)[10]), std::stoi((itr->second)[11]))); // + territory_x/y, movement_type
										break;
									}
									else
									{
										m_destructible_objects.push_back(new Enemy("", x + std::stof((itr->second)[0]) / 2.f, y + std::stof((itr->second)[1]) / 2.f, // name, pos_x/y
											std::stof((itr->second)[0]), std::stof((itr->second)[1]), &(itr->second)[2], std::stof((itr->second)[3]),	// width, height, texture, hp
											ranged, body_damage, jumping, std::stof((itr->second)[7]), std::stof((itr->second)[8])));
										break;
									}
								}
							}
						}
						x++;
					}
					y++;
					std::cout << "\n";
					std::getline(myfile, line);
				}
			}
		}
	}
}

void Level::removeSpaces(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) 
	{
		return !std::isspace(ch);
	}));
}

//? gets next line in file and trims empty spaces in front until it gets a non-empty line
void Level::ignoreEmptyLine(std::ifstream& file, std::string& line)
{
	std::getline(file, line);
	removeSpaces(line);
	while (line == "")	// looks for non-empty line
	{
		std::getline(file, line);
		removeSpaces(line);
	}
}
//? returns substring left of ':'
std::string Level::getDataTitle(std::string s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) 
	{
		return ch == ':';
	}).base(), s.end());	// removes anything right of ':'
	s.pop_back(); // removes ':'
	return s;
}

//? keep substring right of ':'
void Level::getDataValue(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
		{
			return ch == ':';
		}));
	s.erase(0, 1); // removes ':'
	removeSpaces(s);
}

void Level::saveObjectData(std::map<char, std::vector<std::string>>& database, std::vector<std::string> titles, std::ifstream& file, std::string& line)
{
	char tag;
	std::string title;
	std::vector <std::string> data(database.size(), "");
	ignoreEmptyLine(file, line);
	if (line[0] = '{')
	{
		ignoreEmptyLine(file, line);
		tag = line[0];
		ignoreEmptyLine(file, line);
		if (line[0] = '{')
		{
			ignoreEmptyLine(file, line);
			while (line[0] != '}')
			{
				// read data				// ex. line = width: 5
				title = getDataTitle(line);	// ex. title = width
				int index = 0;
				for (std::string type : titles)
				{
					if (title == type)
					{
						getDataValue(line);
						data.insert(data.begin() + index, line);
						break;
					}
					index++;
				}
				ignoreEmptyLine(file, line);	//go to  next data
			}
			// add data for tag to database
			if (data[5] != "" && database == m_terrain_data)	// has ending value
			{
				m_level_end_tag = tag;	// this tag represents end of level
			}
			database.insert({ tag, data });
			ignoreEmptyLine(file, line);	// go to next data type
		}
	}
}

std::vector<LevelBox*> Level::getBlocks() const
{
	return m_blocks;
}
std::list<CollisionObject*> Level::getDestructibleObjects() const
{
	return m_destructible_objects;
}

std::list<CollisionObject*>* Level::getDestructibleObjectsPtr()
{
	return &m_destructible_objects;
}

void Level::updateDynamicBounded(std::_List_iterator < std::_List_val < std::_List_simple_types<CollisionObject*>>> start, 
	std::_List_iterator < std::_List_val < std::_List_simple_types<CollisionObject*>>> end, float dt)
{

//	std::for_each(std::execution::par, start, end, [dt](auto itr) {
//		if (itr->isActive()) itr->update(dt);
//		});

	for (auto itr = start; itr != end; itr++)
		if ((*itr)->isActive()) (*itr)->update(dt);
}

std::vector<std::string>* Level::getFireballSprites()
{
	return &m_fireball_sprites;
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


void Level::readSprites(std::string folder, std::vector<std::string>& myVec)
{
	for (const auto& entry : std::filesystem::directory_iterator(m_state->getFullAssetPath(folder)))
	{
		myVec.push_back(entry.path().u8string());
	}
}

Level::~Level()
{
	destroyGameObjects(m_static_objects);
	destroyGameObjects(m_destructible_objects);
	destroyGameObjects(m_blocks);
}