#include "GlobalFunc.h"

namespace GlobalFunc{
	bool cmp_sort(const Poker* a, const Poker* b){
		/* 如果是大小王，那么直接判断并返回比较结果 */
		if (a->getPokerType() == REDJOKER) return true;
		else if (b->getPokerType() == REDJOKER) return false;
		else if (a->getPokerType() == BLACKJOKER) return true;
		else if (b->getPokerType() == BLACKJOKER) return false;

		/* 普通的扑克 */
		int aValue = a->getValue() % SINGLETYPECARDNUM;
		int bValue = b->getValue() % SINGLETYPECARDNUM;
		/* 如果扑克的牌面值相同，那么直接根据花色排序 */
		if (aValue == bValue){
			return a->getPokerType() > b->getPokerType();
		}
		else{
			/* 如果扑克的牌面值不等，那么首先将A和2的情况排序，然后根据牌面值大小排序即可 */
			if (aValue == 2) return true;
			else if (bValue == 2) return false;
			else if (aValue == 1) return true;
			else if (bValue == 1) return false;
			else if (aValue == 0) return true;	/* K求余是0，所以这里特殊处理一下*/
			else if (bValue == 0) return false;
			return aValue > bValue;
		}
	}

	void sort(Vector<Poker*>& var){
		std::sort(var.begin(), var.end(), &cmp_sort);
	}

	Poker* getGreaterPoker(const Poker* _poker, int increaseValue /* = 1 */){

		CCASSERT(increaseValue >= 0, "不允许查找的牌比当前小！");

		Poker* tmp = _poker->clone();
		int curValue = _poker->getValue();
		int neededValue = curValue + increaseValue;
		if (neededValue >= 3 && neededValue <= 13){
			tmp->setValue(neededValue);
			return tmp;
		}
		switch (neededValue){
		case 14:tmp->setValue(1); break;
		case 15:tmp->setValue(2); break;
		case 16:tmp->setPokerType(BLACKJOKER); tmp->setValue(0); break;
		case 17:tmp->setPokerType(REDJOKER); tmp->setValue(0); break;
		default:tmp = nullptr;	/* 大于17表示没有找到现有的大于当前_poker的牌，置tmp为nullptr即可 */
		}
		return tmp;
	}
}