#include "GameScene.h"
#include "MusicController.h"
#include "Player.h"
#include "Poker.h"
#include "PokerController.h"

Poker::Poker(GameScene* _gameScene, PokerType type, int value){
	this->setCanClick(false);	/* 默认不可点击 */
	this->setSelect(false);
	this->setPokerType(type);
	this->setValue(value);
	this->setGameScene(_gameScene);

	createSprite();		/* 创建两个扑克精灵 */
	this->showBack();	  /* 显示正面 */
}

Poker* Poker::create(GameScene* _gameScene, PokerType type, int value /* = 0 */){
	auto poker = new Poker(_gameScene, type, value);
	if (poker && poker->init()){
		poker->autorelease();
		return poker;
	}else{
		CC_SAFE_DELETE(poker);
		poker = nullptr;
		return nullptr;
	}
}

Poker *Poker::clone() const{
	return create(gameScene, pokerType, value);
}

void Poker::createSprite(){
	auto sprite = PokerController::getInstance()->getPokerWithValue(pokerType, value);
	this->setPoker(sprite);
	this->addChild(poker);
	backPoker = PokerController::getInstance()->getPokerWithValue(BACK);		/* 获取背面扑克精灵 */
	this->addChild(backPoker);

	/* 将无长宽的Node设置成和Sprite一样长宽 */
	auto spriteSize = poker->getContentSize();
	this->setAnchorPoint(Point(0.5, 0.5));
	this->setContentSize(spriteSize);

	/* 将精灵的位置和Node本身一致 */
	poker->setPosition(this->getContentSize() / 2);
	backPoker->setPosition(this->getContentSize() / 2);
}

bool Poker::init(){
	auto listener = EventListenerTouchOneByOne::create();		/* 创建单点触摸监听器 */
	listener->setSwallowTouches(true);	/* 禁止向下传递触摸 */
	/* 触摸开始 */
	listener->onTouchBegan = [&](Touch* touch, Event* event){
		auto rect = this->getBoundingBox();
		auto touchPos = this->getParent()->convertToNodeSpace(touch->getLocation());
		if (rect.containsPoint(touchPos) && canClick){
			/* 播放触摸牌的音效 */
			MusicController::getInstance()->playTouchCardEffect();
			if (!isSelect){
				/* 如果还未选择这张牌，那就将这张牌露出来 */
				selectedCardOut();
			}else{
				/* 否则这张牌已经被选择，将其放回 */
				selectedCardBack();
			}
			/* 检测当前牌是否可以另出牌按钮可按 */
			updateOutState();
			return true;
		}
		return false;		/* 这里返回false，触摸不会被吞掉 */
	};

	listener->onTouchMoved = [](Touch* touch, Event* event){
	};

	listener->onTouchEnded = [](Touch* touch, Event* event){
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void Poker::selectedCardOut(){
	log("Out !!!");
	this->isSelect = true;
	this->setPosition(Point(this->getPositionX(), this->getPositionY() + 10));
	gameScene->addWaitPlayOut(this);
	gameScene->sort();		/* 对待出的扑克进行排序 */
}

void Poker::selectedCardBack(){
	/* 从出牌中移除该张牌 */
	isSelect = false;
	this->setPosition(Point(this->getPositionX(), this->getPositionY() - 10));
	gameScene->deleteWaitPlayOut(this);
}

void Poker::showFront(){
	this->poker->setVisible(true);
	this->backPoker->setVisible(false);
}

void Poker::showBack(){
	this->poker->setVisible(false);
	this->backPoker->setVisible(true);
}

void Poker::updateOutState(){
	gameScene->updateOutState();
}