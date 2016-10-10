#include <algorithm>
#include <ctime>
#include "ComputerPlayer.h"
#include "GameAnimation.h"
#include "GameRules.h"
#include "GameScene.h"
#include "GlobalFunc.h"
#include "HeadImage.h"
#include "HolderPlayer.h"
#include "Lobby.h"
#include "MusicController.h"
#include "OutCards.h"
#include "Player.h"
#include "PlayerOrder.h"
#include "Poker.h"
#include "PokerController.h"

using namespace std;

#define HORIZONAL_INTERVAL_HEADIMAGE_PLAYERORDER 10	/* 头像和玩家命令之间的水平空隙是10 */

Scene* GameScene::createScene(){
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init(){
	if (!Layer::init()){
		return false;
	}

	winSprite = nullptr;
	lostSprite = nullptr;
	landlordPlayer = nullptr;

	initBackground();	/* 初始化背景 */
	initObserver();
	this->scheduleOnce(schedule_selector(GameScene::gameStart), 1.0f);

	return true;
}

void GameScene::update(float delta){
	/*static float deltaCount = 0;
	deltaCount += delta;
	if (deltaCount < 3.0f && gameState == OUTCARD){
		return;
	}
	deltaCount = 0;*/
	switch (gameState){
	case READY: ready(); break;
	case DEAL: dealCard(); initLandlordCard(); displayLandlordCard(); 
		this->setGameState(CALLLANDLORD); break;
	case CALLLANDLORD: callLandlord(); break;
	case CHOOSELANDLORD: 
		chooseLandlord(); 
		updateHeadImage();  
		outCardForLandlord(); 
		initOutCardOrder();
		initPlayerVector(); 
		initLastOutCard();
		this->setGameState(OUTCARD);
		break;
	case OUTCARD: this->scheduleOnce(schedule_selector(GameScene::outCardInOrder), 2.0f);
		this->unschedule(schedule_selector(GameScene::update));
		break;
		/*case END: this->unschedule(schedule_selector(GameScene::update));
			this->gameOver();
			break;*/
	default: break;
	}
}

void GameScene::initObserver(){
	/* 更新叫地主顺序 */
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(GameScene::updateCallLandlordOrder),
		UPDATECALLLANDLORDORDER, nullptr);
	/* 更新出牌顺序 */
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(GameScene::updateOutCardOrder),
		UPDATEOUTCARDORDER, nullptr);
	/* 更新桌面上一手牌 */
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(GameScene::updateLastOutCards),
		UPDATELASTOUTCARD, nullptr);
	/* 设置游戏状态为结束 */
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(GameScene::setGameOver),
		SETGAMEOVER, nullptr);
	/* 设置游戏状态为选择地主 */
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(GameScene::setGameStateChooseLandlord),
		SETGAMESTATECHOOSELANDLORD, nullptr);
	/* 出牌时播放对应的出牌音乐 */
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(GameScene::playOutCardInSceneMusic),
		PLAYEROUTCARDINSCENEMUSIC, nullptr);
}

void GameScene::initBackground(){
	auto bgSprite = Sprite::create("Image/background.png");
	bgSprite->setPosition(Point(Director::getInstance()->getVisibleSize() / 2));
	this->addChild(bgSprite, 0);
}

