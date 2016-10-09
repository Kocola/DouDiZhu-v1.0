#ifndef __COUNT_DOWN_H__
#define __COUNT_DOWN_H__

#include "cocos2d.h"

using namespace cocos2d;

class CountDown : public Layer{
public:
	virtual bool init();
	CREATE_FUNC(CountDown);
public:
	void startCountDown();
	void setCountDownUpper(float _countDownUpper){ this->countDownUpper = _countDownUpper; }
	void setCountDownScale(float _scale);
	void stopCountDown();	/* 停止计时 */
private:
	void countDown(float delta);
private:
	int curSpriteIndex;
	int totalSpriteNum;
	float countDownUpper;	/* 计时上限 */
	float counting;	/* 正在计时 */
private:
	Sprite* sprite;
	LabelAtlas* labelAtlas;
};

#endif