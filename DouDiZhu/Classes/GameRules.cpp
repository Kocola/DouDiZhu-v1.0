#include "GameRules.h"
#include "GlobalDefine.h"
#include "GlobalFunc.h"

#define COMPARE_GREATER(a, b) GlobalFunc::cmp_sort(a, b);

Vector<Poker*> GameRules::search(Vector<Poker*> _pokers, PokerValueType pokerValueType, int value /* = 0 */){
	int index = 0;
	while (index < _pokers.size()){
		Vector<Poker*> tmp;	/* 临时存放结果的Vector，初始时存入三个_pokers里的扑克 */
		tmp.pushBack(_pokers.at(index));
		tmp.pushBack(_pokers.at(index + 1));
		tmp.pushBack(_pokers.at(index + 2));
		tmp.pushBack(_pokers.at(index + 3));

		if (isBomb(tmp) == true){	/* 如果存入的牌是四张，那么不可以拆开这炸弹单出某张牌 */
			/* 如果查找的类型是炸弹，并且炸弹牌面大于value，那么找到了直接返回 */
			if (pokerValueType == BOMB && tmp.at(index)->getValue() >= value) return tmp;
			index += 4;
			tmp.clear();
			continue;
		}

		tmp.popBack();		/* 退出最后添加的扑克，保证接下来的判断不会出错 */
		if (isTriple(tmp) == true){	/* 如果存入的牌是三张，那么不可以拆开这三张单出某张牌 */
			if (pokerValueType == TRIPLE && tmp.at(index)->getValue() >= value) return tmp;
			index += 3;
			tmp.clear();
			continue;
		}

		tmp.popBack();	/* 退出最后添加的扑克，保证接下来的判断不会出错 */
		if (isPair(tmp) == true){		/* 判断是否是对子 */
			if (pokerValueType == PAIR && tmp.at(index)->getValue() >= value) return tmp;
			index += 2;
			tmp.clear();
			continue;
		}

		tmp.popBack();		/* 退出最后添加的扑克，保证接下来的判断不会出错 */
		/* 如果当前的index对应的牌不和后面的牌构成炸弹，三张和对子，那么
		判断其是否大于上家所出的牌，即value值，如果不是，继续寻找*/
		if (pokerValueType == SINGLE){
			if (tmp.at(index)->getValue() >= value) return tmp;
			index += 1;
			tmp.clear();
		}
	}
	return Vector<Poker*>();		/* 保证当_pokers本身为空时，可以正确返回 */
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

/* 该函数暂时不用，待后面找到优化和合并的办法再写 */
Vector<Poker*> GameRules::searchStraight(Vector<Poker*> _pokers, int step, int length, int value /* = 0 */){
	CCASSERT(value < 0, "该函数不可调用");
	return Vector<Poker*>();
}

Vector<Poker*> GameRules::searchSingleStraight(Vector<Poker*> _pokers, int length, int value /* = 0 */){
	/* 如果length参数出错，或者扑克的张数不足所需的length，那么直接返回空 */
	if (length < 5 || length > 12 || _pokers.size() < length) return Vector<Poker*>();
	
	/* 如果查找的顺子的最小的牌面值是1或者2，以及所需顺子牌面最低值+顺子长度（此时就是顺子牌面的最大值）> A（14）,那么直接返回空 */
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
	GlobalFunc::sort(_pokers);	/* 对扑克进行排序 */

	auto _first_poker = _pokers.at(0);
	/* 如果第一张是王，或者2，那么肯定不是顺子 */
	if (_first_poker->getValue() == 2
		|| _first_poker->getPokerType() == BLACKJOKER
		|| _first_poker->getPokerType() == REDJOKER){
		return false;
	}

	auto secondpoker_index = 0 + step;	/* 第二张牌的索引，这里的第二张是不同牌值的第二张，例如AAKK，那么第二张是K */

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