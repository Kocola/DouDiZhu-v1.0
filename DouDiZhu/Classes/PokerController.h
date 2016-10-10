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
public:
	static int calcCardValue(PokerType type, int order = 0);
	static Size getPokerSize() { return getInstance()->getCardWithValue(BACK)->getContentSize(); } /* 获取扑克的尺寸 */
	static Sprite* getCardWithValue(PokerType type, int order = 0);		/* 利用牌值找到对应的牌面精灵 */
public:
	static Vector<Poker*> getRandomCards();	/* 获取一套随机的扑克，对外接口 */
private:
	static void initCards();	/* 初始化一套扑克 */
	static int randomInt(int begin, int end);
	static void randomCards();	/* 洗牌 */
private:
	PokerController(){}	/* 将默认构造函数的访问权限设置为private，禁止外部实例化 */
private:
	SpriteFrameCache* pokerCache;
	static PokerController* pokerController;
private:
	static Vector<Poker*> pokers;
};

#endif