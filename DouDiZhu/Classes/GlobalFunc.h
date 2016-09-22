#ifndef __GLOBAL_FUNC_H__
#define __GLOBAL_FUNC_H__

#include <algorithm>
#include "GlobalDefine.h"
#include "Poker.h"

namespace GlobalFunc{

	/* �ȽϺ���������withTypeѡ���Ƿ��ƵĻ�ɫ�������������� > ʱ����true */
	bool cmpPoker(const Poker* a, const Poker* b, bool withType);

	bool cmpPokerWithType(const Poker* a, const Poker* b);

	bool cmpPokerWithoutType(const Poker* a, const Poker* b);

	void sort(Vector<Poker*>& var);

	/* ���ұ�_poker����˿ˣ�increaseValue�Ǳ�_poker��ֵ��ľ�����ֵ��Ĭ����1
		����3��4��5��6��7��8��9��10��J��Q��K��A��2��С����������˳�����*/
	Poker* getGreaterPoker(const Poker* _poker, int increaseValue = 1);
}

#endif