#include "PokerController.h"

PokerController* PokerController::pokerController = nullptr;

PokerController* PokerController::getInstance(){
	if (pokerController == nullptr){
		pokerController = new PokerController();
		if (pokerController && pokerController->init()){
			pokerController->autorelease();
			pokerController->retain();		/* 避免被Cocos2dx的内存管理工具回收 */
			return pokerController;
		}else{
			CC_SAFE_DELETE(pokerController);
		}
		return nullptr;
	}
	return pokerController;
}

bool PokerController::init(){
	if (!Layer::init()){
		return false;
	}

	/* 将打包好的图片经过SpriteFrameCache处理，然后可以直接获取 */
	pokerCache = SpriteFrameCache::getInstance();
	pokerCache->addSpriteFramesWithFile("Image/Joker.plist", "Image/Joker.png");

	return true;
}

int PokerController::calcPokerValue(PokerType type, int order /* = 0 */){
	int result = 0;
	switch (type){
	case BLACKJOKER:
	case REDJOKER:
	case BACK:result = type; break;
	default: result = type * SINGLETYPECARDNUM + order - 1; break;
	}
	return result;
}

/* 根据牌值返回对应的精灵，此精灵位置没有经过指定 */
Sprite* PokerController::getPokerWithValue(PokerType type, int order){
	int result = calcPokerValue(type, order);
	std::string str = StringUtils::format("%d.png", result);
	return Sprite::createWithSpriteFrameName(str);
} 