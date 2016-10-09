#include "CountDown.h"

bool CountDown::init(){
	if (!Layer::init()){
		return false;
	}

	auto _spriteFrameCache = SpriteFrameCache::getInstance();
	_spriteFrameCache->addSpriteFramesWithFile("clock/clock.plist", "clock/clock.png");

	totalSpriteNum = 2;
	curSpriteIndex = 0;
	countDownUpper = 15; /* 倒计时默认是15S */
	counting = countDownUpper;

	sprite = Sprite::create();
	this->addChild(sprite, 0);
	auto spriteFrame = _spriteFrameCache->getInstance()->getSpriteFrameByName(
		StringUtils::format("clock%d.png", curSpriteIndex));
	sprite->setSpriteFrame(spriteFrame);

	labelAtlas = LabelAtlas::create(StringUtils::format("%d", countDownUpper), "clock/clock_num.png", 38, 37, '0');
	labelAtlas->setAnchorPoint(Point(0.5, 0.5));	/* 重新设置锚点 */
	this->addChild(labelAtlas, 1);

	return true;
}

void CountDown::countDown(float delta){
	counting -= delta;

	/* 大于0小于5时，显示闪烁效果 */
	if (counting >0 && counting <= 5){
		curSpriteIndex = (curSpriteIndex + 1) % totalSpriteNum;
		auto _spriteFrameCache = SpriteFrameCache::getInstance();
		sprite->setSpriteFrame(_spriteFrameCache->getInstance()->getSpriteFrameByName(
			StringUtils::format("clock%d.png", curSpriteIndex)));
	}

	/* 计时时间已到 */
	if (counting <= 0){
		this->unschedule(schedule_selector(CountDown::countDown));
		//counting = countDownUpper;	/* 重置计时初始值 */
	}else{
		labelAtlas->setString(StringUtils::format("%d", (int)counting));
	}
}

void CountDown::startCountDown(){
	this->counting = countDownUpper;	/* 每次显示前都要重置计数 */
	this->schedule(schedule_selector(CountDown::countDown), 0.2f);
}

void CountDown::stopCountDown(){
	this->unschedule(schedule_selector(CountDown::countDown));
}

void CountDown::setCountDownScale(float _scale){
	CC_ASSERT(sprite != nullptr); 
	this->labelAtlas->setScale(_scale);
	this->sprite->setScale(_scale);
}