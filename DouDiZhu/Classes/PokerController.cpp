#include "PokerController.h"

PokerController* PokerController::pokerController = nullptr;
Vector<Poker*> PokerController::pokers;

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

int PokerController::calcCardValue(PokerType type, int order /* = 0 */){
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
Sprite* PokerController::getCardWithValue(PokerType type, int order){
	int result = calcCardValue(type, order);
	std::string str = StringUtils::format("%d.png", result);
	return Sprite::createWithSpriteFrameName(str);
}

Vector<Poker*> PokerController::getRandomCards(){
	initCards();
	randomCards();
	return pokers;
}

void PokerController::initCards(){
	pokers.clear();
	/* 方块DIAMOND */
	for (int i = 1; i <= SINGLETYPECARDNUM; ++i){
		auto poker = Poker::create(DIAMOND, i);
		pokers.pushBack(poker);
	}
	/* 梅花HEART */
	for (int i = 1; i <= SINGLETYPECARDNUM; ++i){
		auto poker = Poker::create(CLUB, i);
		pokers.pushBack(poker);
	}
	/* 红桃HEART */
	for (int i = 1; i <= SINGLETYPECARDNUM; ++i){
		auto poker = Poker::create(HEART, i);
		pokers.pushBack(poker);
	}
	/* 黑桃SPADE */
	for (int i = 1; i <= SINGLETYPECARDNUM; ++i){
		auto poker = Poker::create(SPADE, i);
		pokers.pushBack(poker);
	}
	/* 大小王 */
	pokers.pushBack(Poker::create(BLACKJOKER));
	pokers.pushBack(Poker::create(REDJOKER));
}

int PokerController::randomInt(int begin, int end){
	if (begin > end) std::swap(begin, end);
	int ret = begin + rand() % (end - begin);
	return ret;
}

void PokerController::randomCards(){
	srand((unsigned)time(0));	/* 开启随机种子 */
	for (int i = pokers.size(); i > 0; --i){
		pokers.swap(i - 1, randomInt(0, i));
	}
}