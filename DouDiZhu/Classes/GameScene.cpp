#include <algorithm>
#include <ctime>
#include "GameAnimation.h"
#include "GameRules.h"
#include "GameScene.h"
#include "GlobalFunc.h"
#include "HeadImage.h"
#include "OutCards.h"
#include "Player.h"
#include "Poker.h"
#include "PokerController.h"

using namespace std;

GameScene::GameScene(){
	gameState = DEAL;	/* 初始状态是发牌 */
	winSprite = nullptr;
	lostSprite = nullptr;
}

GameScene::~GameScene(){
}

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

	bool tRet = false;
	do {
		CC_BREAK_IF(!initBackground());	/* 初始化背景 */
		//CC_BREAK_IF(!initPoker());		/* 创建扑克 */
		//CC_BREAK_IF(!shuffleCards());		/* 洗牌 */
		//CC_BREAK_IF(!initPlayer());		/* 初始化玩家，包括电脑 */
		//CC_BREAK_IF(!initHeadImage());
		CC_BREAK_IF(!initButton());	/* 初始化按钮 */

		NotificationCenter::getInstance()->addObserver(this,
			callfuncO_selector(GameScene::updatePokerPosAndRemovePoker),
			"UpdatePokerPosAndRemovePoker", nullptr);
		//this->schedule(schedule_selector(GameScene::update));
		this->scheduleOnce(schedule_selector(GameScene::gameStart), 0.0f);
		tRet = true;
	} while (0);

	return tRet;
}

void GameScene::update(float delta){
	static float deltaCount = 0;
	deltaCount += delta;
	if (deltaCount < 3.0f && gameState == OUTCARD){
		return;
	}
	deltaCount = 0;
	switch (gameState){
	/* 发好牌后，初始化出牌，并转入出牌模式 */
	case DEAL: dealCard(); initOutCard(); gameState = OUTCARD; break;
	case OUTCARD: outCardInOrder(delta); break;
	case END: this->unschedule(schedule_selector(GameScene::update)); this->gameOver(); break;
	default: break;
	}
}

bool GameScene::initBackground(){
	bool tRet = false;
	do {
		auto bgSprite = Sprite::create("Image/background.png");
		bgSprite->setPosition(Point(Director::getInstance()->getVisibleSize() / 2));
		this->addChild(bgSprite, 0);

		tRet = true;
	} while (0);

	return tRet;
}

bool GameScene::initPlayer(){
	auto visbleSize = Director::getInstance()->getVisibleSize();	/* 获取屏幕的可见尺寸 */

	/* 计算电脑的位置 */
	auto computerPlayerOnePos = Point(visbleSize.width / 12, visbleSize.height * 2 / 3);
	auto computerPlayerTwoPos = Point(visbleSize.width * 11 / 12, visbleSize.height * 2 / 3);

	/* 创建玩家示例（包括电脑），设置各自的类型，并且将其设为GameScene场景的子结点，防止被回收 */
	player = Player::create();
	this->addChild(player);		/* 将玩家设置成场景的子结点，防止被回收 */
	player->setPlayerType(PLAYER);	/* 设置玩家类型 */
	computerPlayer_one = Player::create();
	this->addChild(computerPlayer_one);
	computerPlayer_one->setPlayerType(COMPUTER);
	computerPlayer_two = Player::create();
	computerPlayer_two->setPlayerType(COMPUTER); 
	this->addChild(computerPlayer_two);

	/* 设置玩家和电脑的坐标 */
	player->setPosition(visbleSize.width / 2, DISPLAYCARDHEIGHT + PokerController::getPokerSize().height / 2);
	computerPlayer_one->setPosition(computerPlayerOnePos);
	computerPlayer_two->setPosition(computerPlayerTwoPos);

	return true;
}

bool GameScene::initPoker(){
	/* 方块DIAMOND */
	for (int i = 1; i <= SINGLETYPECARDNUM; ++i){
		auto poker = Poker::create(this, DIAMOND, i);
		pokers.pushBack(poker);
	}
	/* 梅花HEART */
	for (int i = 1; i <= SINGLETYPECARDNUM; ++i){
		auto poker = Poker::create(this, CLUB, i);
		pokers.pushBack(poker);
	}
	/* 红桃HEART */
	for (int i = 1; i <= SINGLETYPECARDNUM; ++i){
		auto poker = Poker::create(this, HEART, i);
		pokers.pushBack(poker);
	}
	/* 黑桃SPADE */
	for (int i = 1; i <= SINGLETYPECARDNUM; ++i){
		auto poker = Poker::create(this, SPADE, i);
		pokers.pushBack(poker);
	}
	/* 大小王 */
	pokers.pushBack(Poker::create(this, BLACKJOKER));
	pokers.pushBack(Poker::create(this, REDJOKER));

	return true;
}

