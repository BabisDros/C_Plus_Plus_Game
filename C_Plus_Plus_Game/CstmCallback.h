#pragma once
#include <functional>
#include <vector>
/*callbacks used for things that happen. E.g Callback playerIsDamaged,Callback buttonPressed.
Then we add actionCallbacks (reactions) to that. E.g UI health to be reduced, sound to be played.
Instead of getting references of every object and call the functions directly we can use callbacks 
to decouple (make more independent) relations between classes.*/

//using https://en.cppreference.com/w/cpp/language/parameter_pack
template<typename... T>
class CstmCallback
{
protected:
	bool m_action_occured = false;

	std::vector<std::function<void(const T&...)>>m_actionCallbacks;
public:
	void addArgActionCallback(std::function<void(const T&...)> cb);
	void removeArgActionCallback(std::function<void(const T&...)> cb);
	void trigger(const T&... values);
};

template<typename... T>
inline void CstmCallback<T...>::addArgActionCallback(std::function<void(const T&...)> cb)
{
	m_actionCallbacks.push_back(cb);
}

template<typename... T>
inline void CstmCallback<T...>::removeArgActionCallback(std::function<void(const T&...)> cb) 
{
	auto it = std::find(m_actionCallbacks.begin(), m_actionCallbacks.end(), cb);
	if (it != m_actionCallbacks.end()) 
	{
		m_actionCallbacks.erase(it);
	}
}

template<typename ...T>
inline void CstmCallback<T...>::trigger(const T & ...values)
{
	for (const auto& callback : m_actionCallbacks) {
		callback(values...);
	}
}

