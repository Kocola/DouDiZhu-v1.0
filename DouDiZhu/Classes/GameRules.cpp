#include "GameRules.h"
#include "GlobalDefine.h"
#include "GlobalFunc.h"
#include "OutCards.h"

#define COMPARE_GREATER(a, b) GlobalFunc::cmpPokerWithoutType(a, b)
#define COMPATE_EQUAL(a, b) (!GlobalFunc::cmpPokerWithoutType(a, b) && !GlobalFunc::cmpPokerWithoutType(b, a))

GameRules* GameRules::gameRules = nullptr;

GameRules* GameRules::getInstance(){
	if (gameRules == nullptr){
		gameRules = new GameRules();
		if (gameRules != nullptr && gameRules->init()){
			gameRules->autorelease();
			gameRules->retain();	/* ����Ϊ�˷�ֹ���ڴ���������գ���Ҫ����retaiin��ֹ���� */
		}else{
			CC_SAFE_DELETE(gameRules);
		}
	}
	return gameRules;
}

bool GameRules::init(){
	return true;
}

Vector<Poker*> GameRules::searchProperPokers(Vector<Poker*> _pokers){
	Vector<Poker*> ret;
	/* ���ҵ��� */
	ret = calcPokerWithValueType(_pokers, SINGLE);
	if (ret.size() == 0){	/* û�ҵ����� */
		ret = calcPokerWithValueType(_pokers, PAIR);
		if (ret.size() == 0){
			ret = calcPokerWithValueType(_pokers, TRIPLE);
			if (ret.size() == 0){
				ret = calcPokerWithValueType(_pokers, BOMB);
			}
		}
	}
	return ret;
}

bool  GameRules::canOutCards(Vector<Poker*> curCards, const OutCards* lastOutCards){
	GlobalFunc::sort(curCards);
	/* ����֮һ����ը */
	if (isKingBomb(curCards) == true) return true;
	if (lastOutCards->getPokerValueType() == KINGBOMB) return false;
	/* ����֮һ��ը�� */
	if (lastOutCards->getPokerValueType() == BOMB){
		if (isBomb(curCards) == BOMB && COMPARE_GREATER(curCards.at(curCards.size() - 1), lastOutCards->getLowestPoker())){
			return true;
		}else{
			return false;
		}
	} else if (isBomb(curCards) == true){
		return true;
	}
	/* ���⣬�������ͱ���һ�� */
	/* ���Ͳ�һ�£�����false */
	if (analysePokerValueType(curCards) != lastOutCards->getPokerValueType()){
		return false;
	}
	/* ��������һ��ʱ */
	PokerValueType _pokerValueType = lastOutCards->getPokerValueType();
	/* �����ĳ��˳�� */
	if (_pokerValueType == STRAIGHT || _pokerValueType == PAIRSRAIGHT || _pokerValueType == TRIPLESTRAIGHT){
		return (lastOutCards->getTotalLength() == curCards.size()) && (COMPARE_GREATER(curCards.at(curCards.size() - 1), lastOutCards->getLowestPoker()));
	} 
	/* ���ţ����ӣ����� ���Ժ���һ���ϲ�����ʱ����ô��*/
	return (lastOutCards->getTotalLength() == curCards.size()) && (COMPARE_GREATER(curCards.at(curCards.size() - 1), lastOutCards->getLowestPoker()));
}

PokerValueType GameRules::analysePokerValueType(Vector<Poker*> _pokers){
	if (_pokers.size() == 0) return NONE;

	/* ÿ������һ�Σ���ֹ���� */
	GlobalFunc::sort(_pokers);

	if (_pokers.size() < 5){
		if (isSingle(_pokers) == true) return SINGLE;
		if (isKingBomb(_pokers) == true) return KINGBOMB;		/* ˫����valueֵ��0�����������ж϶��ӻ�����жϴ��� */
		if (isPair(_pokers) == true) return PAIR;
		if (isTriple(_pokers) == true) return TRIPLE;
		if (isBomb(_pokers) == true) return BOMB;
	}else{
		if (isSingleStraight(_pokers) == true) return STRAIGHT;
		if (isPairStraight(_pokers) == true) return PAIRSRAIGHT;
		if (isTripleStraight(_pokers) == true) return TRIPLESTRAIGHT;
	}

	return NONE;
}

