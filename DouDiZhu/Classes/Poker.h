#ifndef __POKER_H__
#define __POKER_H__

#include "cocos2d.h"
#include "GlobalDefine.h"

using namespace cocos2d;

class GameScene;

class Poker : public Node{
private:
	Poker(GameScene* _gameScene, PokerType type, int value);
public:
	virtual bool init();
	static Poker* create(GameScene* _gameScene, PokerType type, int value = 0);
	/* ��¡һ����ǰ���ʵ�� */
	Poker* clone() const;
public:
	void showFront();		/* ��ʾ���� */
	void showBack();		/* ��ʾ���� */
	void selectedCardOut();		/* ѡ��δ��ѡ����ƣ���ô���Ƴ�ͷ   */
	void selectedCardBack();	/* ѡ������Ѿ���ѡ����ô�����ƷŻ� */
private:
	CC_SYNTHESIZE(bool, isSelect, Select);		/* �Ƿ���ѡ */
	CC_SYNTHESIZE(GameScene*, gameScene, GameScene);		/* ������Ϸ���� */
	CC_SYNTHESIZE(bool, canClick, CanClick);		/* �Ƿ��ܱ���� */
	CC_SYNTHESIZE(PokerType, pokerType, PokerType);		/* ��ɫ */
	CC_SYNTHESIZE(int, value, Value);		/* ��ֵ */
	CC_SYNTHESIZE(Sprite*, poker, Poker);
private:
	void createSprite();		/* ���������˿˾��飬�ֱ��������ͷ��� */
	void updateSpritePos();		/* ��������������ʵ�������Ӧ��������һ�� */
private:
	//Sprite* poker;	/* ������˿˶�Ӧ�ľ��� */
	Sprite* backPoker;	/* �����˿˵ı��澫�飬����Ҫ��ʾ����ʱ��ʾ */
};

#endif