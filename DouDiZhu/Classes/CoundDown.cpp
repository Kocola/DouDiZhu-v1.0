#include "CountDown.h"

bool CountDown::init(){
	if (!Layer::init()){
		return false;
	}

	auto _spriteFrameCache = SpriteFrameCache::getInstance();
	_spriteFrameCache->addSpriteFramesWithFile("clock/clock.plist", "clock/clock.png");

	totalSpriteNum = 2;
	curSpriteIndex = 0;
	countDownUpper = 15; /* ����ʱĬ����15S */
	counting = countDownUpper;

	sprite = Sprite::create();
	this->addChild(sprite, 0);
	auto spriteFrame = _spriteFrameCache->getInstance()->getSpriteFrameByName(
		StringUtils::format("clock%d.png", curSpriteIndex));
	sprite->setSpriteFrame(spriteFrame);

	labelAtlas = LabelAtlas::create(StringUtils::format("%d", countDownUpper), "clock/clock_num.png", 38, 37, '0');
	labelAtlas->setAnchorPoint(Point(0.5, 0.5));	/* ��������ê�� */
	this->addChild(labelAtlas, 1);

	return true;
}

void CountDown::countDown(float delta){
	counting -= delta;

	/* ����0С��5ʱ����ʾ��˸Ч�� */
	if (counting >0 && counting <= 5){
		curSpriteIndex = (curSpriteIndex + 1) % totalSpriteNum;
		auto _spriteFrameCache = SpriteFrameCache::getInstance();
		sprite->setSpriteFrame(_spriteFrameCache->getInstance()->getSpriteFrameByName(
			StringUtils::format("clock%d.png", curSpriteIndex)));
	}

	/* ��ʱʱ���ѵ� */
	if (counting <= 0){
		this->unschedule(schedule_selector(CountDown::countDown));
		//counting = countDownUpper;	/* ���ü�ʱ��ʼֵ */
	}else{
		labelAtlas->setString(StringUtils::format("%d", (int)counting));
	}
}

void CountDown::startCountDown(){
	this->counting = countDownUpper;	/* ÿ����ʾǰ��Ҫ���ü��� */
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