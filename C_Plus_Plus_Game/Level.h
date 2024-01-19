#pragma once
#include "GameObject.h"
#include <sgg/graphics.h>
#include <map>
#include "Enemy.h"

class Level : public GameObject
{
	float m_center_x = 3.0f;
	float m_center_y = 3.0f;
	int pointsGainedInLevel = 0;
	/*/Seperate list for destructibles to improve performance. 
	TODO: use a binary search to find elements and then remove destructed*/
	std::list<CollisionObject*> m_destructible_objects;
	std::vector<LevelBox*> m_blocks;
	std::thread t1;

	std::map <char, std::vector<std::string>> m_terrain_data;	//? For every tag, width, height, texture and is IDestructible are saved
	std::map <char, std::vector<std::string>> m_enemy_data;	//? for every tag, width, height, texture, health, territory_x and territory_y are saved
	std::vector<std::string> m_terrain_titles = { 
		"width",
		"height",
		"texture",
		"destructible",
		"health",
		"level_end"};

	std::vector<std::string> m_enemy_titles = {
		"width",
		"height",
		"texture",
		"health",
		"ranged",
		"body_damage",
		"jumping",
		"looking",
		"stick_to_wall",
		"territory_x",
		"territory_y",
		"movement"};
	
	char m_level_end_tag;
	LevelBox *m_level_end;
public:
	float m_player_start_x;
	float m_player_start_y;

	void init() override;
	void draw() override;
	void update(float dt) override;
	Level(const std::string& name = "Level 0");
	~Level();

	void read();	// methods used to read from file
	void removeSpaces(std::string& s);
	void ignoreEmptyLine(std::ifstream& file, std::string& line);
	std::string getDataTitle(std::string s);
	void getDataValue(std::string& s);
	void saveObjectData(std::map <char, std::vector<std::string>>& data, std::vector<std::string> titles, std::ifstream& file, std::string& line);

	std::vector<LevelBox*> getBlocks() const;

	std::list<CollisionObject*> getDestructibleObjects() const;
	std::list<CollisionObject*>* getDestructibleObjectsPtr();

	template <typename Container>
	void destroyGameObjects(Container& myContainer);
	/*	Not used currently cause of new optimized parallelization 
	void updateDynamicBounded(std::_List_iterator < std::_List_val < std::_List_simple_types<CollisionObject*>>>  start, 
		std::_List_iterator < std::_List_val < std::_List_simple_types<CollisionObject*>>> end, float dt);
	*/
	std::vector<std::string>* getFireballSprites();
	void onPointsCollected(int points);
};