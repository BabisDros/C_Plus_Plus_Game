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


	std::unordered_map<void*, std::function<void(const T&...)>> m_actionCallbacks;
public:
	void addArgActionCallback(void* key, std::function<void(const T&...)> cb);
	void removeActionCallback(void* key);
	void trigger(const T&... values);
};

template<typename... T>
inline void CstmCallback<T...>::addArgActionCallback(void* key, std::function<void(const T&...)> cb)
{
	m_actionCallbacks[key] = std::move(cb);
}

template<typename ...T>
inline void CstmCallback<T...>::trigger(const T & ...values)
{
	for (const auto& [key, callback] : m_actionCallbacks) {
		callback(values...);
	}
}

template<typename... T>
inline void CstmCallback<T...>::removeActionCallback(void* key)
{
	m_actionCallbacks.erase(key);
}
