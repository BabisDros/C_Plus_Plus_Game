#pragma once
#include <functional>

/*callbacks used for things that happen. E.g Callback playerIsDamaged,Callback buttonPressed.
Then we add actionCallbacks (reactions) to that. E.g UI health to be reduced, sound to be played.
Instead of getting references of every object and call the functions directly we can use callbacks 
to decouple (make more independent) relations between classes.*/
class CstmCallback
{
protected:
	bool m_action_occured = false;
	std::vector<std::function<void(const int&, const int&)>>m_actionCallbacks;

public:
	void addActionCallbackTwoInt (std::function<void(const int&, const int&)> cb)
	{
		m_actionCallbacks.push_back(cb);
	}

	void trigger(int val1,int val2)
	{
		for (const auto& callB : m_actionCallbacks)
			callB(val1, val2);	
	}
};