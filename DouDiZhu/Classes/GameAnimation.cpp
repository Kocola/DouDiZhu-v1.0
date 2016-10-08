#include "GameAnimation.h"

GameAnimation* GameAnimation::gameAnimation = nullptr;

GameAnimation* GameAnimation::getInstance(){
	if (gameAnimation == nullptr){
		gameAnimation = new GameAnimation();
		if (gameAnimation && gameAnimation->init()){
			gameAnimation->autorelease();
			gameAnimation->retain();		/* 防止被内存管理器回收 */
		}else{
			CC_SAFE_DELETE(gameAnimation);
		}
	}
	return gameAnimation;
}

bool GameAnimation::init(){
	bool tRet = false;
	do {
		CC_BREAK_IF(!initWinAnimation());
		CC_BREAK_IF(!initLostAnimation());
		tRet = true;
	} while (0);

	return tRet;
}

bool GameAnimation::initWinAnimation(){
	/* 加载图片帧到缓存池 */
	auto frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("Image/win.plist", "Image/win.png");

	int iFrameNum = 6;
	SpriteFrame* frame = nullptr;
	Vector<SpriteFrame*> frameVec;

	/* 用一个列表保存所有SpriteFrame对象 */
	for (int i = 0; i <= iFrameNum; ++i){
		frame = frameCache->getSpriteFrameByName(StringUtils::format("win_%d.png", i));
		frameVec.pushBack(frame);
	}
	
	/* 使用SpriteFrame列表创建动画对象 */
	auto animation = Animation::createWithSpriteFrames(frameVec);
	animation->setDelayPerUnit(0.25f);

	/* 将动画打包成一个动作 */
	winAction = Animate::create(animation);

	return true;
}

Animate* GameAnimation::getWinAnimation() const {
	return this->winAction;
}

bool GameAnimation::initLostAnimation(){
	/* 加载图片帧到缓存池 */
	auto frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("Image/lost.plist", "Image/lost.png");

	int iFrameNum = 7;
	SpriteFrame* frame = nullptr;
	Vector<SpriteFrame*> frameVec;

	/* 用一个列表保存所有SpriteFrame对象 */
	for (int i = 0; i <= iFrameNum; ++i){
		frame = frameCache->getSpriteFrameByName(StringUtils::format("lost_%d.png", i));
		frameVec.pushBack(frame);
	}

	/* 使用SpriteFrame列表创建动画对象 */
	auto animation = Animation::createWithSpriteFrames(frameVec);
	animation->setDelayPerUnit(0.25f);

	/* 将动画打包成一个动作 */
	lostAction = Animate::create(animation);

	return true;
}

Animate* GameAnimation::getLostAnimation() const {
	return this->lostAction;
}