void GameScene::initPlayer(){
	auto visbleSize = Director::getInstance()->getVisibleSize();	/* 获取屏幕的可见尺寸 */

	/* 计算电脑的位置 */
	auto computerPlayerOnePos = Point(visbleSize.width * 11 / 12, visbleSize.height * 3 / 4);
	auto computerPlayerTwoPos = Point(visbleSize.width / 12, visbleSize.height * 3 / 4);

	/* 创建玩家示例（包括电脑），设置各自的类型，并且将其设为GameScene场景的子结点，防止被回收 */
	player = HolderPlayer::create(PLAYERINMIDDLE);
	this->addChild(player);		/* 将玩家设置成场景的子结点，防止被回收 */
	player->setPlayerType(PLAYER);	/* 设置玩家类型 */
	computerPlayer_one = ComputerPlayer::create(PLAYERINRIGHT);
	this->addChild(computerPlayer_one);
	computerPlayer_one->setPlayerType(COMPUTER);
	computerPlayer_two = ComputerPlayer::create(PLAYERINLEFT);
	computerPlayer_two->setPlayerType(COMPUTER); 
	this->addChild(computerPlayer_two);

	/* 设置玩家和电脑的坐标 */
	player->setPosition(visbleSize.width / 2, DISPLAYCARDHEIGHT + PokerController::getPokerSize().height / 2);
	computerPlayer_one->setPosition(computerPlayerOnePos);
	computerPlayer_two->setPosition(computerPlayerTwoPos);

	player->initResource();
}

void GameScene::initPoker(){
	/* 利用PokerController来获取已随机的扑克 */
	this->pokers = PokerController::getRandomCards();
}

void GameScene::deletePlayer(){
	/* 删除玩家和电脑端的指针 */
 	this->removeChild(player);
	this->removeChild(computerPlayer_one);
	this->removeChild(computerPlayer_two);
	players.clear();	/* 删除玩家容器里的所有玩家指针，如果没有这一步，
						新的一局开始时，玩家无法出牌*/
}

void GameScene::chooseLandlord(){
	int maxlandlordscore = 0;
	if (player->getCallLandlordScore() > maxlandlordscore){
		maxlandlordscore = player->getCallLandlordScore();
		landlordPlayer = player;
	}
	if (computerPlayer_one->getCallLandlordScore() > maxlandlordscore){
		maxlandlordscore = computerPlayer_one->getCallLandlordScore();
		landlordPlayer = computerPlayer_one;
	}
	if (computerPlayer_two->getCallLandlordScore() > maxlandlordscore){
		maxlandlordscore = computerPlayer_two->getCallLandlordScore();
		landlordPlayer = computerPlayer_two;
	}
	/* 设置分数最高的人是地主，如果都没有叫，那么这里让手动玩家成为地主 */
	landlordPlayer = landlordPlayer == nullptr ? player : landlordPlayer;
	landlordPlayer->setLandlord(true);

	/* 播放获得地主的音乐 */
	MusicController::getInstance()->playCallLandlordEffect(LANDLORD_MUSIC);

	/* 隐藏所有玩家的叫分命令状态 */
	this->setPlayerOrderStateUnVisible();
}

void GameScene::updateHeadImage(){
	player->updateHeadImage();
	computerPlayer_one->updateHeadImage();
	computerPlayer_two->updateHeadImage();
}

void GameScene::outCardForLandlord(){
	landlordPlayer->insertCards(cardForLandlord);
	landlordPlayer->updateCardPos();	/* 添加扑克完毕后，重新排序 */

	cardForLandlord.clear();	/* 属于地主的牌发给地主后，置容器为空 */
}

void GameScene::callLandlord(){
	switch (callLandlordOrder){
	case 0: player->callLandlord(); break;
	case 1: computerPlayer_one->callLandlord(); break;
	case 2: computerPlayer_two->callLandlord(); break;
	default:  this->setGameState(CHOOSELANDLORD); break;
	}
}

/************************************************************************/
/*                             观察者（状态更新）                                         */
/************************************************************************/
void GameScene::updateCallLandlordOrder(Ref*){ 
	/* 这里不需要求余%，因为是当callLandlordOrder的值大于等于3时， 就进入选地主状态*/
	this->callLandlordOrder = this->callLandlordOrder + 1; 
}

void GameScene::updateOutCardOrder(Ref*){
	this->outcardOrder = (this->outcardOrder + 1) % 3;

	this->scheduleOnce(schedule_selector(GameScene::outCardInOrder), 1.0f);
}

