#include "GlobalFunc.h"
#include "HeadImage.h"
#include "Player.h"

#define DEBUG

#ifdef DEBUG
#include "PokerController.h"
#endif // DEBUG


bool Player::init(){
	isCall = false;
	callLandlordScore = 0; /* 初始化叫地主的分数是0 */
	isLandlord = false;
	isOutPoker = false;

	displayCardMiddleX = 0;
	displayCardStartX = displayCardMiddleX - 5.0 / 12 * Director::getInstance()->getVisibleSize().width;

	headImage = HeadImage::create();
	//this->addChild(headImage);

	displayOutcardMaxWidth = 60;		/* 出牌显示的最大宽度 */

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
		it->setVisible(false);	/* 默认扑克不可见 */
		if (playerType == PLAYER){	/* 如果是手动玩家，那么显示正面，且可以点击 */
			it->showFront();	/* 扑克显示正面 */
			it->setCanClick(true);  /* 扑克可以点击 */
			it->setVisible(true);
		}
		this->addChild(it);		/* 添加到结点树中 */
	}
}

void Player::setHeadImagePos(Point _pointInWorld){
	/* 传入的坐标必须是世界坐标系下的坐标，此时将其转化成当前节点坐标系下的坐标
		这种方式的好处是，通过两次转化，可以保证坐标和所在的节点及层次无关*/
	auto _pointInPlayer = this->convertToNodeSpace(_pointInWorld);
	/* 设置头像的坐标，这个函数主要用于设置手动玩家的坐标
		电脑的头像和其本身的坐标是一样的*/
	headImage->setPosition(_pointInPlayer);

	/* 更新出牌位置和倒计时位置 */
	this->calcOutcardPos();
	this->calcTimerPos();
}

void Player::calcOutcardPos(){
	const float INTERVALBETWEENOUTCARDANDHEADIMAGE = 20.0;	/* 待出的牌的位置和头像之间的X轴距离，默认设置为20 */
	auto _headImageX = headImage->getPosition().x;
	switch (playerPos){
	case PLAYERINLEFT:
		displayOutcardStartX = _headImageX + INTERVALBETWEENOUTCARDANDHEADIMAGE;
		displayOutcardMiddleX = displayOutcardStartX + displayOutcardMaxWidth / 2;
		break;
	case PLAYERINMIDDLE:
		displayOutcardMiddleX = 0;
		displayOutcardStartX = displayOutcardMiddleX - displayOutcardMaxWidth / 2;
		break;
	case PLAYERINRIGHT:
		displayOutcardStartX = _headImageX - INTERVALBETWEENOUTCARDANDHEADIMAGE - displayOutcardMaxWidth;
		displayOutcardMiddleX = displayOutcardStartX + displayOutcardMaxWidth / 2;
		break;
	default:CC_ASSERT(0 != 0); break;
	}
}

void Player::calcTimerPos(){
	const float INTERVALBETWEENTIMERANDHEADIMAGE = 20.0;	/* 待出的牌的位置和头像之间的X轴距离，默认设置为20 */
	auto _headImageX = headImage->getPosition().x;
	switch (playerPos){
	case PLAYERINLEFT: displayTimerStartX = _headImageX + INTERVALBETWEENTIMERANDHEADIMAGE;
		break;
	case PLAYERINMIDDLE: displayTimerStartX = 0;
		break;
	case PLAYERINRIGHT: displayTimerStartX = _headImageX - INTERVALBETWEENTIMERANDHEADIMAGE;
		break;
	default:CC_ASSERT(0 != 0);
		break;
	}
}

void Player::displayCard(const Vector<Poker*>& _pokers, float _displayMaxWidth, float _displayStartX, float _displayMiddleX, float _displayY, bool isAdded, const float SCALE /* = 1.0 */){
	//if (_pokers.size() == 0) return;	/* 如果卡牌数量是0，那么不需要排序 */
	int cardsNum = _pokers.size();	/* 卡牌数量 */
	float cardWidth = _pokers.at(0)->getContentSize().width * SCALE;
	float cardHeight = _pokers.at(0)->getContentSize().height * SCALE;
	/* 两张牌之间最大的间隙 */
	float maxIntervalBetweenCards = cardWidth - MIMIUM_CARDS_OVERLAPWIDTH * SCALE;
	float intervalBetweenCards = (_displayMaxWidth - cardWidth) < maxIntervalBetweenCards * (cardsNum - 1) ?
		(_displayMaxWidth - cardWidth) / (cardsNum - 1) : maxIntervalBetweenCards;

	/* 卡牌之间的间隙达到最大间隙时，此时需要以显示区中点展开显示，不然整体的显示会左倾，造成错误 */
	if (intervalBetweenCards == maxIntervalBetweenCards){
		_displayStartX = cardsNum % 2 == 0 ?
			_displayMiddleX - (cardsNum / 2) * intervalBetweenCards :
			_displayMiddleX - (cardsNum / 2 + 0.5) * intervalBetweenCards;
	}
	/* 显示卡牌的位置 */
	for (int i = 0; i < cardsNum; ++i){
		if (isAdded == false){
			this->addChild(_pokers.at(i));
		}else{
			_displayY = _pokers.at(i)->getPosition().y;	/* 如果是已经添加过的扑克（也就是待出的牌）
														，那么获取其Y坐标，如果统一的Y坐标，那么会产生BUG*/
		}
		_pokers.at(i)->setPosition(Point(_displayStartX + cardWidth / 2 + i * intervalBetweenCards, _displayY));
	}
}

void Player::showOutcardInScene(const Vector<Poker*> _pokers){
	if (_pokers.size() == 0) return;	/* 如果卡牌数量是0，那么不需要排序 */
	CC_ASSERT(headImage != nullptr);	/* 调用这个函数时，headImage必须已经设置好 */
	const int SCALE = 0.6;
	//float _displayOutcardMiddleX = displayOutcardStartX + displayOutcardMaxWidth / 2;	/* 计算显示出牌的中心位置 */
	/* 将头像的底部Y坐标作为所出牌的中心Y坐标 */
	float _displayOutcardY = this->headImage->getPosition().y - this->headImage->getContentSize().height / 2;
	displayCard(_pokers, displayOutcardMaxWidth, displayOutcardStartX, displayOutcardMiddleX, _displayOutcardY, false, SCALE);
}

void Player::updatePokerPos(){
	if (pokers.size() == 0 || playerType == COMPUTER) return;	/* 如果没有扑克了，直接返回，一般不会运行这句的 */
	float _displayCardMaxWidth = Director::getInstance()->getVisibleSize().width * 5.0 / 6;
	float _displayCardY = 0;
	displayCard(pokers, _displayCardMaxWidth, displayCardStartX, displayCardMiddleX, _displayCardY, true);
}

void Player::test(const Vector<Poker*>& _pokers){
	showOutcardInScene(_pokers);
}