void GameScene::sort(){
	GlobalFunc::sort(arrWaitPlayOut);
}

void GameScene::updatePokerPosAndRemovePoker(Ref* data){
	Poker* poker = dynamic_cast<Poker*>(data);
	auto player = dynamic_cast<Player*>(poker->getParent());	/* 这里Poker和Player过度耦合 */
	player->removePoker(poker);
	player->updatePokerPos();
}

int GameScene::randomInt(int begin, int end){	/* 左闭右开区间 */
	if (begin > end) std::swap(begin, end);
	int ret = begin + rand() % (end - begin);
	return ret;
}

/* Knuth_Durstenfeld_Shuffle */
bool GameScene::shuffleCards(){
	srand((unsigned)time(0));	/* 开启随机种子 */
	for (int i = pokers.size(); i > 0; --i){
		pokers.swap(i - 1, randomInt(0, i));
	}
	return true;
}

bool GameScene::initButton(){
	auto visibleSize = Director::getInstance()->getVisibleSize();	/* 屏幕尺寸 */

	auto _menu = Menu::create();

	auto _pass = Sprite::create("Image/btn_pass.png");
	auto _pass_pressed = Sprite::create("Image/btn_pass_selected.png");// Sprite::createWithSpriteFrame(_pass->getSpriteFrame());	/* 利用精灵帧来复制创建一个精灵 */
	auto _pass_disabled = Sprite::create("Image/btn_pass.png");
	pass = MenuItemSprite::create(_pass, _pass_pressed, CC_CALLBACK_1(GameScene::pass_callback, this));
	//out->setCallback(CC_CALLBACK_1(GameScene::pass_callback, this)); /* 这样写会出错，为什么？*/ 

	auto _out = Sprite::create("Image/btn_out.png");
	auto _out_pressed = Sprite::create("Image/btn_out_selected.png");
	auto _out_disabled = Sprite::create("Image/btn_out_disabled.png");
	out = MenuItemSprite::create(_out, _out_pressed, CC_CALLBACK_1(GameScene::out_callback, this));
	out->setDisabledImage(_out_disabled);
	out->setEnabled(false);		/* 初始化时设置 出牌 按钮不可按下*/

	pass->setVisible(false);
	out->setVisible(false);/* 初始化时设置 两个按钮 不可见 */

	auto _pass_pos = Point(visibleSize.width / 2 - _pass->getContentSize().width / 2 - 10, POKER_HEIGHT + DISPLAYCARDHEIGHT + _pass->getContentSize().height / 2 + 15);
	auto _out_pos = Point(visibleSize.width / 2 + _pass->getContentSize().width / 2 + 10, POKER_HEIGHT + DISPLAYCARDHEIGHT + out->getContentSize().height / 2 + 15);
	pass->setPosition(_pass_pos);
	out->setPosition(_out_pos);
	_menu->setPosition(Point(0, 0));

	_menu->addChild(pass);
	_menu->addChild(out);
	this->addChild(_menu);	/* MenuItemSprite对象必须存放在Menu中才能正常使用 */

	return true;
}

bool GameScene::initHeadImage(){
	auto playerPos = player->getPosition();
	auto computerOne_pos = computerPlayer_one->getPosition();
	auto computerTwo_pos = computerPlayer_two->getPosition();
	auto cardSize = PokerController::getInstance()->getPokerSize();
	auto headBoxSize = HeadImage::create()->getHeadBoxSize();	/* 获取头像框的大小 */

	auto playerHeadImagePos = Point(computerOne_pos.x, playerPos.y + cardSize.height / 2 + headBoxSize.height / 2 + HEIGHTDISTANCE_HEADIMAGEANDPLAYER);
	auto computerOneHeadImagePos = Point(computerOne_pos.x, computerOne_pos.y + cardSize.height / 2 + headBoxSize.height / 2 + HEIGHTDISTANCE_HEADIMAGEANDPLAYER);
	auto computerTwoHeadImagePos = Point(computerTwo_pos.x, computerTwo_pos.y + cardSize.height / 2 + headBoxSize.height / 2 + HEIGHTDISTANCE_HEADIMAGEANDPLAYER);

	playerHeadImage = HeadImage::create();		/* 玩家头像 */
	playerHeadImage->setPosition(playerHeadImagePos);
	this->addChild(playerHeadImage);
	//playerHeadImage->setHeadImageType(LANDLORD, RIGHT);
	computerPlayer_one_headImage = HeadImage::create();	/* 电脑端头像 */
	computerPlayer_one_headImage->setPosition(computerOneHeadImagePos);
	this->addChild(computerPlayer_one_headImage);
	//computerPlayer_one_headImage->setHeadImageType(FARMER, RIGHT);
	computerPlayer_two_headImage = HeadImage::create();	/* 电脑端头像 */
	computerPlayer_two_headImage->setPosition(computerTwoHeadImagePos);
	this->addChild(computerPlayer_two_headImage);
	//computerPlayer_two_headImage->setHeadImageType(FARMER, LEFT);

	return true;
}

