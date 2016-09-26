#include "GameAnimation.h"

GameAnimation* GameAnimation::gameAnimation = nullptr;

GameAnimation* GameAnimation::getInstance(){
	if (gameAnimation == nullptr){
		gameAnimation = new GameAnimation();
		if (gameAnimation && gameAnimation->init()){
			gameAnimation->autorelease();
			gameAnimation->retain();		/* ��ֹ���ڴ���������� */
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
	/* ����ͼƬ֡������� */
	auto frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("Image/win.plist", "Image/win.png");

	int iFrameNum = 6;
	SpriteFrame* frame = nullptr;
	Vector<SpriteFrame*> frameVec;

	/* ��һ���б�������SpriteFrame���� */
	for (int i = 0; i <= iFrameNum; ++i){
		frame = frameCache->getSpriteFrameByName(StringUtils::format("win_%d.png", i));
		frameVec.pushBack(frame);
	}
	
	/* ʹ��SpriteFrame�б����������� */
	auto animation = Animation::createWithSpriteFrames(frameVec);
	animation->setDelayPerUnit(0.25f);

	/* �����������һ������ */
	winAction = Animate::create(animation);

	return true;
}

Animate* GameAnimation::getWinAnimation() const {
	return this->winAction;
}

bool GameAnimation::initLostAnimation(){
	/* ����ͼƬ֡������� */
	auto frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("Image/lost.plist", "Image/lost.png");

	int iFrameNum = 7;
	SpriteFrame* frame = nullptr;
	Vector<SpriteFrame*> frameVec;

	/* ��һ���б�������SpriteFrame���� */
	for (int i = 0; i <= iFrameNum; ++i){
		frame = frameCache->getSpriteFrameByName(StringUtils::format("lost_%d.png", i));
		frameVec.pushBack(frame);
	}

	/* ʹ��SpriteFrame�б����������� */
	auto animation = Animation::createWithSpriteFrames(frameVec);
	animation->setDelayPerUnit(0.25f);

	/* �����������һ������ */
	lostAction = Animate::create(animation);

	return true;
}

Animate* GameAnimation::getLostAnimation() const {
	return this->lostAction;
}