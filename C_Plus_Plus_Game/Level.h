#pragma once
#include "GameObject.h"
#include <sgg/graphics.h>
#include <list>
#include "box.h"
#include <map>
#include <tuple>
#include "LevelBox.h"
#include "CrateDestructible.h"

class Level : public GameObject
{
	float m_center_x = 3.0f;
	float m_center_y = 3.0f;

	//? everything included in the level
	std::vector<GameObject*> m_static_objects;
	//seperate list for destructibles to improve performance. TODO:use a binary search to find elements and then remove destructed
	std::list<GameObject*> m_dynamic_objects;
	std::vector<LevelBox*> m_blocks;
	

	std::map <char, std::tuple <float, float, const std::string , bool>> m_objects_data;	//? For every tag, width, height, texture and is IDestructible are saved

public:
	void init() override;
	void draw() override;
	void update(float dt) override;
	Level(const std::string& name = "Level 0");
	~Level();

	void checkCollisions();
	void pausedDraw();
	void read();

	std::vector<LevelBox*> getBlocks() const;

	std::list<GameObject*> getDynamicObjects() const;


	template <typename Container>
	void destroyGameObjects(Container& myContainer);

};