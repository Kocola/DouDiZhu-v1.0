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
	/* 过滤掉XX带Y里的Y */
	Vector<Poker*> filterAccessoryCards(const Vector<Poker*>& _cards);
private:
	/* 获取当前牌的最小牌 */
	/*Poker* getLowestPoker(const Vector<Poker*> _pokers);*/
public:
	PokerValueType analysePokerValueType(Vector<Poker*> _pokers);	/* 分析给定扑克的牌型 */
	bool isPokerValueType(Vector<Poker*> _pokers);		/* 是否是扑克类型 */
	bool isSpecifiedPokerValueType(Vector<Poker*> _pokers, PokerValueType pokerValueType);	/* 给定的扑克是否是某种牌型 */
	Poker* calcLowestPoker(Vector<Poker*> _pokers, PokerValueType _pokerValueType);	/* 计算扑克里最小的扑克值 */
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

	bool isTripleWithSingle(Vector<Poker*> _pokers);/* 是否是3带1 */
	bool isTripleWithPair(Vector<Poker*> _pokers);	/* 是否是3带2 */

	bool isSingleStraight(Vector<Poker*> _pokers);	/* 是否是单顺子 */
	bool isPairStraight(Vector<Poker*> _pokers);	/* 是否是双顺 */
	/* 这里只判断不带单张或者双张的三顺 */
	bool isTripleStraight(Vector<Poker*> _pokers);	/* 是否是三顺 */
	/* 带双张和单张的三顺 */
	bool isTripleStraightWithSingle(Vector<Poker*> _pokers);		/* 三顺带相同数量的单张 */
	bool isTripleStraightWithPair(Vector<Poker*> _pokers);	/* 三顺带相同数量的双张 */

	bool isBomb(Vector<Poker*> _pokers);		/* 是否是炸弹 */
	bool isKingBomb(Vector<Poker*> _pokers);	/* 是否是王炸 */
private:
	Poker* calcTripleStraightWithSingleLowestPoker(Vector<Poker*> _pokers);
	Poker* calcTripleStraightWithPairLowestPoker(Vector<Poker*> _pokers);

	bool isExistBomb(const Vector<Poker*>& _pokers);	/* 是否存在炸弹，用于分离三张时避免分离炸弹造成的问题 */
	bool isAllPair(const Vector<Poker*>& _pokers);	/* 判断容器里的扑克是否都是对子，这个函数用于isTripleStraightWithPair */
	PokerValueType calcBasePokerValueType(PokerValueType _pokerValueType);	/* 例如单张，返回SINGLE，三带1和三带2，三张都返回TRIPLE，
																			三顺带1，三顺带2，三顺都返回TRIPLESTRAIGHT，这样做的目的是为了兼容牌型非完全一致，但是
																			基本牌型却是一致的情况，不然的话，三张和三带1就无法等价，可是如果把三带一直接返回TRIPLE，
																			又会导致其它问题，现在这种办法是折中的办法*/

	/* 从扑克容器里分离出所有的三张，返回的是三张的容器，传入的容器里在函数执行完毕后会去除所有的三张 */
	Vector<Poker*> splitTriplesInPokers(Vector<Poker*>& _pokers);
private:
	/* 闭开区间,通过索引区间获取子容器 */
	Vector<Poker*> subVectorWithIndex(Vector<Poker*> _pokers, int startIndex, int endIndex);
private:
	static GameRules* gameRules;
};

#endif