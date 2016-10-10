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
			gameRules->retain();	/* 单例为了防止被内存管理器回收，需要主动retaiin防止回收 */
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
	/* 先找单张 */
	ret = calcPokerWithValueType(_pokers, SINGLE);
	if (ret.size() == 0){	/* 没找到单张 */
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
	/* 两者之一是王炸 */
	if (isKingBomb(curCards) == true) return true;
	if (lastOutCards->getPokerValueType() == KINGBOMB) return false;
	/* 两者之一是炸弹 */
	if (lastOutCards->getPokerValueType() == BOMB){
		if (isBomb(curCards) == BOMB && COMPARE_GREATER(curCards.at(curCards.size() - 1), lastOutCards->getLowestPoker())){
			return true;
		}else{
			return false;
		}
	} else if (isBomb(curCards) == true){
		return true;
	}
	/* 此外，两者类型必须一致 */
	/* 类型不一致，返回false */
	if (analysePokerValueType(curCards) != lastOutCards->getPokerValueType()){
		return false;
	}
	/* 两者牌型一致时 */
	PokerValueType _pokerValueType = lastOutCards->getPokerValueType();
	/* 如果是某种顺子 */
	if (_pokerValueType == STRAIGHT || _pokerValueType == PAIRSRAIGHT || _pokerValueType == TRIPLESTRAIGHT){
		return (lastOutCards->getTotalLength() == curCards.size()) && (COMPARE_GREATER(curCards.at(curCards.size() - 1), lastOutCards->getLowestPoker()));
	} 
	/* 单张，对子，三张 可以和上一条合并，暂时不这么做*/
	return (lastOutCards->getTotalLength() == curCards.size()) && (COMPARE_GREATER(curCards.at(curCards.size() - 1), lastOutCards->getLowestPoker()));
}

PokerValueType GameRules::analysePokerValueType(Vector<Poker*> _pokers){
	if (_pokers.size() == 0) return NONE;

	/* 每次排序一次，防止乱序 */
	GlobalFunc::sort(_pokers);

	if (_pokers.size() < 5){
		if (isSingle(_pokers) == true) return SINGLE;
		if (isKingBomb(_pokers) == true) return KINGBOMB;		/* 双王的value值是0，因此如果先判断对子会造成判断错误 */
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
	/* 每次排序一次，防止乱序 */
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
	/* 每次排序一次，防止乱序 */
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

Vector<Poker*> GameRules::calcPokerWithValueTypeInSplit(Vector<Poker*> _pokers, PokerValueType pokerValueType, const Poker* _poker /* = nullptr */, int length /* = 0 */){
	/* 每次排序一次，防止乱序 */
	GlobalFunc::sort(_pokers);

	switch (pokerValueType){
	case SINGLE: return searchSingleInSplit(_pokers, _poker); break;
	case PAIR: return searchPairInSplit(_pokers, _poker); break;
	case TRIPLE: return searchTripleInSplit(_pokers, _poker); break;
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

	int index = _pokers.size() - 1;	/* 因为牌面从大到小排序的，因此需要从高位来查找 */
	///* 根据所要比较的poker来查找所需的扑克，一般是大1即可 */
	//Poker* searchPoker = GlobalFunc::searchGreaterPoker(_poker, 1);
	while (index >= 0){
		Vector<Poker*> tmp;
		tmp.pushBack(_pokers.at(index));
		/* 首先判断是否牌面值大于要找的牌，如果是，那就可以进一步判断是对子，三张，炸弹等，这样可以减轻计算量 
			判断对子等的作用是防止拆牌，在非顺子的前提下，不可以拆牌*/
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
								tmp.popBack();	/* 如果不是炸弹，那么要当前最后一张popBack */
							}
							index = index - 3;
							continue;
						}
						tmp.popBack();/* 如果不是三张，那么要当前最后一张popBack  */
					}
					index = index - 2;
					continue;
				}
				tmp.popBack();	/* 如果不是对子，那么要当前最后一张popBack  */
			}
			return tmp;
		}
		index = index - 1;
		tmp.clear();
	}
	/* 存在的一个问题，为了不拆牌，可能出现333444555这种情况时，不会出单张 */
	return Vector<Poker*>();
}

