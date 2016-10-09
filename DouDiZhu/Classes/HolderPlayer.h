#ifndef __HOLDER_PLAYER_H__
#define __HOLDER_PLAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

#include "Player.h"	/* 继承一个类时,被继承的类不可用前置声明 */

class OutCards;

class HolderPlayer : public Player{
public:
	void updatePokerPos();	/* 更新玩家的卡牌，每次出牌后调用，使扑克保持良好的排列 */
	void updateOutState();	/* 更新出牌按钮的状态，即依据判断准备出的牌是否能够打出来决定出牌按钮的状态 */
	void outCard(OutCards* _lastOutcards, std::function<void(OutCards*)>& _updateOutcardsCallback, std::function<void>& _updateOutcardOrderCallback);
private:
	void initObserver();	/* 初始化观察者 */
	void initCardPos();		/* 初始化显示卡牌的位置 */
	void initMenuItemSprite();	/* 初始化和Player相关的按钮菜单 */
	void initPassHintSprite();	/* 初始化打不过上家的提示 */
private:
	Vector<Poker*> HolderPlayer::searchHintCard(OutCards* _lastOutcards);
private:
	void setCallLandLordMenuItemVisible(bool _visible);
	void addCardForWaitOut(Ref* _data);
	void deleteCardForWaitOut(Ref* _data);
	void updateOutState(Ref*);	/* 更新出牌状态，这里是观察者注册函数 */
	/* 回调函数 */
private:
	void startCallback(Ref*);

	void passCallback(Ref*);
	void hintCallback(Ref*);
	void outCallback(Ref*);

	void noCallCallback(Ref*);
	void oneScoreCallback(Ref*);
	void twoScoreCallback(Ref*);
	void threeScoreCallback(Ref*);
	/* 和玩家卡牌显示相关的位置成员 */
private:
	float displayCardStartX;	/* 扑克左边显示的起始点X坐标值 */
	float displayCardMiddleX;		/* 扑克屏幕中间点的X坐标值 */
	/* 按钮 */
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
	Vector<Poker*> cardsForWaitOut;	/* 已点击准备出的牌 */
};

#endif