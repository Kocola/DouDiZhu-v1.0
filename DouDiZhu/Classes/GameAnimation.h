#ifndef __GAME_ANIMATION_H__
#define __GAME_ANIMATION_H__

#include "cocos2d.h"

using namespace cocos2d;

class GameAnimation : public Ref{
public:
	static GameAnimation* getInstance();
	virtual bool init();
public:
	bool initWinAnimation();
	Animate* getWinAnimation() const;
	bool initLostAnimation();
	Animate* getLostAnimation() const;
private:
	static GameAnimation* gameAnimation;
private:
	Animate* winAction;
	Animate* lostAction;
};

#endif