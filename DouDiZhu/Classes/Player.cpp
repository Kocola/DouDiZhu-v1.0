#include "CountDown.h"
#include "GlobalDefine.h"
#include "GlobalFunc.h"
#include "HeadImage.h"
#include "MusicController.h"
#include "Player.h"
#include "PlayerOrder.h"

#define DEBUG

#ifdef DEBUG
#include "PokerController.h"
#endif // DEBUG

#define HORIZONAL_INTERVAL_HEADIMAGE_PLAYERORDER 10	/* 头像和玩家命令之间的水平空隙是10 */

Player::Player(GameScene* _gameScene, PlayerPosType _playerPosType){
	this->gameScene = _gameScene;
	this->playerPosType = _playerPosType;
}

Player* Player::create(GameScene* _gameScene, PlayerPosType _playerPosType){
	auto pRet = new(std::nothrow) Player(_gameScene, _playerPosType);
	if (pRet && pRet->init()){
		pRet->autorelease();
		return pRet;
	}else{
		delete pRet;
		pRet = nullptr;
		return pRet;
	}
}

bool Player::init(){
	isCall = false;
	callLandlordScore = 0; /* 初始化叫地主的分数是0 */
	isLandlord = false;
	isOutPoker = false;

	initPos();	/* 初始化所有位置相关的数据 */

	initHeadImage();
	initPlayerOrder();
	initCountDown();

	displayOutcardMaxWidth = 280;		/* 出牌显示的最大宽度 */

	return true;
}

void Player::initHeadImage(){
	headImage = HeadImage::create();
	headImage->setPosition(Point(headImagePosX, headImagePosY));
	this->addChild(headImage);
}

void Player::initPlayerOrder(){
	auto _headBoxSize = HeadImage::create()->getHeadBoxSize();

	playerOrder = PlayerOrder::create();	/* 头像默认是在屏幕左边 */
	auto _headImagePos = Point(headImagePosX, headImagePosY);	/* 玩家命令是和头像的位置相对的，因此要先获取对应玩家头像的位置 */
	auto tempOffset = (_headBoxSize.width / 2 +
		HORIZONAL_INTERVAL_HEADIMAGE_PLAYERORDER + playerOrder->getContentSize().width / 2);
	/* 左中的命令放在左，右的命令放在右 */
	auto _offsetBetweenHeadImageAndPlayerOrder = playerPosType != PLAYERINRIGHT ? tempOffset : -1 * tempOffset;
	playerOrder->setPosition(_headImagePos.x + _offsetBetweenHeadImageAndPlayerOrder, _headImagePos.y);
	playerOrder->setVisible(false);	/* 玩家命令初始化时不可见 */
	this->addChild(playerOrder);		/* 添加到节点树中 */
}

void Player::initCountDown(){
	countDown = CountDown::create();
	countDown->setPosition(Point(displayTimerStartX, headImage->getPosition().y));	/* 高度和头像对齐 */
	this->addChild(countDown);
	//countDown->setScale(0.5);	/* countDown本身无大小，因此直接调用setScale会出现意想不到的错误 */ 
	countDown->setCountDownScale(0.7);
}

void Player::initPos(){
	initHeadImagePos();
	initOutcardPos();
	initTimerPos();
}

void Player::initHeadImagePos(){
	/* 计算的是相对于Player的坐标 */
	if (playerPosType == PLAYERINLEFT || playerPosType == PLAYERINRIGHT){
		/* 这里是根据左中右的位置中左右属于电脑来进行设置，对于电脑来说，
			其头像位置就是电脑对象本身的位置，两者重合*/
		headImagePosX = 0;
		headImagePosY = 0;
		return;
	}

	auto _cardSize = PokerController::getInstance()->getPokerSize();
	auto _headBoxSize = HeadImage::create()->getHeadBoxSize();	/* 获取头像框的大小 */

	auto _curPlayerWorldPosY = this->convertToWorldSpace(this->getPosition()).y;	/* 获取当前Player中心点在世界坐标系的坐标 */
	auto _headImageWorldPosY = _curPlayerWorldPosY + _cardSize.height / 2 + _headBoxSize.height / 2 + HEIGHTBETWEENHEADIMAGEANDPLAYER;
	auto _headImageWorldPosX = WIDTHBETWEENSCREENANDHEADIMAGE;

	/* 计算头像相对于Player的坐标 */
	auto _headImagePos = this->convertToNodeSpace(Point(_headImageWorldPosX, headImagePosY));

	headImagePosX = _headImagePos.x;
	headImagePosY = _headImagePos.y;
}

