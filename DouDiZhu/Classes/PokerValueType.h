#ifndef __POKER_VALUE_TYPE_H__
#define __POKER_VALUE_TYPE_H__

enum PokerValueType{
	NONE, /* 什么都不是 */
	SINGLE, /* 单张 */
	PAIR,	/* 对子 */
	TRIPLE,	/* 三张 */
	BOMB,	/* 炸弹 */
	KINGBOMB,	/* 王炸 */
	STRAIGHT,	/* 顺子 */
	PAIRSRAIGHT,	/* 双顺 */
	TRIPLESTRAIGHT	/* 三顺 */
};

class Poker;

class PokerTypeBase{
public:
	virtual Poker* getLowerPoker() const = 0;
	virtual int getStraightLength() const = 0;
private:
	CC_SYNTHESIZE(PokerValueType, pokerValueType, PokerValueType);
};

class PokerTypeGeneral : public PokerTypeBase{
public:
	Poker* getLowestPoker() const { return lowestPoker; }
	void setLowestPoker(Poker* poker) { this->lowestPoker = poker; }
	int getStraightLength() const { return 0; }	/* 普通牌型没有顺子长度 */
private:
	Poker* lowestPoker;
};

class PokerTypeStraight : public PokerTypeGeneral{
public:
};

#endif