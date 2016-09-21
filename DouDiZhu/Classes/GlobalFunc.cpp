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

	Poker* getGreaterPoker(const Poker* _poker, int increaseValue /* = 1 */){

		CCASSERT(increaseValue >= 0, "��������ҵ��Ʊȵ�ǰС��");

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
		default:tmp = nullptr;	/* ����17��ʾû���ҵ����еĴ��ڵ�ǰ_poker���ƣ���tmpΪnullptr���� */
		}
		return tmp;
	}
}