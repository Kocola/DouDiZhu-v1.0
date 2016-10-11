#ifndef __HOLDER_PLAYER_H__
#define __HOLDER_PLAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

#include "Player.h"	/* 继承一个类时,被继承的类不可用前置声明 */

class OutCards;

class HolderPlayer : public Player{
public:
	static HolderPlayer* create(PlayerPosType _playerPosType);
	virtual bool init(PlayerPosType _playerPosType);
public:
	void initResource();	/* BUG */
public:
	virtual void updateCardPos() override;	/* 更新玩家的卡牌，每次出牌或者插入牌后调用，使扑克保持良好的排列 */
	virtual void callLandlord() override;	/* 叫地主 */
	virtual void outCard(OutCards* _lastOutcards) override;
public:
	void updateOutState();	/* 更新出牌按钮的状态，即依据判断准备出的牌是否能够打出来决定出牌按钮的状态 */
private:
	void initObserver();	/* 初始化观察者 */
	void initCardPos();		/* 初始化显示卡牌的位置 */
	void initMenuItemSprite();	/* 初始化和Player相关的按钮菜单 */
	void initPassHintSprite();	/* 初始化打不过上家的提示 */
private:
	Vector<Poker*> searchHintCard(OutCards* _lastOutcards);
	void pass();	/* 不出牌 */
private:
	void addCardForWaitOut(Ref* _data);
	void deleteCardForWaitOut(Ref* _data);
	void updateOutState(Ref*);	/* 更新出牌状态，这里是观察者注册函数 */
public:
	void setCallLandLordMenuItemVisible(bool _visible);
	void setReady(bool _visible) override { startMenuItem->setVisible(_visible); }
private:
	void setLastOutCard(OutCards* _lastOutCard) { this->lastOutCard = _lastOutCard; }
	void setLastOutCardNull() { this->lastOutCard = nullptr; }
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
	Vector<Poker*> hintCards;	/* 提示要出的牌 */
	Vector<Poker*> clickedPokers;	/* 已经点击的牌，每次触摸事件结束需要初始化为空 */
	OutCards* lastOutCard;		/* 上一手扑克，初始化为nullptr，轮到其出牌时获取，出完牌后必须置nullptr */
};

#endif