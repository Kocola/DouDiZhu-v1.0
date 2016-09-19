#include "PokerController.h"

PokerController* PokerController::pokerController = nullptr;

PokerController* PokerController::getInstance(){
	if (pokerController == nullptr){
		pokerController = new PokerController();
		if (pokerController && pokerController->init()){
			pokerController->autorelease();
			pokerController->retain();		/* ���ⱻCocos2dx���ڴ�����߻��� */
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

	/* ������õ�ͼƬ����SpriteFrameCache����Ȼ�����ֱ�ӻ�ȡ */
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

/* ������ֵ���ض�Ӧ�ľ��飬�˾���λ��û�о���ָ�� */
Sprite* PokerController::getPokerWithValue(PokerType type, int order){
	int result = calcPokerValue(type, order);
	std::string str = StringUtils::format("%d.png", result);
	return Sprite::createWithSpriteFrameName(str);
}