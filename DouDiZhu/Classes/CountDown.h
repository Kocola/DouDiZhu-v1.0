#ifndef __COUNT_DOWN_H__
#define __COUNT_DOWN_H__

#include "cocos2d.h"

using namespace cocos2d;

class CountDown : public Layer{
public:
	virtual bool init();
	CREATE_FUNC(CountDown);
public:
	void startCountDown(const std::function<void(void)>& _callback = [](){});
	void setCountDownUpper(float _countDownUpper){ this->countDownUpper = _countDownUpper; }
	void setCountDownScale(float _scale);
	void stopCountDown();	/* ֹͣ��ʱ */
private:
	void countDown(float delta);
	void resetCallback();
	void setCallback(const std::function<void(void)>& _callback) { this->callbackFunc = _callback; }
private:
	int curSpriteIndex;
	int totalSpriteNum;
	float countDownUpper;	/* ��ʱ���� */
	float counting;	/* ���ڼ�ʱ */
private:
	Sprite* sprite;
	LabelAtlas* labelAtlas;
private:
	std::function<void(void)> callbackFunc;
};

#endif