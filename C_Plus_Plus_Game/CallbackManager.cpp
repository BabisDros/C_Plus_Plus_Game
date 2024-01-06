#include "CallbackManager.h"

CallbackManager* CallbackManager::s_unique_instance = nullptr;

CallbackManager* CallbackManager::getInstance()
{
	if (s_unique_instance == nullptr)
	{
		s_unique_instance = new CallbackManager();
	}
	return s_unique_instance;
}