bool GameRules::isPokerValueType(Vector<Poker*> _pokers){
	return !(analysePokerValueType(_pokers) == NONE);
}

bool GameRules::isSpecifiedPokerValueType(Vector<Poker*> _pokers, PokerValueType pokerValueType){
	/* ÿ������һ�Σ���ֹ���� */
	GlobalFunc::sort(_pokers);

	switch (pokerValueType){
	case NONE: return false; break;
	case SINGLE: return isSingle(_pokers); break;
	case PAIR: return isPair(_pokers); break;
	case TRIPLE: return isTriple(_pokers); break;
	case BOMB: return isBomb(_pokers); break;
	case KINGBOMB: return isKingBomb(_pokers); break;
	case STRAIGHT: return isSingleStraight(_pokers); break;
	case PAIRSRAIGHT: return isPairStraight(_pokers); break;
	case TRIPLESTRAIGHT: return isTripleStraight(_pokers); break;
	default: return false; break;
	}
	return false;
}

Vector<Poker*> GameRules::calcPokerWithValueType(Vector<Poker*> _pokers, PokerValueType pokerValueType, const Poker* _poker /* = nullptr */, int length /* = 0 */){
	/* ÿ������һ�Σ���ֹ���� */
	GlobalFunc::sort(_pokers);

	switch (pokerValueType){
	case SINGLE: return searchSingle(_pokers, _poker); break;
	case PAIR: return searchPair(_pokers, _poker); break;
	case TRIPLE: return searchTriple(_pokers, _poker); break;
	case BOMB: return searchBomb(_pokers, _poker); break;
	case KINGBOMB: return searchKingBomb(_pokers); break;
	case STRAIGHT: return searchSingleStraight(_pokers, length, _poker); break;
	case PAIRSRAIGHT: return searchPairStraight(_pokers, length, _poker); break;
	case TRIPLESTRAIGHT: return searchTripleStraight(_pokers, length, _poker); break;
	default: break;
	}
	return Vector<Poker*>();
}

Vector<Poker*> GameRules::searchSingle(Vector<Poker*> _pokers, const Poker* _poker /* = nullptr */){

	if (_pokers.size() == 0) return Vector<Poker*>();

	int index = _pokers.size() - 1;	/* ��Ϊ����Ӵ�С����ģ������Ҫ�Ӹ�λ������ */
	///* ������Ҫ�Ƚϵ�poker������������˿ˣ�һ���Ǵ�1���� */
	//Poker* searchPoker = GlobalFunc::searchGreaterPoker(_poker, 1);
	while (index >= 0){
		Vector<Poker*> tmp;
		tmp.pushBack(_pokers.at(index));
		/* �����ж��Ƿ�����ֵ����Ҫ�ҵ��ƣ�����ǣ��ǾͿ��Խ�һ���ж��Ƕ��ӣ����ţ�ը���ȣ��������Լ�������� 
			�ж϶��ӵȵ������Ƿ�ֹ���ƣ��ڷ�˳�ӵ�ǰ���£������Բ���*/
		if (_poker == nullptr || COMPARE_GREATER(tmp.at(0), _poker) == true){
			if (index - 1 >= 0){
				tmp.pushBack(_pokers.at(index - 1));
				if (isPair(tmp) == true){
					if (index - 2 >= 0){
						tmp.pushBack(_pokers.at(index - 2));
						if (isTriple(tmp) == true){
							if (index - 3 >= 0){
								tmp.pushBack(_pokers.at(index - 3));
								if (isBomb(tmp) == true){
									index = index - 4;
									continue;
								}
								tmp.popBack();	/* �������ը������ôҪ��ǰ���һ��popBack */
							}
							index = index - 3;
							continue;
						}
						tmp.popBack();/* ����������ţ���ôҪ��ǰ���һ��popBack  */
					}
					index = index - 2;
					continue;
				}
				tmp.popBack();	/* ������Ƕ��ӣ���ôҪ��ǰ���һ��popBack  */
			}
			return tmp;
		}
		index = index - 1;
		tmp.clear();
	}
	/* ���ڵ�һ�����⣬Ϊ�˲����ƣ����ܳ���333444555�������ʱ����������� */
	return Vector<Poker*>();
}