void GameScene::updateLastOutCards(Ref* _data){
	auto _lastOutCard = dynamic_cast<OutCards*>(_data);
	CC_ASSERT(_lastOutCard != nullptr);
	lastOutCard = _lastOutCard;
	lastOutCard->retain();
}

void GameScene::setGameOver(Ref*){
	this->gameOver();
}

void GameScene::setGameStateChooseLandlord(Ref*){
	this->setGameState(CHOOSELANDLORD);
}

void GameScene::playOutCardInSceneMusic(Ref*){
	this->outCardInSceneMusic();
}

void GameScene::ready(){
	player->setReady(true);		/* 显示开始按钮 */
	computerPlayer_one->setReady(true);	/* 电脑端玩家1准备好 */
	computerPlayer_two->setReady(true);	/* 电脑端玩家2准备好 */

	if (this->checkAllReady()){
		/* 所有人准备好后，将命令窗口隐藏 */
		this->setPlayerOrderStateUnVisible();
		MusicController::getInstance()->playStartMusic();
		player->setReady(false);	/* 隐藏开始按钮 */
		this->setGameState(DEAL);	/* 所有人准备好后，切换游戏状态 */
	}
}

bool GameScene::checkAllReady(){
	return player->isReady() == true
		&& computerPlayer_one->isReady() == true
		&& computerPlayer_two->isReady() == true;
}

void GameScene::dealCard(){
	Vector<Poker*> _pokers;

	for (int i = 0; i < TOTAL_POKER_NUM - 3; i += 3){
		_pokers.pushBack(pokers.at(i));
	}

	player->insertCards(_pokers);/* 对玩家发牌 */
	player->updateCardPos();	/* 初始化时对玩家的牌的位置进行更新，防止只显示一张牌 */

	_pokers.clear();
	for (int i = 1; i < TOTAL_POKER_NUM - 3; i += 3){
		_pokers.pushBack(pokers.at(i));
	}
	computerPlayer_one->insertCards(_pokers);

	_pokers.clear();
	for (int i = 2; i < TOTAL_POKER_NUM - 3; i += 3){
		_pokers.pushBack(pokers.at(i));
	}
	computerPlayer_two->insertCards(_pokers);
}

void GameScene::initLandlordCard(){
	for (int i = TOTAL_POKER_NUM - 3; i < TOTAL_POKER_NUM; ++i){
		cardForLandlord.pushBack(pokers.at(i));
	}
}

void GameScene::displayLandlordCard(){
	CC_ASSERT(cardForLandlord.size() > 0);
	/* 复制属于地主的三张牌 */
	for (int i = 0; i < cardForLandlord.size(); ++i){
		cardDisplayInTop.pushBack(cardForLandlord.at(i)->clone());
	}

	const float SCALE = 0.7;
	/* 卡牌是无序的，需要先排序 */
	GlobalFunc::sort(cardDisplayInTop);
	/* 显示卡牌 */
	float cardWidth = POKER_WIDTH * SCALE;
	float cardHeight = POKER_HEIGHT * SCALE;
	float interval = cardWidth - 10;// -MIMIUM_CARDS_OVERLAPWIDTH;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float startPosX = visibleSize.width / 2 - 1.5 * interval;
	float cardPosY = visibleSize.height - DISPLAYCARDINTOP_INTERVAL - cardHeight / 2;
	for (int i = 0; i < cardDisplayInTop.size(); ++i){
		cardDisplayInTop.at(i)->setScale(SCALE);
		cardDisplayInTop.at(i)->setPosition(Point(startPosX + cardWidth / 2 + i * interval, cardPosY));
		cardDisplayInTop.at(i)->showFront();
		this->addChild(cardDisplayInTop.at(i));
	}
}

void GameScene::setPlayerOrderStateUnVisible(){
	/* 隐藏所有命令状态 */
	player->setOrderVisible(false);
	computerPlayer_one->setOrderVisible(false);
	computerPlayer_two->setOrderVisible(false);
}

