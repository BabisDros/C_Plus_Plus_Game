#include "CstmCallback.h"

void CstmCallback::addTwoIntActionCallback(std::function<void(const int&, const int&)> cb)
{
	m_twoIntActionCallbacks.push_back(cb);
}

void CstmCallback::trigger(int val1, int val2)
{
	for (const auto& callB : m_twoIntActionCallbacks)
		callB(val1, val2);
}
