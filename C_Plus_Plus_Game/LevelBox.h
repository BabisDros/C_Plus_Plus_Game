#pragma once
#include "box.h"
#include <string>
#include "GameObject.h"

class LevelBox : public GameObject, public Box
{
protected:
	const std::string* texture;
	bool isDestructible;
public:
	LevelBox(float x, float y, float w, float h, const std::string* texture, bool IDestructible)
		:Box(x, y, w, h), texture(texture), isDestructible(IDestructible) {}

	const std::string* getTexture()
	{
		return texture;
	}

	bool getIsDestructible() const
	{
		return isDestructible;
	}
};