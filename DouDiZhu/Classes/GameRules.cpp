#include "GameRules.h"
#include "GlobalDefine.h"
#include "GlobalFunc.h"

#define COMPARE_GREATER(a, b) GlobalFunc::cmp_sort(a, b);

Vector<Poker*> GameRules::search(Vector<Poker*> _pokers, PokerValueType pokerValueType, int value /* = 0 */){
	int index = 0;
	while (index < _pokers.size()){
		Vector<Poker*> tmp;	/* ��ʱ��Ž����Vector����ʼʱ��������_pokers����˿� */
		tmp.pushBack(_pokers.at(index));
		tmp.pushBack(_pokers.at(index + 1));
		tmp.pushBack(_pokers.at(index + 2));
		tmp.pushBack(_pokers.at(index + 3));

		if (isBomb(tmp) == true){	/* ���������������ţ���ô�����Բ���ը������ĳ���� */
			/* ������ҵ�������ը��������ը���������value����ô�ҵ���ֱ�ӷ��� */
			if (pokerValueType == BOMB && tmp.at(index)->getValue() >= value) return tmp;
			index += 4;
			tmp.clear();
			continue;
		}

		tmp.popBack();		/* �˳������ӵ��˿ˣ���֤���������жϲ������ */
		if (isTriple(tmp) == true){	/* ���������������ţ���ô�����Բ������ŵ���ĳ���� */
			if (pokerValueType == TRIPLE && tmp.at(index)->getValue() >= value) return tmp;
			index += 3;
			tmp.clear();
			continue;
		}

		tmp.popBack();	/* �˳������ӵ��˿ˣ���֤���������жϲ������ */
		if (isPair(tmp) == true){		/* �ж��Ƿ��Ƕ��� */
			if (pokerValueType == PAIR && tmp.at(index)->getValue() >= value) return tmp;
			index += 2;
			tmp.clear();
			continue;
		}

		tmp.popBack();		/* �˳������ӵ��˿ˣ���֤���������жϲ������ */
		/* �����ǰ��index��Ӧ���Ʋ��ͺ�����ƹ���ը�������źͶ��ӣ���ô
		�ж����Ƿ�����ϼ��������ƣ���valueֵ��������ǣ�����Ѱ��*/
		if (pokerValueType == SINGLE){
			if (tmp.at(index)->getValue() >= value) return tmp;
			index += 1;
			tmp.clear();
		}
	}
	return Vector<Poker*>();		/* ��֤��_pokers����Ϊ��ʱ��������ȷ���� */
}

Vector<Poker*> GameRules::searchSingle(Vector<Poker*> _pokers, int value /* = 0 */){
	return search(_pokers, SINGLE, value);
}

Vector<Poker*> GameRules::searchPair(Vector<Poker*> _pokers, int value /* = 0 */){
	return search(_pokers, PAIR, value);
}

Vector<Poker*> GameRules::searchTriple(Vector<Poker*> _pokers, int value /* = 0 */){
	return search(_pokers, TRIPLE, value);
}

Vector<Poker*> GameRules::searchBomb(Vector<Poker*> _pokers, int value /* = 0 */){
	return search(_pokers, BOMB, value);
}

/* �ú�����ʱ���ã��������ҵ��Ż��ͺϲ��İ취��д */
Vector<Poker*> GameRules::searchStraight(Vector<Poker*> _pokers, int step, int length, int value /* = 0 */){
	CCASSERT(value < 0, "�ú������ɵ���");
	return Vector<Poker*>();
}

Vector<Poker*> GameRules::searchSingleStraight(Vector<Poker*> _pokers, int length, int value /* = 0 */){
	/* ���length�������������˿˵��������������length����ôֱ�ӷ��ؿ� */
	if (length < 5 || length > 12 || _pokers.size() < length) return Vector<Poker*>();
	
	/* ������ҵ�˳�ӵ���С������ֵ��1����2���Լ�����˳���������ֵ+˳�ӳ��ȣ���ʱ����˳����������ֵ��> A��14��,��ôֱ�ӷ��ؿ� */
	if (value == 1 || value == 2 || value + length > 14) return Vector<Poker*>();
	
	
}

bool GameRules::isSingle(Vector<Poker*> _pokers){
	return _pokers.size() == 1;
}

bool GameRules::isPair(Vector<Poker*> _pokers){
	if (_pokers.size() != 2) return false;
	return _pokers.at(0) == _pokers.at(1);
}

bool GameRules::isTriple(Vector<Poker*> _pokers){
	if (_pokers.size() != 3) return false;
	return (_pokers.at(0)->getValue() == _pokers.at(1)->getValue()) 
		&& (_pokers.at(0)->getValue() == _pokers.at(2)->getValue());
}

bool GameRules::isBomb(Vector<Poker*> _pokers){
	if (_pokers.size() != 4) return false;
	return(_pokers.at(0)->getValue() == _pokers.at(1)->getValue())
		&& (_pokers.at(0)->getValue() == _pokers.at(2)->getValue())
		&& (_pokers.at(0)->getValue() == _pokers.at(3)->getValue());
}

bool GameRules::isKingBomb(Vector<Poker*> _pokers){
	if (_pokers.size() != 2) return false;
	PokerType type_1 = _pokers.at(0)->getPokerType();
	PokerType type_2 = _pokers.at(1)->getPokerType();
	return (type_1 == REDJOKER || type_1 == BLACKJOKER)
		&& (type_2 == REDJOKER || type_2 == BLACKJOKER);
}

bool GameRules::isStraight(Vector<Poker*> _pokers, int step /* = 1 */){
	GlobalFunc::sort(_pokers);	/* ���˿˽������� */

	auto _first_poker = _pokers.at(0);
	/* �����һ������������2����ô�϶�����˳�� */
	if (_first_poker->getValue() == 2
		|| _first_poker->getPokerType() == BLACKJOKER
		|| _first_poker->getPokerType() == REDJOKER){
		return false;
	}

	auto secondpoker_index = 0 + step;	/* �ڶ����Ƶ�����������ĵڶ����ǲ�ͬ��ֵ�ĵڶ��ţ�����AAKK����ô�ڶ�����K */

	if (_first_poker->getValue() == 1){
		if (_pokers.at(secondpoker_index)->getValue() != 13){
			return false;
		}else{
			for (int i = secondpoker_index; i < _pokers.size() - step; i += step){
				if (_pokers.at(i)->getValue() != (_pokers.at(i + step)->getValue() + 1)){
					return false;
				}
			}
			return true;
		}
	}else{
		for (int i = 0; i < _pokers.size() - step; i += step){
			if (_pokers.at(i)->getValue() != (_pokers.at(i + step)->getValue() + 1)){
				return false;
			}
		}
		return true;
	}
}

bool GameRules::isPairStraight(Vector<Poker*> _pokers){
	return isStraight(_pokers, 2);
}

bool GameRules::isTripleStraight(Vector<Poker*> _pokers){
	return isStraight(_pokers, 3);
}