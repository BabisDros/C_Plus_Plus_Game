#include "LevelManager.h"
#include "Gamestate.h"
#include <iostream>
#include <filesystem>
#include "Level.h"
#include "Player.h"
#include <fstream>
#include <iostream>

LevelManager* LevelManager::s_unique_instance = nullptr;

void LevelManager::init()
{
	m_state = GameState::getInstance();
	std::string path = m_state->getFullDataPath("");
	for (const auto& entry : std::filesystem::directory_iterator(path))	// list of level names on data folder
		levels_list.push_back(entry.path().u8string().erase(entry.path().u8string().find(".txt"), 4).	// remove .txt extention
			erase(0, m_state->getFullDataPath("").size()));	// remove parent directory
	levels_list.erase(std::remove(levels_list.begin(), levels_list.end(), "save file"), levels_list.end());
}

LevelManager* LevelManager::getInstance()
{
	if (s_unique_instance == nullptr)
	{
		s_unique_instance = new LevelManager();
	}
	return s_unique_instance;
}

void LevelManager::nextLevel()
{
	if (m_state->m_current_level) m_state->m_current_level->~Level();
	m_state->m_current_level = new Level(levels_list[(++m_level_counter) % levels_list.size()]);	// no end level, so loop through list
	m_state->m_current_level->init();

	if (!m_state->m_player) m_state->m_player = new Player("Player", 1000);
	m_state->m_player->init();
	m_state->goNextLevel = false;
	if (!m_loadingFile) saveData();
}

void LevelManager::saveData()
{
	std::ofstream reader;
	reader.open("data\\save file.txt", std::ofstream::out | std::ofstream::trunc);
	reader << m_state->m_current_level->m_name << std::endl;
	reader << "HP: " << m_state->getPlayer()->getHealth();
	// Write more data
	reader.close();
}

void LevelManager::loadSaveFile()
{
	std::ifstream saveFile("data\\save file.txt");
	std::string line;
	std::getline(saveFile, line);
	auto itr = std::find(levels_list.begin(), levels_list.end(), line);
	m_level_counter = itr - levels_list.begin() - 1;
	nextLevel();
	// Read more data 
	// SaveObjectData of Level.cpp might be usefull
	// list of wanted data required, write it below
	// 
	// Set Data to player
	std::getline(saveFile, line);
	m_state->getLevel()->getDataValue(line);
	m_state->getPlayer()->setHealth((stoi(line))) ; //Better setter required
	saveData();
}
