#include "GameScene.h"
#include "Player.h"
#include "Poker.h"
#include "PokerController.h"

Poker::Poker(GameScene* _gameScene, PokerType type, int value){
	this->setCanClick(false);	/* Ĭ�ϲ��ɵ�� */
	this->setSelect(false);
	this->setPokerType(type);
	this->setValue(value);
	this->setGameScene(_gameScene);

	createSprite();		/* ���������˿˾��� */
	this->showBack();	  /* ��ʾ���� */
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
	backPoker = PokerController::getInstance()->getPokerWithValue(BACK);		/* ��ȡ�����˿˾��� */
	this->addChild(backPoker);

	/* ���޳����Node���óɺ�Spriteһ������ */
	auto spriteSize = poker->getContentSize();
	this->setAnchorPoint(Point(0.5, 0.5));
	this->setContentSize(spriteSize);

	/* �������λ�ú�Node����һ�� */
	poker->setPosition(this->getContentSize() / 2);
	backPoker->setPosition(this->getContentSize() / 2);
}

bool Poker::init(){
	auto listener = EventListenerTouchOneByOne::create();		/* �������㴥�������� */
	listener->setSwallowTouches(true);	/* ��ֹ���´��ݴ��� */
	/* ������ʼ */
	listener->onTouchBegan = [&](Touch* touch, Event* event){
		auto rect = this->getBoundingBox();
		auto touchPos = this->getParent()->convertToNodeSpace(touch->getLocation());// this->getParent()->convertToNodeSpace(touch->getLocationInView());
		/*log("RECT : %f, %f, %f, %f\n", rect.getMinX(), rect.getMinY(), rect.getMaxX(), rect.getMaxY());
		log("Touch Pos : %f, %f\n", touchPos.x, touchPos.y);*/
		if (rect.containsPoint(touchPos) && canClick){
			/* log("TouchBegin!!!"); */
			if (!isSelect){
				/* �����δѡ�������ƣ��Ǿͽ�������¶���� */
				selectedCardOut();
			}else{
				/* �����������Ѿ���ѡ�񣬽���Ż� */
				selectedCardBack();
			}
			///* ����ǳ��ƣ��ͽ����Ŵ����漯����ɾ�� */
			//NotificationCenter::getInstance()->postNotification("UpdatePokerPos", this);
			//this->removeFromParent();
			return true;
		}
		return false;		/* ���ﷵ��false���������ᱻ�̵� */
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
	gameScene->sort();		/* �Դ������˿˽������� */
}

void Poker::selectedCardBack(){
	/* �ӳ������Ƴ������� */
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