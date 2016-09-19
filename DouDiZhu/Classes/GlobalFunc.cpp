#include "GlobalFunc.h"

namespace GlobalFunc{
	bool cmp_sort(const Poker* a, const Poker* b){
		/* ����Ǵ�С������ôֱ���жϲ����رȽϽ�� */
		if (a->getPokerType() == REDJOKER) return true;
		else if (b->getPokerType() == REDJOKER) return false;
		else if (a->getPokerType() == BLACKJOKER) return true;
		else if (b->getPokerType() == BLACKJOKER) return false;

		/* ��ͨ���˿� */
		int aValue = a->getValue() % SINGLETYPECARDNUM;
		int bValue = b->getValue() % SINGLETYPECARDNUM;
		/* ����˿˵�����ֵ��ͬ����ôֱ�Ӹ��ݻ�ɫ���� */
		if (aValue == bValue){
			return a->getPokerType() > b->getPokerType();
		}
		else{
			/* ����˿˵�����ֵ���ȣ���ô���Ƚ�A��2���������Ȼ���������ֵ��С���򼴿� */
			if (aValue == 2) return true;
			else if (bValue == 2) return false;
			else if (aValue == 1) return true;
			else if (bValue == 1) return false;
			else if (aValue == 0) return true;	/* K������0�������������⴦��һ��*/
			else if (bValue == 0) return false;
			return aValue > bValue;
		}
	}

	void sort(Vector<Poker*>& var){
		std::sort(var.begin(), var.end(), &cmp_sort);
	}
}