void GameScene::dealCard(Player* _player, Vector<Poker*>& _pokers, bool displayFront /* = false */){
	GlobalFunc::sort(_pokers);
	_player->setPoker(_pokers);	/* 这里必须将扑克进行排序后再添加，不然会因为添加顺序的原因导致覆盖的顺序不对 */

	bool isCanClick = displayFront;
	for (auto it : _pokers){
		_player->addChild(it);
		it->setCanClick(isCanClick);
		if(displayFront == true) it->showFront();
		else it->showBack();
	}
}

void GameScene::dealCard(){
	Vector<Poker*> _pokers;

	for (int i = 0; i < TOTAL_POKER_NUM - 3; i += 3){
		_pokers.pushBack(pokers.at(i));
	}

	for (int i = TOTAL_POKER_NUM - 3; i < TOTAL_POKER_NUM; ++i){
		_pokers.pushBack(pokers.at(i));
	}
	dealCard(player, _pokers, true);		/* 对玩家发牌 */
	player->updatePokerPos();	/* 初始化时对玩家的牌的位置进行更新，防止只显示一张牌 */

	_pokers.clear();
	for (int i = 1; i < TOTAL_POKER_NUM - 3; i += 3){
		_pokers.pushBack(pokers.at(i));
	}
	dealCard(computerPlayer_one, _pokers);	/* 对电脑1发牌 */ 

	_pokers.clear();
	for (int i = 2; i < TOTAL_POKER_NUM - 3; i += 3){
		_pokers.pushBack(pokers.at(i));
	}
	dealCard(computerPlayer_two, _pokers);
}

bool GameScene::isCurAndManualPlayer() const { 
	return this->player == lastOutCards->getPokerOwner(); 
}

void GameScene::updateOutState(){
	/* 如果当前出牌玩家不是player，那么不需要更新出牌按钮的可按性 */
	if (players.at(order % 3) != player) return;
	if (lastOutCards->getPokerValueType() == NONE || lastOutCards->getPokerOwner() == player){
		/* 如果上一次的出牌是NONE（表示刚开始）或者 上一次出牌的持有者还是player，
			那么判断当前待出的牌是不是正确的牌型，如果是，就令出牌按钮可按，否则不可按 */
		if (GameRules::getInstance()->isPokerValueType(arrWaitPlayOut) == true){
			out->setEnabled(true);
		}else{
			out->setEnabled(false);
		}
		return;
	} else{
		PokerValueType _pokerValueType = GameRules::getInstance()->analysePokerValueType(arrWaitPlayOut);
		if (_pokerValueType == NONE){	/* 如果当前不是任何牌型 */
			out->setEnabled(false);
		} else{
			out->setEnabled(GameRules::getInstance()->canOutCards(arrWaitPlayOut, lastOutCards));
		}
	}
}

void GameScene::pass_callback(Ref*){
	pass->setVisible(false);
	out->setVisible(false);
	out->setEnabled(false);	/* 每次出牌或者pass后，将out按钮设为不可按 */

	this->order = (this->order + 1) % 3;
}