Vector<Poker*> GameRules::searchPair(Vector<Poker*> _pokers, const Poker* _poker /* = nullptr */){
	if (_pokers.size() < 2) return Vector<Poker*>();

	int index = _pokers.size() - 1;
	/* >= 1��Ŀ���ǣ���֤ÿ��ѭ�������������� */
	while (index >= 1){
		Vector<Poker*> tmp;
		tmp.pushBack(_pokers.at(index));
		tmp.pushBack(_pokers.at(index - 1));
		if (isPair(tmp) == true && (_poker == nullptr || COMPARE_GREATER(tmp.at(0), _poker) == true)){
			if (index - 2 >= 0){
				tmp.pushBack(_pokers.at(index - 2));
				if (isTriple(tmp) == true){
					if (index - 3 >= 0){
						tmp.pushBack(_pokers.at(index - 3));
						if (isBomb(tmp) == true){
							index = index - 4;
							continue;
						}
						tmp.popBack();	/* �������ը������ôҪ��ǰ���һ��popBack */
					}
					index = index - 3;
					continue;
				}
				tmp.popBack(); /* ����������ţ���ôҪ��ǰ���һ��popBack */
			}
			return tmp;
		}
		index = index - 1;
	}
	return Vector<Poker*>();
}

Vector<Poker*> GameRules::searchTriple(Vector<Poker*> _pokers, const Poker* _poker /* = nullptr */){
	if (_pokers.size() < 3) return Vector<Poker*>();

	int index = _pokers.size() - 1;
	while (index >= 2){
		Vector<Poker*> tmp;
		tmp.pushBack(_pokers.at(index));
		tmp.pushBack(_pokers.at(index - 1));
		tmp.pushBack(_pokers.at(index - 2));

		if (isTriple(tmp) == true && (_poker == nullptr || COMPARE_GREATER(tmp.at(0), _poker) == true)){
			if (index - 3 >= 0){
				tmp.pushBack(_pokers.at(index - 3));
				if (isBomb(tmp) == true){
					index = index - 4;
					continue;
				}
				tmp.popBack();/* �������ը������ôҪ��ǰ���һ��popBack */
			}
			return tmp;
		}
		index = index - 1;
	}
	return Vector<Poker*>();
}

Vector<Poker*> GameRules::searchBomb(Vector<Poker*> _pokers, const Poker* _poker /* = nullptr */){
	if (_pokers.size() < 4) return Vector<Poker*>();

	int index = _pokers.size() - 1;
	while (index >= 3){
		Vector<Poker*> tmp;
		tmp.pushBack(_pokers.at(index));
		tmp.pushBack(_pokers.at(index - 1));
		tmp.pushBack(_pokers.at(index - 2));
		tmp.pushBack(_pokers.at(index - 3));
		if (isBomb(tmp) == true && (_poker == nullptr || COMPARE_GREATER(tmp.at(0), _poker) == true)){
			return tmp;
		}
		index = index - 1;
	}
	/* ���û���ҵ�ը�������Բ�����ը */
	return searchKingBomb(_pokers);
}

