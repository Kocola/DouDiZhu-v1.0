#ifndef __OUT_CARDS_H__
#define __OUT_CARDS_H__

#include "cocos2d.h"

using namespace cocos2d;

#include "PokerValueType.h"

class Poker;
class Player;

/* �����࣬���ڱ�����ҳ����ƣ��̳���Ref����Ϊ�˱����ֶ��ڴ���� */
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
	PokerValueType pokerValueType;		/* ��ǰ������ */
	size_t totalLength;		/* ��������ĳ��ȣ�����334455�ĳ�����6 */
	Poker* lowestPoker;		/* ����������ֵ��С���ƣ�����334455��ָ��3����Ӧ���˿˵�ָ�� */
	Player* pokerOwner;		/* ��ǰ�˿˳����� */
};

#endif