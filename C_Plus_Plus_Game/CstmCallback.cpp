//#include "CstmCallback.h"
//
//void CstmCallback::addTwoIntActionCallback(std::function<void(const T1&, const T2&)> cb)
//{
//	m_twoIntActionCallbacks.push_back(cb);
//}
//template<typename T1>
//void CstmCallback::addintActionCallback(std::function<void(const int&)> cb)
//{
//	m_intActionCallbacks.push_back(cb);
//}
//
//void CstmCallback::trigger(int val1, int val2)
//{
//	for (const auto& callB : m_twoIntActionCallbacks)
//		callB(val1, val2);
//}
//
//void CstmCallback::trigger(int val1)
//{
//	for (const auto& callB : m_intActionCallbacks)
//		callB(val1);
//}
