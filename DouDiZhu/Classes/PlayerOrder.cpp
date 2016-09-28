#include "HeadImage.h"
#include "PlayerOrder.h"

SpriteFrameCache* PlayerOrder::spriteFrameCache = nullptr;

#define HORIZONAL_INTERVAL_HEADIMAGE_PLAYERORDER 10	/* ͷ����������֮���ˮƽ��϶��10 */

PlayerOrder* PlayerOrder::create(){
	auto pRet = new(std::nothrow) PlayerOrder();
	if (pRet && pRet->init()){
		pRet->autorelease();
	}else{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool PlayerOrder::init(){
	/* ��ʼ������֡���� */
	spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->addSpriteFramesWithFile("Image/playerOrderState.plist",
		"Image/playerOrderState.png");

	sprite = Sprite::create();	/* sprite��ֵ */
	this->addChild(sprite);

	this->setContentSize(sprite->getContentSize());	/* ����PlayerOrder����ߴ磬���������ܴ������� */

	this->setPlayerOrderState(ORDERREADY);	/* ��ʼ״̬�� ׼�� */

	return true;
}

void PlayerOrder::setPlayerOrderState(PlayerOrderState _playerOrderState){
	auto spriteFrame = createSpriteFrameWithPlayerOrderState(_playerOrderState);
	sprite->setSpriteFrame(spriteFrame);
	this->setContentSize(sprite->getContentSize());	/* ��ֹ��Ϊ���spriteFrame��С��һ��ʱ������� */
}

SpriteFrame* PlayerOrder::createSpriteFrameWithPlayerOrderState(PlayerOrderState _playerOrderState){
	if (_playerOrderState == ORDERREADY){
		return spriteFrameCache->getSpriteFrameByName("ready.png");
	}
	else if (_playerOrderState == NOCALL){
		return spriteFrameCache->getSpriteFrameByName("flagcall_0.png");
	}
	else if (_playerOrderState == CALLONE){
		return spriteFrameCache->getSpriteFrameByName("flagcall_1.png");
	}
	else if (_playerOrderState == CALLTWO){
		return spriteFrameCache->getSpriteFrameByName("flagcall_2.png");
	}
	else if (_playerOrderState == CALLTHREE){
		return spriteFrameCache->getSpriteFrameByName("flagcall_3.png");
	}
	else if (_playerOrderState == PASS){
		return spriteFrameCache->getSpriteFrameByName("pass.png");
	}
}