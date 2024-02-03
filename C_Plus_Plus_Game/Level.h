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
	std::vector<LevelBox*> m_background_objects;

	std::map <char, std::vector<std::string>> m_terrain_data;	//? 1-1 correlation with m_terrain_titles
	std::map <char, std::vector<std::string>> m_enemy_data;	//? 1-1 correlation with m_enemy_titles
	std::vector<std::string> m_terrain_titles = { 
		"width",
		"height",
		"texture",
		"destructible",
		"health",
		"level_end",
		"background"};

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
	Level(const std::string& name = "Level 0") : GameObject(name) {};
	~Level();

	void read();	// methods used to read from file
	void removeSpaces(std::string& s);
	void ignoreEmptyLine(std::ifstream& file, std::string& line);
	std::string getDataTitle(std::string s);
	void getDataValue(std::string& s);
	void saveObjectData(std::map <char, std::vector<std::string>>& data, std::vector<std::string> titles, std::ifstream& file, std::string& line);

	std::vector<LevelBox*> getBlocks() const;

	std::list<CollisionObject*> getDestructibleObjects() const;
	std::list<CollisionObject*>* getDestructibleObjectsPtr();	//cheaper

	template <typename Container>
	void destroyGameObjects(Container& myContainer);

	std::vector<std::string>* getFireballSprites();
	void onPointsCollected(int points);
};