#ifndef __OUT_CARDS_H__
#define __OUT_CARDS_H__

#include "cocos2d.h"

using namespace cocos2d;

#include "PokerValueType.h"

class Poker;
class Player;

/* 出牌类，用于保存玩家出的牌，继承自Ref，是为了避免手动内存管理 */
class OutCards : public Ref{
public:
	OutCards(Player* _pokerOwner, PokerValueType _pokerValueType, size_t _totalLength, Poker* _lowestPoker);
	virtual bool init();
	static OutCards* create(Player* _pokerOwner, PokerValueType _pokerValueType, size_t _totalLength, Poker* _lowestPoker);
public:
	/*PokerValueType*/
	PokerValueType getPokerValueType() const { return pokerValueType; }
	void setPokerValueType(PokerValueType _pokerValueType){ this->pokerValueType = _pokerValueType; }
	/* totalLength */
	size_t getTotalLength() const { return totalLength; }
	void setTotalLength(size_t _totalLength) { this->totalLength = _totalLength; }
	/* lowestPoker */
	const Poker* getLowestPoker() const { return lowestPoker; }
	void setLowestPoker(Poker* _lowestPoker) { this->lowestPoker = _lowestPoker; }
	/* PokerOwner */
	Player* getPokerOwner() const{ return pokerOwner; }
private:
	PokerValueType pokerValueType;		/* 当前的牌型 */
	size_t totalLength;		/* 整个牌面的长度，例如334455的长度是6 */
	Poker* lowestPoker;		/* 整个牌面牌值最小的牌，例如334455是指向3所对应的扑克的指针 */
	Player* pokerOwner;		/* 当前扑克持有者 */
};

#endif