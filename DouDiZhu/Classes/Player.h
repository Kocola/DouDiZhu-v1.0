#ifndef __PLAYER__H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "GlobalFunc.h"
#include "Poker.h"

using namespace cocos2d;

class HeadImage;

class Player : public Node{
public:
	CREATE_FUNC(Player);
	virtual bool init();
public:
	void updatePokerPos();
	void addPoker(Poker* poker);
	void removePoker(Poker* poker);
	void removeAllPoker();
	void insertCards(const Vector<Poker*>& _pokers);
private:
	CC_SYNTHESIZE(bool, isLandlord, Landlord);		/* 是否是地主 */
	CC_SYNTHESIZE(bool, isCall, Call);		/* 是否已经叫地主 */
	CC_SYNTHESIZE(Vector<Poker*>, pokers, Poker);		/* 手里有的扑克牌 */
	CC_SYNTHESIZE(PlayerType, playerType, PlayerType);		/* 玩家类型 */
	CC_SYNTHESIZE(bool, isOutPoker, IsOutPoker);	/* 是否出牌 */
	CC_SYNTHESIZE(int, callLandlordScore, CallLandlordScore);	/* 设置叫地主分数 */
private:
	float displayLeftStartX;	/* 卡牌左边显示的起始点X坐标值 */
	float displayMiddleX;		/* 屏幕中间点的X坐标值 */
	HeadImage* headImage;		/* 头像对象 */
};

#endif