void GameScene::initOutCardOrder(){
	if (landlordPlayer == player) this->outcardOrder = 0;
	if (landlordPlayer == computerPlayer_one) this->outcardOrder = 1;
	if (landlordPlayer == computerPlayer_two) this->outcardOrder = 2;
}

void GameScene::initPlayerVector(){
	/* 如果是地主就插入到Vector首位，这样就不需考虑顺序问题了，只要按照1，2，3的顺序来出牌即可 */
	this->players.clear();	/* 防止因代码不严谨导致players不为空 */
	this->players.pushBack(player);
	this->players.pushBack(computerPlayer_one);
	this->players.pushBack(computerPlayer_two);

	/*this->players.eraseObject(landlordPlayer);
	this->players.insert(0, landlordPlayer);*/
}

void GameScene::initLastOutCard(){
	CC_ASSERT(landlordPlayer != nullptr);
	/* 初始化outCards */
	this->lastOutCard = OutCards::create(this->players.at(0), NONE, 0, nullptr);
	lastOutCard->retain();
}

void GameScene::outCardInOrder(float delta){
	this->setPlayerOrderStateUnVisible();	/* 将所有的玩家的状态隐藏 */

	auto _player = this->players.at(outcardOrder);	/* 这里利用了多态 */
	_player->outCard(lastOutCard);
}

void GameScene::outCardInSceneMusic(){
	auto _musicController = MusicController::getInstance();
	/* 分析所出的扑克类型 */
	PokerValueType _pokerValueType = lastOutCard->getPokerValueType();//GameRules::getInstance()->analysePokerValueType(cardsInScene);
	if (_pokerValueType == SINGLE){
		/* 先判断是不是大小王，如果是，播放对应的音效后退出 */
		if (lastOutCard->getLowestPoker()->getPokerType() == BLACKJOKER){
			_musicController->playOutCardEffect(BLACKKING);
			return;
		}else if(lastOutCard->getLowestPoker()->getPokerType() == REDJOKER){
			_musicController->playOutCardEffect(REDKING);
			return;
		}

		auto _pokerValue = lastOutCard->getLowestPoker()->getValue();
		/* 单张音乐 */
		switch (_pokerValue){
		case 1: _musicController->playOutCardEffect(A); break;
		case 2:_musicController->playOutCardEffect(TWO); break;
		case 3:_musicController->playOutCardEffect(THREE); break;
		case 4:_musicController->playOutCardEffect(FOUR); break;
		case 5:_musicController->playOutCardEffect(FIVE); break;
		case 6:_musicController->playOutCardEffect(SIX); break;
		case 7:_musicController->playOutCardEffect(SEVEN); break;
		case 8:_musicController->playOutCardEffect(EIGHT); break;
		case 9:_musicController->playOutCardEffect(NINE); break;
		case 10:_musicController->playOutCardEffect(TEN); break;
		case 11:_musicController->playOutCardEffect(J); break;
		case 12:_musicController->playOutCardEffect(Q); break;
		case 13:_musicController->playOutCardEffect(K); break;
		default:break;
		}
		return;
	}
	else if(_pokerValueType == PAIR){
		auto _pokerValue = lastOutCard->getLowestPoker()->getValue();
		/* 单张音乐 */
		switch (_pokerValue){
		case 1: _musicController->playOutCardEffect(AA); break;
		case 2:_musicController->playOutCardEffect(PAIRTWO); break;
		case 3:_musicController->playOutCardEffect(PAIRTHREE); break;
		case 4:_musicController->playOutCardEffect(PAIRFOUR); break;
		case 5:_musicController->playOutCardEffect(PAIRFIVE); break;
		case 6:_musicController->playOutCardEffect(PAIRSIX); break;
		case 7:_musicController->playOutCardEffect(PAIRSEVEN); break;
		case 8:_musicController->playOutCardEffect(PAIREIGHT); break;
		case 9:_musicController->playOutCardEffect(PAIRNINE); break;
		case 10:_musicController->playOutCardEffect(PAIRTEN); break;
		case 11:_musicController->playOutCardEffect(JJ); break;
		case 12:_musicController->playOutCardEffect(QQ); break;
		case 13:_musicController->playOutCardEffect(KK); break;
		default:break;
		}
		return;
	}else if (_pokerValueType == PAIRSRAIGHT){
		MusicController::getInstance()->playOutCardEffect(PAIRSTRAIGHT_EFFECT);
	}else if (_pokerValueType == TRIPLE){
		MusicController::getInstance()->playOutCardEffect(TRIPLE_EFFECT);
	}else if (_pokerValueType == TRIPLESTRAIGHT){
		MusicController::getInstance()->playOutCardEffect(TRIPLESTRAIGHT_EFFECT);
	}else if (_pokerValueType == BOMB || _pokerValueType == KINGBOMB){
		MusicController::getInstance()->playOutCardEffect(BOMB_EFFECT);
	}
}