Vector<Poker*> GameRules::searchPair(Vector<Poker*> _pokers, const Poker* _poker /* = nullptr */){
	if (_pokers.size() < 2) return Vector<Poker*>();

	int index = _pokers.size() - 1;
	/* >= 1的目的是，保证每轮循环至少有两张牌 */
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
						tmp.popBack();	/* 如果不是炸弹，那么要当前最后一张popBack */
					}
					index = index - 3;
					continue;
				}
				tmp.popBack(); /* 如果不是三张，那么要当前最后一张popBack */
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
				tmp.popBack();/* 如果不是炸弹，那么要当前最后一张popBack */
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
	/* 如果没有找到炸弹，尝试查找王炸 */
	return searchKingBomb(_pokers);
}

Vector<Poker*> GameRules::searchKingBomb(Vector<Poker*> _pokers){
	if (_pokers.size() < 2) return Vector<Poker*>();	/* 如果扑克的张数小于2，那么肯定不是王炸 */
	/* 两张王肯定是整个牌面的第一第二张，因此直接将前两张牌放入一个Vector，调用isKingBomb函数来判断 */
	Vector<Poker*> tmp;
	tmp.pushBack(_pokers.at(0));
	tmp.pushBack(_pokers.at(1));
	bool isBomb = isKingBomb(tmp);
	return isBomb == true ? tmp : Vector<Poker*>();
}

/* 查找固定值的扑克 */
Vector<Poker*> GameRules::searchSpecifiedSingle(Vector<Poker*> _pokers, const Poker* _poker){
	if (_pokers.size() < 1 || _poker == nullptr) return Vector<Poker*>();
	int index = _pokers.size() - 1;
	while (index >= 0){
		Vector<Poker*> tmp;
		tmp.pushBack(_pokers.at(index));
		if (COMPATE_EQUAL(tmp.at(0), _poker) == true) return tmp;
		/* 如果当前扑克已经大于要找的扑克，那么直接break */
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
			/* 如果当前扑克已经大于要找的扑克，那么直接break */
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

Vector<Poker*> GameRules::searchSingleStraight(Vector<Poker*> _pokers, int length, const Poker* _poker){
	/* 如果length参数出错，或者扑克的张数不足所需的length，那么直接返回空 */
	if (length < 5 || length > 12 || _pokers.size() < length) return Vector<Poker*>();
	/*CCASSERT(length < 5 || length > 12, "顺子的长度必须在5~12之前");
	CCASSERT(_pokers.size() < length, "剩余的扑克数不够组成符合条件的顺子");*/
	
	/* 如果查找的顺子的最小的牌面值是1或者2，以及所需顺子牌面最低值+顺子长度（此时就是顺子牌面的最大值）> A（14）,那么直接返回空 */
	int pokerValue = _poker->getValue();
	/* 如果查找的牌值是0，表明是大王或者小王 */
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
		if (ret.size() == length){	/* 查询所得的结果，是递增的，和规定的递减相反，因此这里排序一次 */
			GlobalFunc::sort(ret);
			return ret;
		}
		index++; /* 当前已经查找失败，将index+1，从下一个开始查找 */
	}
	return Vector<Poker*>();
}

/* length这里是顺子的不同牌的个数 */
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
		if (ret.size() == length * 2){
			GlobalFunc::sort(ret);
			return ret;
		}
		index++; /* 当前已经查找失败，将index+1，从下一个开始查找 */
	}
	return Vector<Poker*>();
}