void GameScene::out_callback(Ref*){
	pass->setVisible(false);
	out->setVisible(false);
	out->setEnabled(false);	/* 每次出牌或者pass后，将out按钮设为不可按 */

	lastOutCards = OutCards::create(player, GameRules::getInstance()->analysePokerValueType(arrWaitPlayOut),
		arrWaitPlayOut.size(), arrWaitPlayOut.at(arrWaitPlayOut.size() - 1));
	lastOutCards->retain();		/* 防止被内存管理器回收 */

	deleteCardInScene();		/* 在将cardInScene存放新的扑克时，先将以前的在Scene的扑克删除 */

	cardsInScene = arrWaitPlayOut;	/* 将出的牌放到出牌的容器里，待在出牌区域显示 */

	for (int i = 0; i < arrWaitPlayOut.size(); ++i){
		arrWaitPlayOut.at(i)->removeFromParent();
		player->removePoker(arrWaitPlayOut.at(i));
		player->updatePokerPos();
	}

	arrWaitPlayOut.clear();	/* 待出的牌出了，就要将待出牌容器清空，不然和下次的重复在一起会导致错误 */

	outCardInScene();

	/* 如果玩家已经出完牌，则获胜 */
	if (player->getPoker().size() == 0){
		this->gameState = END;
		return;
	}

	this->order = (this->order + 1) % 3;
}

void GameScene::initOutCard(){
	this->order = 0;
	/* 如果是地主就插入到Vector首位，这样就不需考虑顺序问题了，只要按照1，2，3的顺序来出牌即可 */
	if (player->getLandlord() == true){
		this->players.insert(0, player);
	}else{
		this->players.pushBack(player);
	}

	if (computerPlayer_one->getLandlord() == true){
		this->players.insert(0, computerPlayer_one);
	}else{
		this->players.pushBack(computerPlayer_one);
	}

	if (computerPlayer_two->getLandlord() == true){
		this->players.insert(0, computerPlayer_two);
	}else{
		this->players.pushBack(computerPlayer_two);
	}
	/* 初始化outCards */
	this->lastOutCards = OutCards::create(this->players.at(0), NONE, 0, nullptr);
	lastOutCards->retain();
}

void GameScene::outCardForPlayer(Player* _player){
	/* 让不出和出牌按钮显示出来，出牌按钮不可按由出牌和不出按钮的触发事件控制 */
	pass->setVisible(true);
	out->setVisible(true);
	/* 轮到玩家出牌时，玩家可能已经准备好要出的牌，因此轮到玩家出牌时，需要对此做一次更新 */
	this->updateOutState();
}

void GameScene::outCardForComputer(Player* _computer){
  	Vector<Poker*> _pokers;
	                                                                                /* 如果上一手牌也是自己的 */
	if (lastOutCards->getPokerOwner() == _computer || lastOutCards->getPokerValueType() == NONE){
		//_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), SINGLE, nullptr);	/* 这样写会导致电脑在找不到单张后一直卡在这个地方 */ 
		_pokers = GameRules::getInstance()->searchProperPokers(_computer->getPoker());
	} else{
		PokerValueType _pokerValueType = lastOutCards->getPokerValueType();
		if (_pokerValueType != KINGBOMB){
			if (_pokerValueType == BOMB){
				_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), BOMB, lastOutCards->getLowestPoker());
			}else{
				_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), lastOutCards->getPokerValueType(), lastOutCards->getLowestPoker(), lastOutCards->getTotalLength());
				if (_pokers.size() == 0){ /* 如果找不到对应的牌，就找炸弹 */
					_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), BOMB, nullptr);
					if (_pokers.size() == 0){	/* 如果找不到普通的炸，就找王炸 */
						_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), KINGBOMB);
					}
				}
			}
		}
	}
	if (_pokers.size() != 0){
		lastOutCards = OutCards::create(_computer, GameRules::getInstance()->analysePokerValueType(_pokers), _pokers.size(), _pokers.at(_pokers.size() - 1));
		lastOutCards->retain();
	}

	for (int i = 0; i < _pokers.size(); ++i){
		_pokers.at(i)->removeFromParent();
		_computer->removePoker(_pokers.at(i));
	}

	if (_pokers.size() != 0){
		deleteCardInScene();
		cardsInScene = _pokers;	/* 如果电脑出牌了，那么将出的牌放入待显示在Scene中的容器 */
		outCardInScene();
	}
	 
	if (_computer->getPoker().size() == 0){
		this->gameState = END;
		return;
	}

	this->order = (this->order + 1) % 3;
}

void GameScene::outCardInOrder(float delta){
	switch (order){
	case 0: outCardForPlayer(player); break;
	case 1: outCardForComputer(computerPlayer_one); break;
	case 2: outCardForComputer(computerPlayer_two); break;
	default:break;
	}
}

