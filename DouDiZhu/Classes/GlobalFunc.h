#ifndef __GLOBAL_FUNC_H__
#define __GLOBAL_FUNC_H__

#include <algorithm>
#include "GlobalDefine.h"
#include "Poker.h"

namespace GlobalFunc{

	/* 比较函数，根据withType选择是否将牌的花色参与排序，这里是 > 时返回true */
	bool cmpPoker(const Poker* a, const Poker* b, bool withType);

	bool cmpPokerWithType(const Poker* a, const Poker* b);

	bool cmpPokerWithoutType(const Poker* a, const Poker* b);

	void sort(Vector<Poker*>& var);

	/* 查找比_poker大的扑克，increaseValue是比_poker的值大的具体数值，默认是1
		按照3，4，5，6，7，8，9，10，J，Q，K，A，2，小王，大王的顺序递增*/
	Poker* getGreaterPoker(const Poker* _poker, int increaseValue = 1);
}

#endif