/* length这里是顺子的不同牌的个数 */
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
		if (ret.size() == length * 2){
			GlobalFunc::sort(ret);
			return ret;
		}
		index++; /* 当前已经查找失败，将index+1，从下一个开始查找 */
	}
	return Vector<Poker*>();
}

Vector<Poker*> GameRules::searchSingleInSplit(Vector<Poker*> _pokers, const Poker* _poker /* = nullptr */){
	if (_pokers.size() == 0) return Vector<Poker*>();

	int _index = _pokers.size() - 1;
	while (_index >= 0){
		if (_poker == nullptr || COMPARE_GREATER(_pokers.at(_index), _poker)){
			Vector<Poker*> _tmp;
			_tmp.pushBack(_pokers.at(_index));
			return _tmp;
		}
		_index--;
	}
	return Vector<Poker*>();
}

Vector<Poker*> GameRules::searchPairInSplit(Vector<Poker*> _pokers, const Poker* _poker /* = nullptr */){
	if (_pokers.size() < 2) return Vector<Poker*>();
	
	int _index = _pokers.size() - 1;
	while (_index >= 1){
		Vector<Poker*> _tmp;
		_tmp.pushBack(_pokers.at(_index));
		_tmp.pushBack(_pokers.at(_index - 1));
		if (isPair(_tmp) && (_poker == nullptr || COMPARE_GREATER(_tmp.at(0), _poker))){
			return _tmp; 
		}
		_tmp.clear();
		_index--;
	}
	return Vector<Poker*>();
}

Vector<Poker*> GameRules::searchTripleInSplit(Vector<Poker*> _pokers, const Poker* _poker /* = nullptr */){
	if (_pokers.size() < 3) return Vector<Poker*>();

	int _index = _pokers.size() - 1;
	while (_index >= 2){
		Vector<Poker*> _tmp;
		_tmp.pushBack(_pokers.at(_index));
		_tmp.pushBack(_pokers.at(_index - 1));
		_tmp.pushBack(_pokers.at(_index - 2));
		if (isTriple(_tmp) && (_poker == nullptr || COMPARE_GREATER(_tmp.at(0), _poker))){
			return _tmp;
		}
		_tmp.clear();
		_index--;
	}
	return Vector<Poker*>();
}

bool GameRules::isSingle(Vector<Poker*> _pokers){
	return _pokers.size() == 1;
}

bool GameRules::isPair(Vector<Poker*> _pokers){
	/* 如果扑克的value值小于1，说明是王，直接返回false */
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
	GlobalFunc::sort(_pokers);	/* 对扑克进行排序 */

	auto _first_poker = _pokers.at(0);
	/* 如果第一张是王，或者2，那么肯定不是顺子 */
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
	/* 如果扑克数不是偶数，那么肯定不是双顺 */
	if (_pokers.size() % 2 != 0) return false;

	GlobalFunc::sort(_pokers);	/* 对扑克进行排序 */

	auto _first_poker = _pokers.at(0);
	/* 如果第一张是王，或者2，那么肯定不是顺子 */
	if (_first_poker->getValue() == 2
		|| _first_poker->getPokerType() == BLACKJOKER
		|| _first_poker->getPokerType() == REDJOKER){
		return false;
	}
	/* 判断是否都是对子 */
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
	/* 如果扑克数不是3的倍数，那么肯定不是三顺，
		这里需要注意的是，本预备这里判断三顺带一张的情况，但是放在一起判断又会导致各种case之间
		错综复杂导致很难发现的BUG，故放弃*/
	if (_pokers.size() % 3 != 0) return false;

	GlobalFunc::sort(_pokers);	/* 对扑克进行排序 */

	auto _first_poker = _pokers.at(0);
	/* 如果第一张是王，或者2，那么肯定不是顺子 */
	if (_first_poker->getValue() == 2
		|| _first_poker->getPokerType() == BLACKJOKER
		|| _first_poker->getPokerType() == REDJOKER){
		return false;
	}
	/* 判断是否都是三张 */
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