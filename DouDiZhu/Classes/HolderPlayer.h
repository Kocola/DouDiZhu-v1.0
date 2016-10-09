#ifndef __HOLDER_PLAYER_H__
#define __HOLDER_PLAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

#include "Player.h"	/* �̳�һ����ʱ,���̳е��಻����ǰ������ */

class OutCards;

class HolderPlayer : public Player{
public:
	void updatePokerPos();	/* ������ҵĿ��ƣ�ÿ�γ��ƺ���ã�ʹ�˿˱������õ����� */
	void updateOutState();	/* ���³��ư�ť��״̬���������ж�׼���������Ƿ��ܹ�������������ư�ť��״̬ */
	void outCard(OutCards* _lastOutcards, std::function<void(OutCards*)>& _updateOutcardsCallback, std::function<void>& _updateOutcardOrderCallback);
private:
	void initObserver();	/* ��ʼ���۲��� */
	void initCardPos();		/* ��ʼ����ʾ���Ƶ�λ�� */
	void initMenuItemSprite();	/* ��ʼ����Player��صİ�ť�˵� */
	void initPassHintSprite();	/* ��ʼ���򲻹��ϼҵ���ʾ */
private:
	Vector<Poker*> HolderPlayer::searchHintCard(OutCards* _lastOutcards);
private:
	void setCallLandLordMenuItemVisible(bool _visible);
	void addCardForWaitOut(Ref* _data);
	void deleteCardForWaitOut(Ref* _data);
	void updateOutState(Ref*);	/* ���³���״̬�������ǹ۲���ע�ắ�� */
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
};

#endif