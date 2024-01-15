#include "LevelManager.h"
#include "Gamestate.h"
#include <iostream>
#include <filesystem>
#include "Level.h"
#include "Player.h"
#include <fstream>
#include <iostream>
#include "CallbackManager.h"

LevelManager* LevelManager::s_unique_instance = nullptr;

void LevelManager::init()
{
	m_state = GameState::getInstance();
	std::string path = m_state->getFullDataPath("");
	CallbackManager::getInstance()->m_playerDied.addArgActionCallback(std::bind(&LevelManager::onPlayerDied, this));

	for (const auto& entry : std::filesystem::directory_iterator(path))	// list of level names on data folder
		levels_list.push_back(entry.path().u8string().erase(entry.path().u8string().find(".txt"), 4).	// remove .txt extention
			erase(0, m_state->getFullDataPath("").size()));	// remove parent directory
	levels_list.erase(std::remove(levels_list.begin(), levels_list.end(), "save file"), levels_list.end());
}

void LevelManager::update(float dt)
{
	if (!m_restart) return;
		restartLevel();
}

LevelManager* LevelManager::getInstance()
{
	if (s_unique_instance == nullptr)
	{
		s_unique_instance = new LevelManager();
	}
	return s_unique_instance;
}

//void LevelManager::nextLevel(bool restartLevel)
//{
//	if (m_state->m_current_level) m_state->m_current_level->~Level();
//	if (!restartLevel)	++m_level_counter;
//	m_state->m_current_level = new Level(levels_list[(m_level_counter) % levels_list.size()]);	// no end level, so loop through list
//	m_state->m_current_level->init();
//
//	if (!m_state->m_player) m_state->m_player = new Player("Player", m_state->getInitialHealth());
//	m_state->m_player->init();
//	//not needed to save in a restart
//	m_state->m_goNextLevel = false;
//
//	if (restartLevel) return;	
//	if (!m_loadingFile) saveData();
//}

void LevelManager::nextLevel(bool restartingLevel)
{
	if (m_state->m_current_level) m_state->m_current_level->~Level();
	if (!restartingLevel && m_level_counter + 1 >= levels_list.size())
	{
		m_win = true;
		m_state->setState(Win);
	}
	else
	{
		if (!restartingLevel) { ++m_level_counter; }
		m_state->m_current_level = new Level(levels_list[(m_level_counter)]);	// no end level, so loop through list
		m_state->m_current_level->init();

		if (!m_state->m_player) m_state->m_player = new Player("Player", m_state->getInitialHealth());
		m_state->m_player->init();
		m_state->m_goNextLevel = false;
		//not needed to save in a restart
		if (restartingLevel) return;
		if (!m_loadingFile) saveData();
	}
}
void LevelManager::restartLevel()
{
	m_state->m_suspendExecution = true;
	m_state->m_player->setInitialHealthValues(m_state->getInitialHealth());
	nextLevel(true);
	m_state->m_pauseButtonPressed = false;
	m_state->m_suspendExecution = false;
	m_restart = false;
	m_state->setState(InGame);
	saveData();
}

void LevelManager::onPlayerDied()
{
	m_restart = true;
}

void LevelManager::saveData()
{
	std::ofstream writer;
	writer.open("data\\save file.txt", std::ofstream::out | std::ofstream::trunc);
	writer << m_state->m_current_level->m_name << std::endl;
	writer << "HP: " << m_state->getPlayer()->getHealth() << std::endl;
	writer << "Lives: " << m_state->m_lives << std::endl;
	writer << "Score: " << m_state->m_points << std::endl;
	writer.close();
}

void LevelManager::loadSaveFile()
{
	std::ifstream saveFile("data\\save file.txt");
	std::string line;
	std::getline(saveFile, line);
	auto itr = std::find(levels_list.begin(), levels_list.end(), line);
	m_level_counter = itr - levels_list.begin() - 1;
	nextLevel();

	std::getline(saveFile, line);	// hp
	m_state->getLevel()->getDataValue(line);
	m_state->getPlayer()->setHealth((stoi(line)));	//! delete if not needed after fixes
	CallbackManager::getInstance()->m_playerHealthChanged.trigger(m_state->getInitialHealth(), stoi(line));


	std::getline(saveFile, line);
	m_state->getLevel()->getDataValue(line);	// lives
	CallbackManager::getInstance()->m_playerLivesChanged.trigger(stoi(line),true);

	std::getline(saveFile, line);
	m_state->getLevel()->getDataValue(line);	// score
	CallbackManager::getInstance()->m_pointsChanged.trigger(std::stoi(line),true);

}
