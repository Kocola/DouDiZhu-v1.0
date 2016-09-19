#ifndef __GLOBAL_FUNC_H__
#define __GLOBAL_FUNC_H__

#include <algorithm>
#include "GlobalDefine.h"
#include "Poker.h"

namespace GlobalFunc{

	bool cmp_sort(const Poker* a, const Poker* b);

	void sort(Vector<Poker*>& var);
}

#endif