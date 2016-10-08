#ifndef __PLAYER_ORDER_H__
#define __PLAYER_ORDER_H__

#include "cocos2d.h"

using namespace cocos2d;

#include "GlobalDefine.h"

class HeadImage;

class PlayerOrder : public Node{
public:
	//CREATE_FUNC(PlayerOrder);
	static PlayerOrder* create();
	virtual bool init();
	void setPlayerOrderState(PlayerOrderState _playerOrderState);
private:
	static SpriteFrame* createSpriteFrameWithPlayerOrderState(PlayerOrderState _playerOrderState);
private:
	Sprite* sprite;
private:
	static SpriteFrameCache* spriteFrameCache;
};

#endif