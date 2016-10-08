#include "GlobalFunc.h"
#include "Player.h"

bool Player::init(){
	isCall = false;
	callLandlordScore = 0; /* 初始化叫地主的分数是0 */
	isLandlord = false;
	isOutPoker = false;

	displayMiddleX = 0;
	displayLeftStartX = displayMiddleX - 5.0 / 12 * Director::getInstance()->getVisibleSize().width;

	return true;
}

void Player::addPoker(Poker* _poker){
	if (_poker == nullptr) return;
	pokers.pushBack(_poker);
	this->addChild(_poker);		/* 扑克和玩家关联，添加一个扑克，需要使其成为玩家的子结点，防止被回收 */
	GlobalFunc::sort(pokers);	/* 每次插入扑克后，进行排序，这样做效率较低，以后再优化 */
}

void Player::removePoker(Poker* _poker){
	if (_poker == nullptr) return;
	this->removeChild(_poker);
	pokers.eraseObject(_poker);
}

void Player::removeAllPoker(){
	this->removeAllChildren();
	pokers.clear();
}

void Player::insertCards(const Vector<Poker*>& _pokers){
	this->removeAllChildren();	/* 删除所有已有的扑克，现在可以正常使用，如果以后添加其它种类孩子，可能会造成误删除 */
	for (auto it : _pokers){	/* 将新的扑克插入 */
		pokers.pushBack(it);
	}
	GlobalFunc::sort(pokers);	/* 对所有扑克进行排序 */
	for (auto it : pokers){
		if (playerType == PLAYER){	/* 如果是手动玩家，那么显示正面，且可以点击 */
			it->showFront();	/* 扑克显示正面 */
			it->setCanClick(true);  /* 扑克可以点击 */
		}
		this->addChild(it);		/* 添加到结点树中 */
	}
}

void Player::updatePokerPos(){
	if (pokers.size() == 0 || playerType == COMPUTER) return;	/* 如果没有扑克了，直接返回，一般不会运行这句的 */
	float displayWidth = Director::getInstance()->getVisibleSize().width;	/* 屏幕宽度 */
	int cardsNum = pokers.size();	/* 卡牌数量 */
	float cardWidth = pokers.at(0)->getContentSize().width;	/* 卡牌宽度 */
	float maxDisplayWidth = displayWidth * 5 / 6;		/* 卡牌组合最大的显示宽度 */
	/* 卡牌应该显示的宽度，即可能彼此间会有重叠 */
	/* MIMIUM_CARDS_OVERLAPWIDTH 是确保卡牌之间始终会有重叠的部分，仅是为了美观 */
	float interval = (maxDisplayWidth - cardWidth) < (cardWidth - MIMIUM_CARDS_OVERLAPWIDTH) * (cardsNum - 1) ? (maxDisplayWidth - cardWidth) / (cardsNum - 1) : (cardWidth - MIMIUM_CARDS_OVERLAPWIDTH)   ;

	if (interval == (cardWidth - MIMIUM_CARDS_OVERLAPWIDTH)){	/* 如果卡牌之间不需要重叠，那么起始位置需要重新计算 */
		displayLeftStartX = cardsNum % 2 == 0 ?
			displayMiddleX - (cardsNum / 2) * interval:
			displayMiddleX - (cardsNum / 2 + 0.5) * interval;
	}
	float cardHeight = pokers.at(0)->getContentSize().height;	/* 卡牌高度 */
	/* 对卡牌位置进行重新设置 */
	for (int i = 0; i < cardsNum; ++i){
		auto _height = pokers.at(i)->getPosition().y;
		pokers.at(i)->setPosition(Point(displayLeftStartX + cardWidth / 2 + i * interval, _height));
	}
}