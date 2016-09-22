#ifndef __POKER_VALUE_TYPE_H__
#define __POKER_VALUE_TYPE_H__

enum PokerValueType{
	NONE, /* ʲô������ */
	SINGLE, /* ���� */
	PAIR,	/* ���� */
	TRIPLE,	/* ���� */
	BOMB,	/* ը�� */
	KINGBOMB,	/* ��ը */
	STRAIGHT,	/* ˳�� */
	PAIRSRAIGHT,	/* ˫˳ */
	TRIPLESTRAIGHT	/* ��˳ */
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
	int getStraightLength() const { return 0; }	/* ��ͨ����û��˳�ӳ��� */
private:
	Poker* lowestPoker;
};

class PokerTypeStraight : public PokerTypeGeneral{
public:
};

#endif