#ifndef __POKER_CONTROLLER_H__
#define __POKER_CONTROLLER_H__

#include "cocos2d.h"
#include "GlobalDefine.h"
#include "Poker.h"

using namespace cocos2d;

class PokerController : public Layer{
public:
	static PokerController* getInstance();
	virtual bool init();
	static int calcPokerValue(PokerType type, int order = 0);
	static Size getPokerSize() { return getInstance()->getPokerWithValue(BACK)->getContentSize(); } /* ��ȡ�˿˵ĳߴ� */
	static Sprite* getPokerWithValue(PokerType type, int order = 0);		/* ������ֵ�ҵ���Ӧ�����澫�� */
	//Vector<Poker*> getAllPoker();
private:
	PokerController(){}	/* ��Ĭ�Ϲ��캯���ķ���Ȩ������Ϊprivate����ֹ�ⲿʵ���� */
private:
	SpriteFrameCache* pokerCache;
	static PokerController* pokerController;
};

#endif