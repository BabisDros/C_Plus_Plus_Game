#include "LevelManager.h"
#include <filesystem>
#include "Level.h"
#include "Player.h"
#include <fstream>
#include "CallbackManager.h"
#include "MusicManager.h"

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

void LevelManager::nextLevel(bool restartingLevel)
{
	if (m_state->m_current_level) m_state->m_current_level->~Level();
	if (!restartingLevel && m_level_counter + 1 >= levels_list.size())
	{
		m_state->setState(Win);
	}
	else
	{
		if (!restartingLevel) { ++m_level_counter; }
		m_state->m_current_level = new Level(levels_list[(m_level_counter)]);
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
}

void LevelManager::onPlayerDied()
{
	m_restart = true;
	m_state->getPlayer()->setPushedState(false);
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
	if (saveFile.is_open())
	{
		std::getline(saveFile, line);
		auto itr = std::find(levels_list.begin(), levels_list.end(), line);
		m_level_counter = itr - levels_list.begin() - 1;
		nextLevel();

		std::getline(saveFile, line);	// hp
		m_state->getLevel()->getDataValue(line);
		m_state->getPlayer()->setHealth((stoi(line)));
		CallbackManager::getInstance()->m_playerHealthChanged.trigger(m_state->getInitialHealth(), stoi(line));

		std::getline(saveFile, line);
		m_state->getLevel()->getDataValue(line);	// lives
		CallbackManager::getInstance()->m_playerLivesChanged.trigger(stoi(line), true);

		std::getline(saveFile, line);
		m_state->getLevel()->getDataValue(line);	// score
		CallbackManager::getInstance()->m_pointsChanged.trigger(std::stoi(line), true);
	}
	else
	{
		nextLevel();
	}
}

void LevelManager::levelEndCutscene()
{
	MusicManager::getInstance()->playDoorSound();
	m_cutscene_ended = false;
	m_state->getPlayer()->forceStop();
	LevelBox* door = m_state->getLevel()->getLevelEnd();
	if (!m_cutscene_move_player)
	{
		if (m_animation_timer == 0) m_animation_timer = *GameState::getInstance()->getPausableClock();
		
		int index = 0;
		int size = m_state->m_door_sprites.size();
		float dif = *GameState::getInstance()->getPausableClock() - m_animation_timer;	// change texture
		index = (int)(2 * dif) % (m_state->m_door_sprites).size();
		door->setTexture((m_state->m_door_sprites).at(index));

		if (index == size - 1)
		{
			m_animation_timer == 0;
			m_cutscene_move_player = true;
		}

		m_state->getPlayer()->setAnimation(AnimationSequence::Idle);
	}
	else
	{
		if (fabs(door->m_pos_x - m_state->getPlayer()->m_pos_x) < 0.125)
		{
			m_cutscene_ended = true;
			m_cutscene_move_player = false;
			m_state->getPlayer()->allowPlayerMovement();
			MusicManager::getInstance()->m_playing_door_sound = false;
		}
		else if (door->m_pos_x < m_state->getPlayer()->m_pos_x)
		{
			m_state->getPlayer()->moveLeft();
		}
		else
		{
			m_state->getPlayer()->moveRight();
		}

	}
}

void LevelManager::levelStartCutscene()
{
}