Vector<Poker*> GameRules::searchKingBomb(Vector<Poker*> _pokers){
	if (_pokers.size() < 2) return Vector<Poker*>();	/* ����˿˵�����С��2����ô�϶�������ը */
	/* �������϶�����������ĵ�һ�ڶ��ţ����ֱ�ӽ�ǰ�����Ʒ���һ��Vector������isKingBomb�������ж� */
	Vector<Poker*> tmp;
	tmp.pushBack(_pokers.at(0));
	tmp.pushBack(_pokers.at(1));
	bool isBomb = isKingBomb(tmp);
	return isBomb == true ? tmp : Vector<Poker*>();
}

/* ���ҹ̶�ֵ���˿� */
Vector<Poker*> GameRules::searchSpecifiedSingle(Vector<Poker*> _pokers, const Poker* _poker){
	if (_pokers.size() < 1 || _poker == nullptr) return Vector<Poker*>();
	int index = _pokers.size() - 1;
	while (index >= 0){
		Vector<Poker*> tmp;
		tmp.pushBack(_pokers.at(index));
		if (COMPATE_EQUAL(tmp.at(0), _poker) == true) return tmp;
		/* �����ǰ�˿��Ѿ�����Ҫ�ҵ��˿ˣ���ôֱ��break */
		if (COMPARE_GREATER(tmp.at(0), _poker) == true) break;
		index = index - 1;
	}
	return Vector<Poker*>();
}

Vector<Poker*> GameRules::searchSpecifiedPair(Vector<Poker*> _pokers, const Poker* _poker){
	if (_pokers.size() < 2 || _poker == nullptr) return Vector<Poker*>();
	int index = _pokers.size() - 1;
	while (index >= 1){
		Vector<Poker*> tmp;
		tmp.pushBack(_pokers.at(index));
		tmp.pushBack(_pokers.at(index - 1));
		if (isPair(tmp) == true){
			if (COMPATE_EQUAL(tmp.at(0), _poker) == true) return tmp;
			/* �����ǰ�˿��Ѿ�����Ҫ�ҵ��˿ˣ���ôֱ��break */
			if (COMPARE_GREATER(tmp.at(0), _poker) == true) break;
			index = index - 2;
		}else{
			index = index - 1;
		}
	}
	return Vector<Poker*>(); 
}

Vector<Poker*> GameRules::searchSpecifiedTriple(Vector<Poker*> _pokers, const Poker* _poker){
	if (_pokers.size() < 3 || _poker == nullptr) return Vector<Poker*>();

	int index = _pokers.size() - 1;
	while (index >= 2){
		Vector<Poker*> tmp;
		tmp.pushBack(_pokers.at(index));
		tmp.pushBack(_pokers.at(index - 1));
		tmp.pushBack(_pokers.at(index - 2));
		if (isTriple(tmp) == true){
			if (COMPATE_EQUAL(tmp.at(0), _poker) == true) return tmp;
			if (COMPARE_GREATER(tmp.at(0), _poker) == true) break; 
			index = index - 3;
		}
		index = index - 1;
	}
	return Vector<Poker*>();
}

/* �ú�����ʱ���ã��������ҵ��Ż��ͺϲ��İ취��д */
//Vector<Poker*> GameRules::searchStraight(Vector<Poker*> _pokers, int step, int length, int value /* = 0 */){
//	CCASSERT(value < 0, "�ú������ɵ���");
//	return Vector<Poker*>();
//}

