#include "GlobalFunc.h"
#include "Player.h"

Player::Player(){
	isCall = false;
	callPoint = 0;
	isLandlord = false;
	isOutPoker = false;

	displayMiddleX = 0;
	displayLeftStartX = displayMiddleX - 5.0 / 12 * Director::getInstance()->getVisibleSize().width;

	this->retain();
}

Player::~Player(){
	this->release();
}

bool Player::init(){
	isCall = false;
	callPoint = 0;
	isLandlord = false;
	isOutPoker = false;

	displayMiddleX = 0;
	displayLeftStartX = displayMiddleX - 5.0 / 12 * Director::getInstance()->getVisibleSize().width;

	return true;
}

void Player::addPoker(Poker* _poker){
	if (_poker == nullptr) return;
	poker.pushBack(_poker);
	this->addChild(_poker);		/* �˿˺���ҹ��������һ���˿ˣ���Ҫʹ���Ϊ��ҵ��ӽ�㣬��ֹ������ */
	GlobalFunc::sort(poker);	/* ÿ�β����˿˺󣬽�������������Ч�ʽϵͣ��Ժ����Ż� */
}

void Player::removePoker(Poker* _poker){
	if (_poker == nullptr) return;
	this->removeChild(_poker);
	poker.eraseObject(_poker);
}

void Player::updatePokerPos(){
	if (poker.size() == 0 || playerType == COMPUTER) return;	/* ���û���˿��ˣ�ֱ�ӷ��أ�һ�㲻���������� */
	float displayWidth = Director::getInstance()->getVisibleSize().width;	/* ��Ļ��� */
	int cardsNum = poker.size();	/* �������� */
	float cardWidth = poker.at(0)->getContentSize().width;	/* ���ƿ�� */
	float maxDisplayWidth = displayWidth * 5 / 6;		/* �������������ʾ��� */
	/* ����Ӧ����ʾ�Ŀ�ȣ������ܱ˴˼�����ص� */
	/* MIMIUM_CARDS_OVERLAPWIDTH ��ȷ������֮��ʼ�ջ����ص��Ĳ��֣�����Ϊ������ */
	float interval = maxDisplayWidth < (cardWidth - MIMIUM_CARDS_OVERLAPWIDTH) * cardsNum ? (maxDisplayWidth - cardWidth) / (cardsNum - 1) : (cardWidth - MIMIUM_CARDS_OVERLAPWIDTH)   ;

	if (interval == (cardWidth - MIMIUM_CARDS_OVERLAPWIDTH)){	/* �������֮�䲻��Ҫ�ص�����ô��ʼλ����Ҫ���¼��� */
		displayLeftStartX = cardsNum % 2 == 0 ?
			displayMiddleX - (cardsNum / 2) * interval:
			displayMiddleX - (cardsNum / 2 + 0.5) * interval;
	}
	float cardHeight = poker.at(0)->getContentSize().height;	/* ���Ƹ߶� */
	/* �Կ���λ�ý����������� */
	for (int i = 0; i < cardsNum; ++i){
		poker.at(i)->setPosition(Point(displayLeftStartX + cardWidth / 2 + i * interval, 0));
	}
}