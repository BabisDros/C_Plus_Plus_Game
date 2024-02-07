#pragma once
#include "CollisionObject.h"

class LevelBox :public CollisionObject
{
protected:
	std::string* m_texture; // removed const
	bool& m_isDestructible;
public:
	LevelBox(float x, float y, float w, float h, std::string* texture, bool destructible) // removed const
		:CollisionObject(x, y, w, h), m_texture(texture), m_isDestructible(destructible) { init(); }

	void init() override;
	void draw() override;
	std::string* getTexture(); // removed const
	bool& getIsDestructible() const;
	void setTexture(std::string& texture);
};