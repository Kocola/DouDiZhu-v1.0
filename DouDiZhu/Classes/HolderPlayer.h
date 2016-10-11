#ifndef __HOLDER_PLAYER_H__
#define __HOLDER_PLAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

#include "Player.h"	/* �̳�һ����ʱ,���̳е��಻����ǰ������ */

class OutCards;

class HolderPlayer : public Player{
public:
	static HolderPlayer* create(PlayerPosType _playerPosType);
	virtual bool init(PlayerPosType _playerPosType);
public:
	void initResource();	/* BUG */
public:
	virtual void updateCardPos() override;	/* ������ҵĿ��ƣ�ÿ�γ��ƻ��߲����ƺ���ã�ʹ�˿˱������õ����� */
	virtual void callLandlord() override;	/* �е��� */
	virtual void outCard(OutCards* _lastOutcards) override;
public:
	void updateOutState();	/* ���³��ư�ť��״̬���������ж�׼���������Ƿ��ܹ�������������ư�ť��״̬ */
private:
	void initObserver();	/* ��ʼ���۲��� */
	void initCardPos();		/* ��ʼ����ʾ���Ƶ�λ�� */
	void initMenuItemSprite();	/* ��ʼ����Player��صİ�ť�˵� */
	void initPassHintSprite();	/* ��ʼ���򲻹��ϼҵ���ʾ */
private:
	Vector<Poker*> searchHintCard(OutCards* _lastOutcards);
	void pass();	/* ������ */
private:
	void addCardForWaitOut(Ref* _data);
	void deleteCardForWaitOut(Ref* _data);
	void updateOutState(Ref*);	/* ���³���״̬�������ǹ۲���ע�ắ�� */
public:
	void setCallLandLordMenuItemVisible(bool _visible);
	void setReady(bool _visible) override { startMenuItem->setVisible(_visible); }
private:
	void setLastOutCard(OutCards* _lastOutCard) { this->lastOutCard = _lastOutCard; }
	void setLastOutCardNull() { this->lastOutCard = nullptr; }
	/* �ص����� */
private:
	void startCallback(Ref*);

	void passCallback(Ref*);
	void hintCallback(Ref*);
	void outCallback(Ref*);

	void noCallCallback(Ref*);
	void oneScoreCallback(Ref*);
	void twoScoreCallback(Ref*);
	void threeScoreCallback(Ref*);
	/* ����ҿ�����ʾ��ص�λ�ó�Ա */
private:
	float displayCardStartX;	/* �˿������ʾ����ʼ��X����ֵ */
	float displayCardMiddleX;		/* �˿���Ļ�м���X����ֵ */
	/* ��ť */
private:
	MenuItemSprite* startMenuItem;

	MenuItemSprite* noCallMenuItem;
	MenuItemSprite* oneScoreMenuItem;
	MenuItemSprite* twoScoreMenuItem;
	MenuItemSprite* threeScoreMenuItem;

	MenuItemSprite* passMenuItem;
	MenuItemSprite* hintMenuItem;
	MenuItemSprite* outMenuItem;

	Sprite* passHintSprite;
private:
	Vector<Poker*> cardsForWaitOut;	/* �ѵ��׼�������� */
	Vector<Poker*> hintCards;	/* ��ʾҪ������ */
	Vector<Poker*> clickedPokers;	/* �Ѿ�������ƣ�ÿ�δ����¼�������Ҫ��ʼ��Ϊ�� */
	OutCards* lastOutCard;		/* ��һ���˿ˣ���ʼ��Ϊnullptr���ֵ������ʱ��ȡ�������ƺ������nullptr */
};

#endif