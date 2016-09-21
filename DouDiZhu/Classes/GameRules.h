#ifndef __GAME_RULES_H__
#define __GAME_RULES_H__

#include "cocos2d.h"
#include "Poker.h"
#include "PokerValueType.h"

using namespace cocos2d;

class GameRules{
public:
	PokerValueType analysePokerValueType(Vector<Poker*> _pokers);	/* 分析给定扑克的牌型 */
	bool isPokerValueType(Vector<Poker*> _pokers, PokerValueType pokerValueType);	/* 给定的扑克是否是某种牌型 */
	/* 获取指定的类型的扑克，返回Vector为空时表示查找失败 
		第一个参数是玩家拥有的扑克 */
	Vector<Poker*> calcPokerWithValueType(Vector<Poker*> _pokers, PokerValueType pokerValueType);
private:
	/* 所有以下函数的value参数，表示所要查找的牌面的最低位的牌面值 */

	/* 第一个参数是玩家的牌面，第二个是上家对应牌型的值，默认是0，表示查找任意对应的牌值，否则需要大于value的对应牌型才可以 */
	/* search是所有search函数的调用对象，根据第二个参数，来查找符合要求的牌型，消除代码冗余 
		search函数仅用于查找 单张， 对子， 三张，以及炸弹*/
	Vector<Poker*> search(Vector<Poker*> _pokers, PokerValueType pokerValueType, int value = 0);
	Vector<Poker*> searchSingle(Vector<Poker*> _pokers, int value = 0);	/* 查找单张 */
	Vector<Poker*> searchPair(Vector<Poker*> _pokers, int value = 0);		/* 查找对子  */
	Vector<Poker*> searchTriple(Vector<Poker*> _pokers, int value = 0); /* 查找三张 */
	Vector<Poker*> searchBomb(Vector<Poker*> _pokers, int value = 0); /* 查找炸弹 */
	/* searchStraight是查找三种顺子的基本函数，其它三种都要调用这个函数, step用于控制查找什么类型的顺子
		length用于限制顺子的长度是多少，例如56789 长度就是5
		单顺长度至少是5，双顺和三顺的长度至少是3*/
	Vector<Poker*> searchStraight(Vector<Poker*> _pokers, int step, int length, int value = 0); 
	Vector<Poker*> searchSingleStraight(Vector<Poker*> _pokers, int length, int value = 0);	/* 查找顺子 */
	Vector<Poker*> searchPairStraight(Vector<Poker*> _pokers, int length, int value = 0); /* 查找双顺 */
	Vector<Poker*> searchTripleStraight(Vector<Poker*> _pokers, int length, int value = 0); /* 查找三顺 */

	Vector<Poker*> searchKingBomb(Vector<Poker*> _pokers, int value = 0); /* 查找王炸 */
private:
	bool isSingle(Vector<Poker*> _pokers);	/* 是否是单张 */
	bool isPair(Vector<Poker*> _pokers);	/* 是否是对子 */
	bool isTriple(Vector<Poker*> _pokers);	/* 是否是三张 */
	bool isStraight(Vector<Poker*> _pokers, int step = 1);	/* 是否是顺子,第二个参数是让双顺和三顺可以调用这个函数，避免代码冗余 */
	bool isPairStraight(Vector<Poker*> _pokers);	/* 是否是双顺 */
	bool isTripleStraight(Vector<Poker*> _pokers);	/* 是否是三顺 */
	bool isBomb(Vector<Poker*> _pokers);		/* 是否是炸弹 */
	bool isKingBomb(Vector<Poker*> _pokers);	/* 是否是王炸 */
};

#endif