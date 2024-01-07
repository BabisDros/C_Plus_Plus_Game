#pragma once
#include <string>
#include "util.h"
#include "Gamestate.h"
#include "CollisionObject.h"

class LevelBox :public CollisionObject
{
protected:
	const std::string* m_texture;
	bool& m_isDestructible;
public:
	LevelBox(float x, float y, float w, float h, const std::string* texture, bool destructible)
		:CollisionObject(x, y, w, h), m_texture(texture), m_isDestructible(destructible) { init(); }

	void init() override;
	void draw() override;
	const std::string* getTexture();
	bool& getIsDestructible() const;
};