Vector<Poker*> GameRules::searchSingleStraight(Vector<Poker*> _pokers, int length, const Poker* _poker){
	/* ���length�������������˿˵��������������length����ôֱ�ӷ��ؿ� */
	if (length < 5 || length > 12 || _pokers.size() < length) return Vector<Poker*>();
	/*CCASSERT(length < 5 || length > 12, "˳�ӵĳ��ȱ�����5~12֮ǰ");
	CCASSERT(_pokers.size() < length, "ʣ����˿���������ɷ���������˳��");*/
	
	/* ������ҵ�˳�ӵ���С������ֵ��1����2���Լ�����˳���������ֵ+˳�ӳ��ȣ���ʱ����˳����������ֵ��> A��14��,��ôֱ�ӷ��ؿ� */
	int pokerValue = _poker->getValue();
	/* ������ҵ���ֵ��0�������Ǵ�������С�� */
	if (pokerValue == 0 || pokerValue == 1 || pokerValue == 2 || (pokerValue + length > 14)) return Vector<Poker*>();
	
	int index = pokerValue + 1;
	Vector<Poker*> ret;
	while (index <= (14 - length + 1)){
		for (int i = 0; i < length; ++i, ++index){
			auto tmpPoker = GlobalFunc::getGreaterPoker(_poker, index - pokerValue);
			Vector<Poker*> tmp = searchSpecifiedSingle(_pokers, tmpPoker);
			if (tmp.size() != 0){
				ret.pushBack(tmp.at(0));
			}else{
				ret.clear();
				break;
			}
		}
		if (ret.size() == length) return ret;
		index++; /* ��ǰ�Ѿ�����ʧ�ܣ���index+1������һ����ʼ���� */
	}
	return Vector<Poker*>();
}

/* length������˳�ӵĲ�ͬ�Ƶĸ��� */
Vector<Poker*> GameRules::searchPairStraight(Vector<Poker*> _pokers, int length, const Poker* _poker){
	if (length < 3 || length > 12 || _pokers.size() < length * 2) return Vector<Poker*>();
	int pokerValue = _poker->getValue();
	if (pokerValue == 0 || pokerValue == 1 || pokerValue == 2 || (pokerValue + length > 14)) return Vector<Poker*>();

	int index = pokerValue + 1;
	Vector<Poker*> ret;
	while (index <= (14 - length + 1)){
		for (int i = 0; i < length; ++i, ++index){
			auto tmpPoker = GlobalFunc::getGreaterPoker(_poker, index - pokerValue);
			Vector<Poker*> tmp = searchSpecifiedPair(_pokers, tmpPoker);
			if (tmp.size() != 0){
				ret.pushBack(tmp.at(0));
				ret.pushBack(tmp.at(1));
			} else{
				ret.clear();
				break;
			}
		}
		if (ret.size() == length * 2) return ret;
		index++; /* ��ǰ�Ѿ�����ʧ�ܣ���index+1������һ����ʼ���� */
	}
	return Vector<Poker*>();
}

/* length������˳�ӵĲ�ͬ�Ƶĸ��� */
Vector<Poker*> GameRules::searchTripleStraight(Vector<Poker*> _pokers, int length, const Poker* _poker){
	if (length < 2 || length > 12 || _pokers.size() < length * 3) return Vector<Poker*>();
	int pokerValue = _poker->getValue();
	if (pokerValue == 0 || pokerValue == 1 || pokerValue == 2 || (pokerValue + length > 14)) return Vector<Poker*>();

	int index = pokerValue + 1;
	Vector<Poker*> ret;
	while (index <= (14 - length + 1)){
		for (int i = 0; i < length; ++i, ++index){
			auto tmpPoker = GlobalFunc::getGreaterPoker(_poker, index - pokerValue);
			Vector<Poker*> tmp = searchSpecifiedTriple(_pokers, tmpPoker);
			if (tmp.size() != 0){
				ret.pushBack(tmp.at(0));
				ret.pushBack(tmp.at(1));
				ret.pushBack(tmp.at(2));
			}
			else{
				ret.clear();
				break;
			}
		}
		if (ret.size() == length * 3) return ret;
		index++; /* ��ǰ�Ѿ�����ʧ�ܣ���index+1������һ����ʼ���� */
	}
	return Vector<Poker*>();
}

bool GameRules::isSingle(Vector<Poker*> _pokers){
	return _pokers.size() == 1;
}