void GameScene::runWinAnimation(){
	if (winSprite == nullptr){
		winSprite = Sprite::create();
		winSprite->setPosition(Director::getInstance()->getVisibleSize() / 2);
		this->addChild(winSprite);
	}
	auto fadeOut = FadeOut::create(2);
	winSprite->setVisible(true);
	winSprite->runAction(Sequence::create(GameAnimation::getInstance()->getWinAnimation(), fadeOut, nullptr));
}

void GameScene::runLostAnimation(){
	if (lostSprite == nullptr){
		lostSprite = Sprite::create();
		lostSprite->setPosition(Director::getInstance()->getVisibleSize() / 2);
		this->addChild(lostSprite);
	}
	auto fadeOut = FadeOut::create(2);
	lostSprite->runAction(Sequence::create(GameAnimation::getInstance()->getLostAnimation(), fadeOut, nullptr));
}

void GameScene::deleteCardInTop(){
	for (auto it : cardDisplayInTop){
		this->removeChild(it);
	}
	cardDisplayInTop.clear(); 
}

void GameScene::gameStart(float delta){
	this->gameStart();
}

void GameScene::gameStart(){
	initPoker();	/* 卡牌初始化 */
	initPlayer();	/* 初始化多个玩家 */
	initCallLandlord();	/* 初始化叫地主 */

	this->setGameState(READY);

	/* 播放背景音乐，循环播放 */
	MusicController::getInstance()->playBackgroundMusic();

	this->schedule(schedule_selector(GameScene::update), 2.0f);
}

bool GameScene::isWin(){
	/* 首先查找是谁出完牌了 */
	Player* _winnerPlayer = nullptr;
	CC_ASSERT(players.size() == 3);
	for (int i = 0; i < players.size(); ++i){
		if (players.at(i)->getPoker().size() == 0){
			_winnerPlayer = players.at(i);
			break;
		}
	}
	CC_ASSERT(_winnerPlayer != nullptr);	/* 如果没找到出完牌的，说明游戏逻辑出问题了 */
	/* 胜利的人和玩家是否是相同的类型（农民或者地主） */
	return _winnerPlayer->getLandlord() == this->player->getLandlord();
}

void GameScene::gameOver(){
	if (isWin() == true){
		/* 播放胜利的音乐 */
		MusicController::getInstance()->playWinMusic();
		this->runWinAnimation();
	}else{
		/* 播放失败的音乐 */
		MusicController::getInstance()->playLostMusic();
		this->runLostAnimation();
	}
	deletePlayer();
	deleteCardInTop();	/* 清空在顶部的地主扑克 */
	this->scheduleOnce(schedule_selector(GameScene::gameStart), 5.0f);
	//Director::getInstance()->replaceScene(Lobby::createScene());
}