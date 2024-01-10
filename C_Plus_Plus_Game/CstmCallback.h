#pragma once
#include <functional>
#include <vector>
/*callbacks used for things that happen. E.g Callback playerIsDamaged,Callback buttonPressed.
Then we add actionCallbacks (reactions) to that. E.g UI health to be reduced, sound to be played.
Instead of getting references of every object and call the functions directly we can use callbacks 
to decouple (make more independent) relations between classes.*/

template<typename T1, typename T2>
class CstmCallback
{
protected:
	bool m_action_occured = false;

	std::vector<std::function<void(const T1&, const T2&)>>m_twoArgActionCallbacks;	
	std::vector<std::function<void(const T1&)>>m_argActionCallbacks;

public:
	void addTwoArgActionCallback(std::function<void(const T1&, const T2&)> cb);
	void addArgActionCallback(std::function<void(const T1&)> cb);

	void trigger(T1 val1, T2 val2);
	void trigger(T1 val1);
};

template<typename T1, typename T2>
inline void CstmCallback<T1, T2>::addTwoArgActionCallback(std::function<void(const T1&, const T2&)> cb)
{
	m_twoArgActionCallbacks.push_back(cb);
}

template<typename T1, typename T2>
inline void CstmCallback<T1, T2>::addArgActionCallback(std::function<void(const T1&)> cb)
{
	m_argActionCallbacks.push_back(cb);
}

template<typename T1, typename T2>
inline void CstmCallback<T1, T2>::trigger(T1 val1, T2 val2)
{
	for (const auto& callB : m_twoArgActionCallbacks)
		callB(val1, val2);
}

template<typename T1, typename T2>
inline void CstmCallback<T1, T2>::trigger(T1 val1)
{
	for (const auto& callB : m_argActionCallbacks)
		callB(val1);
}
