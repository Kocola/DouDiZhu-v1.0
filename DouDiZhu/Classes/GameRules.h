#ifndef __GAME_RULES_H__
#define __GAME_RULES_H__

#include "cocos2d.h"
#include "Poker.h"
#include "PokerValueType.h"

using namespace cocos2d;

class OutCards;

class GameRules : public Ref{
public:
	static GameRules* getInstance();
	virtual bool init();
public:
	/* 根据准备出的牌和上一手牌，判断当前牌是否可出 */
	bool canOutCards(Vector<Poker*> curCards, const OutCards* lastOutCards);
	/* 电脑在自己可以任意出牌时，找到一张合适的牌（这里为了赶时间，只是简单地由单张，对子，三张，顺子，炸弹） */
	Vector<Poker*> searchProperPokers(Vector<Poker*> _pokers);
private:
	/* 获取当前牌的最小牌 */
	/*Poker* getLowestPoker(const Vector<Poker*> _pokers);*/
public:
	PokerValueType analysePokerValueType(Vector<Poker*> _pokers);	/* 分析给定扑克的牌型 */
	bool isPokerValueType(Vector<Poker*> _pokers);		/* 是否是扑克类型 */
	bool isSpecifiedPokerValueType(Vector<Poker*> _pokers, PokerValueType pokerValueType);	/* 给定的扑克是否是某种牌型 */
	/* 获取指定的类型的扑克，返回Vector为空时表示查找失败 
		第一个参数是玩家拥有的扑克，第三个参数是返回的类型的扑克里最小的牌，如果是nullptr，表明可以出任意值的该类型扑克
		第四个参数是针对顺子时的长度 */
	Vector<Poker*> calcPokerWithValueType(Vector<Poker*> _pokers, PokerValueType pokerValueType, const Poker* _poker = nullptr, int length = 0);	/* 不可拆牌 */
	Vector<Poker*> calcPokerWithValueTypeInSplit(Vector<Poker*> _pokers, PokerValueType pokerValueType, const Poker* _poker = nullptr, int length = 0);		/* 可拆牌 */
private:
	/* 所有以下函数的value参数，表示所要查找的牌面的最低位的牌面值 */

	/* 第一个参数是玩家的牌面，第二个是上家对应出牌的最小扑克的指针，默认为nullptr，表示查找任意对应的牌型即可，否则需要 大于 _poker的对应牌型才可以 */
	//Vector<Poker*> search(Vector<Poker*> _pokers, PokerValueType pokerValueType, int value = 0);	/* 该函数放弃，各个查找分别实现 */
	Vector<Poker*> searchSingle(Vector<Poker*> _pokers, const Poker* _poker = nullptr);	/* 查找单张 */
	Vector<Poker*> searchPair(Vector<Poker*> _pokers, const Poker* _poker = nullptr);		/* 查找对子  */
	Vector<Poker*> searchTriple(Vector<Poker*> _pokers, const Poker* _poker = nullptr); /* 查找三张 */
	Vector<Poker*> searchBomb(Vector<Poker*> _pokers, const Poker* _poker = nullptr); /* 查找炸弹 */
	Vector<Poker*> searchKingBomb(Vector<Poker*> _pokers); /* 查找王炸 */
	/* 查找固定值的单张，对子，三张 */
	Vector<Poker*> searchSpecifiedSingle(Vector<Poker*> _pokers, const Poker* _poker);
	Vector<Poker*> searchSpecifiedPair(Vector<Poker*> _pokers, const Poker* _poker);
	Vector<Poker*> searchSpecifiedTriple(Vector<Poker*> _pokers, const Poker* _poker);
	/* searchStraight是查找三种顺子的基本函数，其它三种都要调用这个函数, step用于控制查找什么类型的顺子
		length用于限制顺子的长度是多少，例如56789 长度就是5
		单顺长度至少是5，双顺和三顺的长度至少是3*/
	//Vector<Poker*> searchStraight(Vector<Poker*> _pokers, int step, int length, int value = 0);	/* 该函数放弃，查找的多钟顺子分别实现 */
	Vector<Poker*> searchSingleStraight(Vector<Poker*> _pokers, int length, const Poker* _poker);	/* 查找顺子 */
	Vector<Poker*> searchPairStraight(Vector<Poker*> _pokers, int length, const Poker* _poker); /* 查找双顺 */
	Vector<Poker*> searchTripleStraight(Vector<Poker*> _pokers, int length, const Poker* _poker); /* 查找三顺 */
private:
	Vector<Poker*> searchSingleInSplit(Vector<Poker*> _pokers, const Poker* _poker = nullptr);	/* 查找单张，可拆牌 */
	Vector<Poker*> searchPairInSplit(Vector<Poker*> _pokers, const Poker* _poker = nullptr);		/* 查找对子，可拆牌  */
	Vector<Poker*> searchTripleInSplit(Vector<Poker*> _pokers, const Poker* _poker = nullptr); /* 查找三张，可拆牌 */
private:
	bool isSingle(Vector<Poker*> _pokers);	/* 是否是单张 */
	bool isPair(Vector<Poker*> _pokers);	/* 是否是对子 */
	bool isTriple(Vector<Poker*> _pokers);	/* 是否是三张 */
	bool isSingleStraight(Vector<Poker*> _pokers);	/* 是否是单顺子 */
	bool isPairStraight(Vector<Poker*> _pokers);	/* 是否是双顺 */
	/* 这里只判断不带单张或者双张的三顺 */
	bool isTripleStraight(Vector<Poker*> _pokers);	/* 是否是三顺 */
	bool isBomb(Vector<Poker*> _pokers);		/* 是否是炸弹 */
	bool isKingBomb(Vector<Poker*> _pokers);	/* 是否是王炸 */
private:
	static GameRules* gameRules;
};

#endif