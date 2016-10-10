#ifndef __POKER_H__
#define __POKER_H__

#include "cocos2d.h"
#include "GlobalDefine.h"

using namespace cocos2d;

class GameScene;

class Poker : public Node{
private:
	Poker(PokerType type, int value);
public:
	virtual bool init();
	static Poker* create(PokerType type, int value = 0);
	/* 克隆一个当前类的实例 */
	Poker* clone() const;
public:
	void showFront();		/* 显示正面 */
	void showBack();		/* 显示背面 */
	void selectedCardOut();		/* 选择未被选择的牌，那么该牌出头   */
	void selectedCardBack();	/* 选择的牌已经被选择，那么将该牌放回 */
private:
	CC_SYNTHESIZE(bool, isSelect, Select);		/* 是否已选 */
	//CC_SYNTHESIZE(GameScene*, gameScene, GameScene);		/* 设置游戏场景 */
	CC_SYNTHESIZE(bool, canClick, CanClick);		/* 是否能被点击 */
	CC_SYNTHESIZE(PokerType, pokerType, PokerType);		/* 花色 */
	CC_SYNTHESIZE(int, value, Value);		/* 牌值 */
	CC_SYNTHESIZE(Sprite*, poker, Poker);
private:
	void createSprite();		/* 创建两个扑克精灵，分别代表正面和反面 */
	void updateSpritePos();		/* 将精灵的坐标和类实例对象对应的坐标相一致 */
	void updateOutState();	/* 调用GameScene::updateOutState更新出牌按钮状态 */
private:
	//Sprite* poker;	/* 保存该扑克对应的精灵 */
	Sprite* backPoker;	/* 保存扑克的背面精灵，在需要显示背面时显示 */
};

#endif