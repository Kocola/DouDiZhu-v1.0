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
	static int calcPokerValue(PokerType type, int order = 0);
	static Size getPokerSize() { return getInstance()->getPokerWithValue(BACK)->getContentSize(); } /* 获取扑克的尺寸 */
	static Sprite* getPokerWithValue(PokerType type, int order = 0);		/* 利用牌值找到对应的牌面精灵 */
	//Vector<Poker*> getAllPoker();
private:
	PokerController(){}	/* 将默认构造函数的访问权限设置为private，禁止外部实例化 */
private:
	SpriteFrameCache* pokerCache;
	static PokerController* pokerController;
};

#endif