#pragma once
#include <string>
#include "GameObject.h"
class HealthUi
{
public:
	HealthUi();
	~HealthUi();

private:
	std::string bgTexture;
	std::string fillTexture;
};

HealthUi::HealthUi() {}
HealthUi::~HealthUi() {}