bool GameRules::isPair(Vector<Poker*> _pokers){
	/* ����˿˵�valueֵС��1��˵��������ֱ�ӷ���false */
	if (_pokers.size() != 2 || _pokers.at(0)->getValue() <= 0) return false;
	return _pokers.at(0)->getValue() == _pokers.at(1)->getValue();
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

bool GameRules::isSingleStraight(Vector<Poker*> _pokers){
	GlobalFunc::sort(_pokers);	/* ���˿˽������� */

	auto _first_poker = _pokers.at(0);
	/* �����һ������������2����ô�϶�����˳�� */
	if (_first_poker->getValue() == 2
		|| _first_poker->getPokerType() == BLACKJOKER
		|| _first_poker->getPokerType() == REDJOKER){
		return false;
	}

	if (_first_poker->getValue() == 1){
		if (_pokers.at(1)->getValue() != 13){
			return false;
		}else{
			for (int i = 1; i < _pokers.size() - 1; i += 1){
				if (_pokers.at(i)->getValue() != (_pokers.at(i + 1)->getValue() + 1)){
					return false;
				}
			}
			return true;
		}
	}else{
		for (int i = 0; i < _pokers.size() - 1; i += 1){
			if (_pokers.at(i)->getValue() != (_pokers.at(i + 1)->getValue() + 1)){
				return false;
			}
		}
		return true;
	}
}

bool GameRules::isPairStraight(Vector<Poker*> _pokers){
	/* ����˿�������ż������ô�϶�����˫˳ */
	if (_pokers.size() % 2 != 0) return false;

	GlobalFunc::sort(_pokers);	/* ���˿˽������� */

	auto _first_poker = _pokers.at(0);
	/* �����һ������������2����ô�϶�����˳�� */
	if (_first_poker->getValue() == 2
		|| _first_poker->getPokerType() == BLACKJOKER
		|| _first_poker->getPokerType() == REDJOKER){
		return false;
	}
	/* �ж��Ƿ��Ƕ��� */
	for (int i = 0; i <= _pokers.size() - 2; i += 2){
		if (isPair(GlobalFunc::createVectorPokerWithIndex(_pokers, i, i + 2)) == false){
			return false;
		}
	}

	if (_first_poker->getValue() == 1){
		if (_pokers.at(2)->getValue() != 13){
			return false;
		}
		else{
			for (int i = 2; i < _pokers.size() - 2; i += 2){
				if (_pokers.at(i)->getValue() != (_pokers.at(i + 2)->getValue() + 1)){
					return false;
				}
			}
			return true;
		}
	}
	else{
		for (int i = 0; i < _pokers.size() - 2; i += 2){
			if (_pokers.at(i)->getValue() != (_pokers.at(i + 2)->getValue() + 1)){
				return false;
			}
		}
		return true;
	}
}

bool GameRules::isTripleStraight(Vector<Poker*> _pokers){
	GlobalFunc::sort(_pokers);	/* ���˿˽������� */

	auto _first_poker = _pokers.at(0);
	/* �����һ������������2����ô�϶�����˳�� */
	if (_first_poker->getValue() == 2
		|| _first_poker->getPokerType() == BLACKJOKER
		|| _first_poker->getPokerType() == REDJOKER){
		return false;
	}
	/* �ж��Ƿ��Ƕ��� */
	for (int i = 0; i <= _pokers.size() - 3; i += 3){
		if (isTriple(GlobalFunc::createVectorPokerWithIndex(_pokers, i, i + 3)) == false){
			return false;
		}
	}

	if (_first_poker->getValue() == 1){
		if (_pokers.at(3)->getValue() != 13){
			return false;
		}
		else{
			for (int i = 3; i < _pokers.size() - 3; i += 3){
				if (_pokers.at(i)->getValue() != (_pokers.at(i + 3)->getValue() + 1)){
					return false;
				}
			}
			return true;
		}
	}
	else{
		for (int i = 0; i < _pokers.size() - 3; i += 3){
			if (_pokers.at(i)->getValue() != (_pokers.at(i + 3)->getValue() + 1)){
				return false;
			}
		}
		return true;
	}
}