#include "HeadImage.h"
#include "PlayerOrder.h"

SpriteFrameCache* PlayerOrder::spriteFrameCache = nullptr;

#define HORIZONAL_INTERVAL_HEADIMAGE_PLAYERORDER 10	/* 头像和玩家命令之间的水平空隙是10 */

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
	/* 初始化精灵帧缓存 */
	spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->addSpriteFramesWithFile("Image/playerOrderState.plist",
		"Image/playerOrderState.png");

	sprite = Sprite::create();	/* sprite赋值 */
	this->addChild(sprite);

	this->setContentSize(sprite->getContentSize());	/* 设置PlayerOrder对象尺寸，这样做可能存在问题 */

	this->setPlayerOrderState(ORDERREADY);	/* 初始状态是 准备 */

	return true;
}

void PlayerOrder::setPlayerOrderState(PlayerOrderState _playerOrderState){
	auto spriteFrame = createSpriteFrameWithPlayerOrderState(_playerOrderState);
	sprite->setSpriteFrame(spriteFrame);
	this->setContentSize(sprite->getContentSize());	/* 防止因为多个spriteFrame大小不一致时造成问题 */
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