void Player::initOutcardPos(){
	const float INTERVALBETWEENOUTCARDANDHEADIMAGE = 20.0;	/* 待出的牌的位置和头像之间的X轴距离，默认设置为20 */
	auto _headImagePosX = headImagePosX;
	auto _headImageWidth = HeadImage::create()->getContentSize().width;
	switch (playerPosType){
	case PLAYERINLEFT:
		displayOutcardStartX = _headImagePosX + _headImageWidth / 2 + INTERVALBETWEENOUTCARDANDHEADIMAGE;
		displayOutcardMiddleX = displayOutcardStartX + displayOutcardMaxWidth / 2;
		displayOutcardY = this->headImage->getPosition().y - this->headImage->getContentSize().height / 2;
		break;
	case PLAYERINMIDDLE:
		displayOutcardMiddleX = 0;
		displayOutcardStartX = displayOutcardMiddleX - displayOutcardMaxWidth / 2;
		displayOutcardY = this->headImage->getPosition().y + 15;
		break;
	case PLAYERINRIGHT:
		displayOutcardStartX = _headImagePosX - (_headImageWidth / 2 + INTERVALBETWEENOUTCARDANDHEADIMAGE + displayOutcardMaxWidth);
		displayOutcardMiddleX = displayOutcardStartX + displayOutcardMaxWidth / 2;
		displayOutcardY = this->headImage->getPosition().y - this->headImage->getContentSize().height / 2;
		break;
	default:CC_ASSERT(0 != 0); break;
	}
}

void Player::insertCards(const Vector<Poker*>& _pokers){
	this->removeAllCards();	/* 删除所有已有的扑克 */
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

void Player::removeCard(Poker* _poker){
	if (_poker == nullptr) return;
	this->removeChild(_poker);
	pokers.eraseObject(_poker);
}

void Player::removeAllCards(){
	for (int i = 0; i < pokers.size(); ++i){
		pokers.at(i)->removeFromParent();
	}
	pokers.clear();
}

void Player::setHeadImagePos(Point _pointInWorld){
	/* 传入的坐标必须是世界坐标系下的坐标，此时将其转化成当前节点坐标系下的坐标
		这种方式的好处是，通过两次转化，可以保证坐标和所在的节点及层次无关*/
	auto _pointInPlayer = this->convertToNodeSpace(_pointInWorld);
	/* 设置头像的坐标，这个函数主要用于设置手动玩家的坐标
		电脑的头像和其本身的坐标是一样的*/
	headImage->setPosition(_pointInPlayer);

	/* 更新出牌位置和倒计时位置 */
	this->initOutcardPos();
	this->initTimerPos();
}

void Player::initTimerPos(){
	const float INTERVALBETWEENTIMERANDHEADIMAGE = 20.0;	/* 待出的牌的位置和头像之间的X轴距离，默认设置为20 */
	auto _headImageX = headImage->getPosition().x;
	switch (playerPosType){
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
		_pokers.at(i)->setScale(SCALE);
		_pokers.at(i)->showFront();
		_pokers.at(i)->setPosition(Point(_displayStartX + cardWidth / 2 + i * intervalBetweenCards, _displayY));
	}
}

void Player::showOutcardInScene(){
	if (outcardsInScene.size() == 0) return;	/* 如果卡牌数量是0，那么不需要排序 */
	CC_ASSERT(headImage != nullptr);	/* 调用这个函数时，headImage必须已经设置好 */
	const float SCALE = 0.6;
	//float _displayOutcardMiddleX = displayOutcardStartX + displayOutcardMaxWidth / 2;	/* 计算显示出牌的中心位置 */
	/* 将头像的底部Y坐标作为所出牌的中心Y坐标 */
	//float _displayOutcardY = this->headImage->getPosition().y - this->headImage->getContentSize().height / 2;
	displayCard(outcardsInScene, displayOutcardMaxWidth, displayOutcardStartX, displayOutcardMiddleX, displayOutcardY, false, SCALE);
}

void Player::showCountDown(float _totalCount /* = 15 */){
	countDown->setCountDownUpper(_totalCount);
	countDown->startCountDown();
	countDown->setVisible(true);
}

void Player::stopCountDown(){
	if (countDown == nullptr) return;
	countDown->setVisible(false);
	countDown->stopCountDown();
}

void Player::setOutcardInScene(const Vector<Poker*>& _pokers){
	outcardsInScene = _pokers;
}

void Player::deleteOutcardInScene(){
	for (int i = 0; i < this->outcardsInScene.size(); ++i){
		outcardsInScene.at(i)->removeFromParent();
	}
	outcardsInScene.clear();
}

void Player::updateCallLandlordState(){
	/* 根据分数找到对应的叫地主状态 */
	PlayerOrderState _playerOrderState;
	CallLandlordEffect _callLandlordEffect;
	int _score = this->getCallLandlordScore();
	switch (_score){
	case 0:_playerOrderState = NOCALL; _callLandlordEffect = NOCALL_MUSIC; break;
	case 1:_playerOrderState = CALLONE; _callLandlordEffect = CALLONE_MUSIC; break;
	case 2:_playerOrderState = CALLTWO; _callLandlordEffect = CALLTWO_MUSIC; break;
	case 3:_playerOrderState = CALLTHREE; _callLandlordEffect = CALLTHREE_MUSIC; break;
	default: _playerOrderState = NOCALL; _callLandlordEffect = NOCALL_MUSIC; break;
	}

	CC_ASSERT(playerOrder != nullptr);

	/* 播放对应的音效 */
	MusicController::getInstance()->playCallLandlordEffect(_callLandlordEffect);

	playerOrder->setPlayerOrderState(_playerOrderState);
	playerOrder->setVisible(true);
}

//void Player::test(const Vector<Poker*>& _pokers){
//	showOutcardInScene(_pokers);
//	showCountDown(18);
//}