/* 出的牌放在高度和电脑的牌高度一致，最大宽度是1/2屏幕宽度 */
/* 出的牌放在一个容器里，然后放在这个里面 */
void GameScene::outCardInScene(){
	if (cardsInScene.size() == 0) return;

	int _height = computerPlayer_one->getPosition().y;
	float _middleWidth = Director::getInstance()->getVisibleSize().width / 2;
	float _maxWidth = Director::getInstance()->getVisibleSize().width / 2;

	int _cardsNum = cardsInScene.size();	/* 卡牌数量 */
	float _cardWidth = cardsInScene.at(0)->getContentSize().width;	/* 卡牌宽度 */
	float interval = (_maxWidth - _cardWidth) < (_cardWidth - MIMIUM_CARDS_OVERLAPWIDTH) * (_cardsNum - 1) ? (_maxWidth - _cardWidth) / (_cardsNum - 1) : (_cardWidth - MIMIUM_CARDS_OVERLAPWIDTH);

	float startPosX = _middleWidth - _maxWidth / 2; /* 如果所有出的牌的宽度加起来大于能够显示的区域，计算出牌的开始位置 */
	if (interval == (_cardWidth - MIMIUM_CARDS_OVERLAPWIDTH)){
		startPosX = _cardsNum % 2 == 0 ?
			_middleWidth - (_cardsNum / 2) *  interval :
			_middleWidth - (_cardsNum / 2 + 0.5) * interval;
	}

	for (int i = 0; i < cardsInScene.size(); ++i){
		this->addChild(cardsInScene.at(i));
		cardsInScene.at(i)->setPosition(startPosX + _cardWidth / 2 + interval * i, _height);
		cardsInScene.at(i)->showFront();
		/* 出的牌令其canClick属性设置为false，使其不可点击 */
		cardsInScene.at(i)->setCanClick(false);
	}
}

void GameScene::deleteCardInScene(){
	for (int i = 0; i < cardsInScene.size(); ++i){
		cardsInScene.at(i)->removeFromParent();
	}
	cardsInScene.clear();
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

void GameScene::gameStart(float delta){
	initPoker();	/* 卡牌初始化 */
	shuffleCards();	/* 洗牌 */
	initPlayer();	/* 初始化多个玩家 */
	initHeadImage();	/* 初始化多个玩家的头像 */

	this->gameState = DEAL;
	this->schedule(schedule_selector(GameScene::update), 2.0f);
}

void GameScene::gameOver(){
	/* 后面增加代码 */
	if (player->getPoker().size() == 0){
		log("You Win!");
		this->runWinAnimation();
	}else{
		log("You Lose!");
		this->runLostAnimation();
	}

	/* 游戏结束后，一些资源处理操作 */
	pokers.clear();	/* 卡牌置空 */
	/* 删除玩家和电脑的所有卡牌 */
	player->removeAllPoker();
	computerPlayer_one->removeAllPoker();
	computerPlayer_two->removeAllPoker();
	/* 删除玩家和电脑端的指针 */
	this->removeChild(player);
	this->removeChild(computerPlayer_one);
	this->removeChild(computerPlayer_two);
	players.clear();	/* 删除玩家容器里的所有玩家指针，如果没有这一步，
						新的一局开始时，玩家无法出牌*/
	this->removeChild(playerHeadImage);
	this->removeChild(computerPlayer_one_headImage);
	this->removeChild(computerPlayer_two_headImage);

	deleteCardInScene();	/* 删除在Scene的扑克 */

	this->scheduleOnce(schedule_selector(GameScene::gameStart), 10.0f);
}

void GameScene::test(){
	for (int i = 0; i < player->getPoker().size(); ++i){
		auto _poker = player->getPoker().at(i);
		log("%d %d", _poker->getPokerType(), _poker->getValue());
	}
	log("print poker end!");

	auto gameRules = GameRules::getInstance();
	auto poker = Poker::create(this, SPADE, 3);
	auto ret = gameRules->calcPokerWithValueType(player->getPoker(), TRIPLESTRAIGHT, GlobalFunc::getGreaterPoker(poker, 0), 2);
	if (ret.size() == 0){
		log("Not Find!!!");
		return;
	}
	for (int i = 0; i < ret.size(); ++i){
		log("%d : %d", ret.at(i)->getPokerType(), ret.at(i)->getValue());
	}

	for (int i = 0; i < ret.size(); ++i){
		auto _poker = ret.at(i);
		NotificationCenter::getInstance()->postNotification("UpdatePokerPosAndRemovePoker", _poker);
	}
}