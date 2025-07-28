#pragma once
//This class is used between all managers (including GameState) to limit repeating code

template <typename className>
class Singleton
{
protected:
	static className* s_unique_instance;
public:
	static className* getInstance();
	static void delptr();
};


template <typename className>
className* Singleton<className>::s_unique_instance = nullptr;

template <typename className>
void Singleton<className>::delptr()
{
	if (s_unique_instance != nullptr)
		delete s_unique_instance;
}

template <typename className>
className* Singleton<className>::getInstance()
{
	if (s_unique_instance == nullptr)
	{
		s_unique_instance = new className();
	}
	return s_unique_instance;
}