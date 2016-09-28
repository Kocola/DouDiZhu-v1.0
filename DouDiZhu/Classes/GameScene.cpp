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
	gameState = DEAL;	/* ��ʼ״̬�Ƿ��� */
	winSprite = nullptr;
	lostSprite = nullptr;
	landlordPlayer = nullptr;
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
		CC_BREAK_IF(!initBackground());	/* ��ʼ������ */
		//CC_BREAK_IF(!initPoker());		/* �����˿� */
		//CC_BREAK_IF(!shuffleCards());		/* ϴ�� */
		//CC_BREAK_IF(!initPlayer());		/* ��ʼ����ң��������� */
		//CC_BREAK_IF(!initHeadImage());
		CC_BREAK_IF(!initButton());	/* ��ʼ����ť */

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
	/* �����ƺ󣬳�ʼ�����ƣ���ת�����ģʽ */
	case DEAL: dealCard(); initLandlordCard(); displayLandlordCard(); gameState = CALLLANDLORD; break;
	case CALLLANDLORD: callLandlord(); break;
	case CHOOSELANDLORD: chooseLandlord(); updateHeadImage();  outCardForLandlord(); initOutCardOrder(); gameState = OUTCARD; break;
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
	auto visbleSize = Director::getInstance()->getVisibleSize();	/* ��ȡ��Ļ�Ŀɼ��ߴ� */

	/* ������Ե�λ�� */
	auto computerPlayerOnePos = Point(visbleSize.width / 12, visbleSize.height * 2 / 3);
	auto computerPlayerTwoPos = Point(visbleSize.width * 11 / 12, visbleSize.height * 2 / 3);

	/* �������ʾ�����������ԣ������ø��Ե����ͣ����ҽ�����ΪGameScene�������ӽ�㣬��ֹ������ */
	player = Player::create();
	this->addChild(player);		/* ��������óɳ������ӽ�㣬��ֹ������ */
	player->setPlayerType(PLAYER);	/* ����������� */
	computerPlayer_one = Player::create();
	this->addChild(computerPlayer_one);
	computerPlayer_one->setPlayerType(COMPUTER);
	computerPlayer_two = Player::create();
	computerPlayer_two->setPlayerType(COMPUTER); 
	this->addChild(computerPlayer_two);

	/* ������Һ͵��Ե����� */
	player->setPosition(visbleSize.width / 2, DISPLAYCARDHEIGHT + PokerController::getPokerSize().height / 2);
	computerPlayer_one->setPosition(computerPlayerOnePos);
	computerPlayer_two->setPosition(computerPlayerTwoPos);

	return true;
}

bool GameScene::initPoker(){
	/* ����DIAMOND */
	for (int i = 1; i <= SINGLETYPECARDNUM; ++i){
		auto poker = Poker::create(this, DIAMOND, i);
		pokers.pushBack(poker);
	}
	/* ÷��HEART */
	for (int i = 1; i <= SINGLETYPECARDNUM; ++i){
		auto poker = Poker::create(this, CLUB, i);
		pokers.pushBack(poker);
	}
	/* ����HEART */
	for (int i = 1; i <= SINGLETYPECARDNUM; ++i){
		auto poker = Poker::create(this, HEART, i);
		pokers.pushBack(poker);
	}
	/* ����SPADE */
	for (int i = 1; i <= SINGLETYPECARDNUM; ++i){
		auto poker = Poker::create(this, SPADE, i);
		pokers.pushBack(poker);
	}
	/* ��С�� */
	pokers.pushBack(Poker::create(this, BLACKJOKER));
	pokers.pushBack(Poker::create(this, REDJOKER));

	return true;
}

void GameScene::sort(){
	GlobalFunc::sort(arrWaitPlayOut);
}

void GameScene::updatePokerPosAndRemovePoker(Ref* data){
	Poker* poker = dynamic_cast<Poker*>(data);
	auto player = dynamic_cast<Player*>(poker->getParent());	/* ����Poker��Player������� */
	player->removePoker(poker);
	player->updatePokerPos();
}

int GameScene::randomInt(int begin, int end){	/* ����ҿ����� */
	if (begin > end) std::swap(begin, end);
	int ret = begin + rand() % (end - begin);
	return ret;
}

/* Knuth_Durstenfeld_Shuffle */
bool GameScene::shuffleCards(){
	srand((unsigned)time(0));	/* ����������� */
	for (int i = pokers.size(); i > 0; --i){
		pokers.swap(i - 1, randomInt(0, i));
	}
	return true;
}

bool GameScene::initButton(){
	auto visibleSize = Director::getInstance()->getVisibleSize();	/* ��Ļ�ߴ� */

	auto _menu = Menu::create();
	_menu->setPosition(Point(0, 0));	/* Menu����ʱ��Ҫ����λ��Ĭ�ϱ�Ϊ0 */

	auto _pass = Sprite::create("Image/btn_pass.png");
	auto _pass_pressed = Sprite::create("Image/btn_pass_selected.png");// Sprite::createWithSpriteFrame(_pass->getSpriteFrame());	/* ���þ���֡�����ƴ���һ������ */
	auto _pass_disabled = Sprite::create("Image/btn_pass.png");
	pass = MenuItemSprite::create(_pass, _pass_pressed, CC_CALLBACK_1(GameScene::pass_callback, this));
	//out->setCallback(CC_CALLBACK_1(GameScene::pass_callback, this)); /* ����д�����Ϊʲô��*/ 

	auto _out = Sprite::create("Image/btn_out.png");
	auto _out_pressed = Sprite::create("Image/btn_out_selected.png");
	auto _out_disabled = Sprite::create("Image/btn_out_disabled.png");
	out = MenuItemSprite::create(_out, _out_pressed, CC_CALLBACK_1(GameScene::out_callback, this));
	out->setDisabledImage(_out_disabled);
	out->setEnabled(false);		/* ��ʼ��ʱ���� ���� ��ť���ɰ���*/

	/* ���в˵� */
	auto _nocall = Sprite::create("Image/btn_nocall.png");
	auto _nocall_pressed = Sprite::create("Image/btn_nocall_selected.png");
	nocall = MenuItemSprite::create(_nocall, _nocall_pressed, CC_CALLBACK_1(GameScene::nocall_callback, this));

	/* һ�ֲ˵� */
	auto _call_one = Sprite::create("Image/btn_one.png");
	auto _call_one_selected = Sprite::create("Image/btn_one_selected.png");
	call_one = MenuItemSprite::create(_call_one, _call_one_selected, CC_CALLBACK_1(GameScene::callone_callback, this));

	/* ���ֲ˵� */
	auto _call_two = Sprite::create("Image/btn_two.png");
	auto _call_two_selected = Sprite::create("Image/btn_two_selected.png");
	call_two = MenuItemSprite::create(_call_two, _call_two_selected, CC_CALLBACK_1(GameScene::calltwo_callback, this));

	/* ���ֲ˵� */
	auto _call_three = Sprite::create("Image/btn_three.png");
	auto _call_three_selected = Sprite::create("Image/btn_three_selected.png");
	call_three = MenuItemSprite::create(_call_three, _call_three_selected, CC_CALLBACK_1(GameScene::callthree_callback, this));

	pass->setVisible(false);
	out->setVisible(false);/* ��ʼ��ʱ���� ������ť ���ɼ� */

	/* ���ð�ťλ�� */
	auto _pass_pos = Point(visibleSize.width / 2 - _pass->getContentSize().width / 2 - 10, POKER_HEIGHT + DISPLAYCARDHEIGHT + pass->getContentSize().height / 2 + 15);
	auto _out_pos = Point(visibleSize.width / 2 + _pass->getContentSize().width / 2 + 10, POKER_HEIGHT + DISPLAYCARDHEIGHT + out->getContentSize().height / 2 + 15);
	pass->setPosition(_pass_pos);
	out->setPosition(_out_pos);

	/* ��MenuItemSprite��ӵ�Menu����ʾ */
	_menu->addChild(pass);
	_menu->addChild(out);

	/* �зְ�ťȫ������λ���ɼ� */
	nocall->setVisible(false);
	call_one->setVisible(false);
	call_two->setVisible(false);
	call_three->setVisible(false);

	/* ���ð�ťλ�� */
	auto _nocall_pos = Point(visibleSize.width / 2 - call_one->getContentSize().width - BUTTON_INTERVAL - nocall->getContentSize().width / 2 - BUTTON_INTERVAL / 2, 
		POKER_HEIGHT + DISPLAYCARDHEIGHT + nocall->getContentSize().height / 2 + 15);
	nocall->setPosition(_nocall_pos);
	auto _call_one_pos = Point(visibleSize.width / 2 - call_one->getContentSize().width / 2 - BUTTON_INTERVAL / 2, 
		POKER_HEIGHT + DISPLAYCARDHEIGHT + call_one->getContentSize().height / 2 + 15);
	call_one->setPosition(_call_one_pos);
	auto _call_two_pos = Point(visibleSize.width / 2 + call_two->getContentSize().width / 2 + BUTTON_INTERVAL / 2, 
		POKER_HEIGHT + DISPLAYCARDHEIGHT + call_two->getContentSize().height / 2 + 15);
	call_two->setPosition(_call_two_pos);
	auto _call_three_pos = Point(visibleSize.width / 2 + BUTTON_INTERVAL / 2 + call_two->getContentSize().width + BUTTON_INTERVAL + call_three->getContentSize().width / 2, 
		POKER_HEIGHT + DISPLAYCARDHEIGHT + call_three->getContentSize().height / 2 + 15);
	call_three->setPosition(_call_three_pos);

	/* ��MenuItemSprite��ӵ�Menu����ʾ */
	_menu->addChild(nocall);
	_menu->addChild(call_one);
	_menu->addChild(call_two);
	_menu->addChild(call_three);

	this->addChild(_menu);	/* MenuItemSprite�����������Menu�в�������ʹ�� */

	return true;
}

bool GameScene::initHeadImage(){
	auto playerPos = player->getPosition();
	auto computerOne_pos = computerPlayer_one->getPosition();
	auto computerTwo_pos = computerPlayer_two->getPosition();
	auto cardSize = PokerController::getInstance()->getPokerSize();
	auto headBoxSize = HeadImage::create()->getHeadBoxSize();	/* ��ȡͷ���Ĵ�С */

	auto playerHeadImagePos = Point(computerOne_pos.x, playerPos.y + cardSize.height / 2 + headBoxSize.height / 2 + HEIGHTDISTANCE_HEADIMAGEANDPLAYER);
	auto computerOneHeadImagePos = Point(computerOne_pos.x, computerOne_pos.y + cardSize.height / 2 + headBoxSize.height / 2 + HEIGHTDISTANCE_HEADIMAGEANDPLAYER);
	auto computerTwoHeadImagePos = Point(computerTwo_pos.x, computerTwo_pos.y + cardSize.height / 2 + headBoxSize.height / 2 + HEIGHTDISTANCE_HEADIMAGEANDPLAYER);

	playerHeadImage = HeadImage::create();		/* ���ͷ�� */
	playerHeadImage->setPosition(playerHeadImagePos);
	this->addChild(playerHeadImage);
	//playerHeadImage->setHeadImageType(LANDLORD, RIGHT);
	computerPlayer_one_headImage = HeadImage::create();	/* ���Զ�ͷ�� */
	computerPlayer_one_headImage->setPosition(computerOneHeadImagePos);
	this->addChild(computerPlayer_one_headImage);
	//computerPlayer_one_headImage->setHeadImageType(FARMER, RIGHT);
	computerPlayer_two_headImage = HeadImage::create();	/* ���Զ�ͷ�� */
	computerPlayer_two_headImage->setPosition(computerTwoHeadImagePos);
	this->addChild(computerPlayer_two_headImage);
	//computerPlayer_two_headImage->setHeadImageType(FARMER, LEFT);

	return true;
}

bool GameScene::initCallLandlord(){
	callLandlordOrder = 0;

	return true;
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
	/* ���÷�����ߵ����ǵ����������û�нУ���ô�������ֶ���ҳ�Ϊ���� */
	landlordPlayer = landlordPlayer == nullptr ? player : landlordPlayer;
	landlordPlayer->setLandlord(true);
}

void GameScene::playerCallLandlord(){
	/* ��ʾ���нзְ�ť */
	nocall->setVisible(true);
	call_one->setVisible(true);
	call_two->setVisible(true);
	call_three->setVisible(true);
	/* �ȴ��з� */
}

void GameScene::computerCallLandlord(Player* _computer){
	log("Call Landlord!!!");

	int score = automaticCallLandlord();
	_computer->setCallLandlordScore(score);
	isMaxCallLandlordScore(_computer);
	updateCallLandlordOrder();	/* ���½���һ���е�����ID */
}

void GameScene::callLandlord(){
	switch (callLandlordOrder){
	case 0: playerCallLandlord(); break;
	case 1: computerCallLandlord(computerPlayer_one);
	case 2: computerCallLandlord(computerPlayer_two);
	default:  this->gameState = CHOOSELANDLORD; break;
	}
}

int GameScene::automaticCallLandlord(){
	srand((unsigned)time(0));	/* ����������� */
	int score = rand() % 4;	/* �з�һ���в��У�1��2��3�������������0��ʾ���� */
	return score;
}

void GameScene::updateCallLandlordOrder(){ 
	this->callLandlordOrder = this->callLandlordOrder + 1; 
}

void GameScene::isMaxCallLandlordScore(Player* _player){
	/* ������������߽з֣���ôֱ�ӽ�����������׶� */
	if (_player->getCallLandlordScore() == 3){
		this->gameState = CHOOSELANDLORD;
	}
}

void GameScene::dealCard(){
	Vector<Poker*> _pokers;

	for (int i = 0; i < TOTAL_POKER_NUM - 3; i += 3){
		_pokers.pushBack(pokers.at(i));
	}

	/* �������ڵ����������� */
	/*for (int i = TOTAL_POKER_NUM - 3; i < TOTAL_POKER_NUM; ++i){
		_pokers.pushBack(pokers.at(i));
	}*/
	player->insertCards(_pokers);/* ����ҷ��� */
	player->updatePokerPos();	/* ��ʼ��ʱ����ҵ��Ƶ�λ�ý��и��£���ֹֻ��ʾһ���� */

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
	/* �������ڵ����������� */
	for (int i = 0; i < cardForLandlord.size(); ++i){
		cardDisplayInTop.pushBack(cardForLandlord.at(i)->clone());
	}

	/* ����������ģ���Ҫ������ */
	GlobalFunc::sort(cardDisplayInTop);
	/* ��ʾ���� */
	float cardWidth = POKER_WIDTH;
	float cardHeight = POKER_HEIGHT;
	float interval = cardWidth - MIMIUM_CARDS_OVERLAPWIDTH;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float startPosX = visibleSize.width / 2 - 1.5 * interval;
	float cardPosY = visibleSize.height - DISPLAYCARDINTOP_INTERVAL - cardHeight / 2;
	for (int i = 0; i < cardDisplayInTop.size(); ++i){
		cardDisplayInTop.at(i)->setPosition(Point(startPosX + cardWidth / 2 + i * interval, cardPosY));
		cardDisplayInTop.at(i)->showFront();
		this->addChild(cardDisplayInTop.at(i));
	}
}

bool GameScene::isCurAndManualPlayer() const { 
	return this->player == lastOutCards->getPokerOwner(); 
}

void GameScene::updateOutState(){
	/* �����ǰ������Ҳ���player����ô����Ҫ���³��ư�ť�Ŀɰ��� */
	if (players.at(outcardOrder % 3) != player) return;
	if (lastOutCards->getPokerValueType() == NONE || lastOutCards->getPokerOwner() == player){
		/* �����һ�εĳ�����NONE����ʾ�տ�ʼ������ ��һ�γ��Ƶĳ����߻���player��
			��ô�жϵ�ǰ���������ǲ�����ȷ�����ͣ�����ǣ�������ư�ť�ɰ������򲻿ɰ� */
		if (GameRules::getInstance()->isPokerValueType(arrWaitPlayOut) == true){
			out->setEnabled(true);
		}else{
			out->setEnabled(false);
		}
		return;
	} else{
		PokerValueType _pokerValueType = GameRules::getInstance()->analysePokerValueType(arrWaitPlayOut);
		if (_pokerValueType == NONE){	/* �����ǰ�����κ����� */
			out->setEnabled(false);
		} else{
			out->setEnabled(GameRules::getInstance()->canOutCards(arrWaitPlayOut, lastOutCards));
		}
	}
}

void GameScene::setCallLandlordButtonUnVisible(){
	/* �������нзְ�ť */
	nocall->setVisible(false);
	call_one->setVisible(false);
	call_two->setVisible(false);
	call_three->setVisible(false);
}

void GameScene::pass_callback(Ref*){
	pass->setVisible(false);
	out->setVisible(false);
	out->setEnabled(false);	/* ÿ�γ��ƻ���pass�󣬽�out��ť��Ϊ���ɰ� */

	this->outcardOrder = (this->outcardOrder + 1) % 3;
}

void GameScene::out_callback(Ref*){
	pass->setVisible(false);
	out->setVisible(false);
	out->setEnabled(false);	/* ÿ�γ��ƻ���pass�󣬽�out��ť��Ϊ���ɰ� */

	lastOutCards = OutCards::create(player, GameRules::getInstance()->analysePokerValueType(arrWaitPlayOut),
		arrWaitPlayOut.size(), arrWaitPlayOut.at(arrWaitPlayOut.size() - 1));
	lastOutCards->retain();		/* ��ֹ���ڴ���������� */

	deleteCardInScene();		/* �ڽ�cardInScene����µ��˿�ʱ���Ƚ���ǰ����Scene���˿�ɾ�� */

	cardsInScene = arrWaitPlayOut;	/* �������Ʒŵ����Ƶ���������ڳ���������ʾ */

	for (int i = 0; i < arrWaitPlayOut.size(); ++i){
		arrWaitPlayOut.at(i)->removeFromParent();
		player->removePoker(arrWaitPlayOut.at(i));
		player->updatePokerPos();
	}

	arrWaitPlayOut.clear();	/* �������Ƴ��ˣ���Ҫ��������������գ���Ȼ���´ε��ظ���һ��ᵼ�´��� */

	outCardInScene();

	/* �������Ѿ������ƣ����ʤ */
	if (player->getPoker().size() == 0){
		this->gameState = END;
		return;
	}

	this->outcardOrder = (this->outcardOrder + 1) % 3;
}

void GameScene::nocall_callback(Ref*){
	log("Call Landlord---0!!!");

	player->setCallLandlordScore(0);
	updateCallLandlordOrder();	/* ���½зֵ�ID��ʹ��һ���˽з� */
	setCallLandlordButtonUnVisible();
}

void GameScene::callone_callback(Ref*){
	log("Call Landlord---1!!!");
	player->setCallLandlordScore(1);
	updateCallLandlordOrder();
	setCallLandlordButtonUnVisible();
}

void GameScene::calltwo_callback(Ref*){
	log("Call Landlord---2!!!");
	player->setCallLandlordScore(2);
	updateCallLandlordOrder();
	setCallLandlordButtonUnVisible();
}

void GameScene::callthree_callback(Ref*){
	log("Call Landlord---3!!!");
	player->setCallLandlordScore(3);
	updateCallLandlordOrder();
	isMaxCallLandlordScore(player);
	setCallLandlordButtonUnVisible();
}

void GameScene::initOutCardOrder(){
	this->outcardOrder = 0;
	/* ����ǵ����Ͳ��뵽Vector��λ�������Ͳ��迼��˳�������ˣ�ֻҪ����1��2��3��˳�������Ƽ��� */
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
	/* ��ʼ��outCards */
	this->lastOutCards = OutCards::create(this->players.at(0), NONE, 0, nullptr);
	lastOutCards->retain();
}

void GameScene::outCardForPlayer(Player* _player){
	/* �ò����ͳ��ư�ť��ʾ���������ư�ť���ɰ��ɳ��ƺͲ�����ť�Ĵ����¼����� */
	pass->setVisible(true);
	out->setVisible(true);
	/* �ֵ���ҳ���ʱ����ҿ����Ѿ�׼����Ҫ�����ƣ�����ֵ���ҳ���ʱ����Ҫ�Դ���һ�θ��� */
	this->updateOutState();
}

void GameScene::outCardForComputer(Player* _computer){
  	Vector<Poker*> _pokers;
	                                                                                /* �����һ����Ҳ���Լ��� */
	if (lastOutCards->getPokerOwner() == _computer || lastOutCards->getPokerValueType() == NONE){
		//_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), SINGLE, nullptr);	/* ����д�ᵼ�µ������Ҳ������ź�һֱ��������ط� */ 
		_pokers = GameRules::getInstance()->searchProperPokers(_computer->getPoker());
	} else{
		PokerValueType _pokerValueType = lastOutCards->getPokerValueType();
		if (_pokerValueType != KINGBOMB){
			if (_pokerValueType == BOMB){
				_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), BOMB, lastOutCards->getLowestPoker());
			}else{
				_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), lastOutCards->getPokerValueType(), lastOutCards->getLowestPoker(), lastOutCards->getTotalLength());
				if (_pokers.size() == 0){ /* ����Ҳ�����Ӧ���ƣ�����ը�� */
					_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), BOMB, nullptr);
					if (_pokers.size() == 0){	/* ����Ҳ�����ͨ��ը��������ը */
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
		cardsInScene = _pokers;	/* ������Գ����ˣ���ô�������Ʒ������ʾ��Scene�е����� */
		outCardInScene();
	}
	 
	if (_computer->getPoker().size() == 0){
		this->gameState = END;
		return;
	}

	this->outcardOrder = (this->outcardOrder + 1) % 3;
}

void GameScene::outCardInOrder(float delta){
	/*switch (outcardOrder){
	case 0: outCardForPlayer(player); break;
	case 1: outCardForComputer(computerPlayer_one); break;
	case 2: outCardForComputer(computerPlayer_two); break;
	default:break;
	}*/
	auto _player = players.at(outcardOrder);
	if (_player->getPlayerType() == PLAYER) outCardForPlayer(_player);
	else if (_player->getPlayerType() == COMPUTER) outCardForComputer(_player);
}

/* �����Ʒ��ڸ߶Ⱥ͵��Ե��Ƹ߶�һ�£��������1/2��Ļ��� */
/* �����Ʒ���һ�������Ȼ������������ */
void GameScene::outCardInScene(){
	if (cardsInScene.size() == 0) return;

	//int _height = computerPlayer_one->getPosition().y;
	int _height = Director::getInstance()->getVisibleSize().height / 2;	/* �߶�:��Ļ����λ�� */
	float _middleWidth = Director::getInstance()->getVisibleSize().width / 2;
	float _maxWidth = Director::getInstance()->getVisibleSize().width / 2;

	int _cardsNum = cardsInScene.size();	/* �������� */
	float _cardWidth = cardsInScene.at(0)->getContentSize().width;	/* ���ƿ�� */
	float interval = (_maxWidth - _cardWidth) < (_cardWidth - MIMIUM_CARDS_OVERLAPWIDTH) * (_cardsNum - 1) ? (_maxWidth - _cardWidth) / (_cardsNum - 1) : (_cardWidth - MIMIUM_CARDS_OVERLAPWIDTH);

	float startPosX = _middleWidth - _maxWidth / 2; /* ������г����ƵĿ�ȼ����������ܹ���ʾ�����򣬼�����ƵĿ�ʼλ�� */
	if (interval == (_cardWidth - MIMIUM_CARDS_OVERLAPWIDTH)){
		startPosX = _cardsNum % 2 == 0 ?
			_middleWidth - (_cardsNum / 2) *  interval :
			_middleWidth - (_cardsNum / 2 + 0.5) * interval;
	}

	for (int i = 0; i < cardsInScene.size(); ++i){
		this->addChild(cardsInScene.at(i));
		cardsInScene.at(i)->setPosition(startPosX + _cardWidth / 2 + interval * i, _height);
		cardsInScene.at(i)->showFront();
		/* ����������canClick��������Ϊfalse��ʹ�䲻�ɵ�� */
		cardsInScene.at(i)->setCanClick(false);
	}
}

void GameScene::outCardForLandlord(){
	landlordPlayer->insertCards(cardForLandlord);
	landlordPlayer->updatePokerPos();	/* ����˿���Ϻ��������� */

	cardForLandlord.clear();	/* ���ڵ������Ʒ���������������Ϊ�� */
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

void GameScene::updateHeadImage(){
	if (player->getLandlord() == true){
		playerHeadImage->setHeadImageType(LANDLORD, RIGHT);
	}else{
		playerHeadImage->setHeadImageType(FARMER, RIGHT);
	}

	if (computerPlayer_one->getLandlord() == true){
		computerPlayer_one_headImage->setHeadImageType(LANDLORD, RIGHT);
	}else{
		computerPlayer_one_headImage->setHeadImageType(FARMER, RIGHT);
	}

	if (computerPlayer_two->getLandlord() == true){
		computerPlayer_two_headImage->setHeadImageType(LANDLORD, LEFT);
	}
	else{
		computerPlayer_two_headImage->setHeadImageType(FARMER, LEFT);
	}
}

void GameScene::deleteCardInTop(){
	for (auto it : cardDisplayInTop){
		this->removeChild(it);
	}
	cardDisplayInTop.clear(); 
}

void GameScene::gameStart(float delta){
	initPoker();	/* ���Ƴ�ʼ�� */
	shuffleCards();	/* ϴ�� */
	initPlayer();	/* ��ʼ�������� */
	initHeadImage();	/* ��ʼ�������ҵ�ͷ�� */
	initCallLandlord();	/* ��ʼ���е��� */

	this->gameState = DEAL;
	this->schedule(schedule_selector(GameScene::update), 2.0f);
}

void GameScene::gameOver(){
	/* �������Ӵ��� */
	if (player->getPoker().size() == 0){
		log("You Win!");
		this->runWinAnimation();
	}else{
		log("You Lose!");
		this->runLostAnimation();
	}

	/* ��Ϸ������һЩ��Դ������� */
	pokers.clear();	/* �����ÿ� */
	/* ɾ����Һ͵��Ե����п��� */
	player->removeAllPoker();
	computerPlayer_one->removeAllPoker();
	computerPlayer_two->removeAllPoker();
	/* ɾ����Һ͵��Զ˵�ָ�� */
	this->removeChild(player);
	this->removeChild(computerPlayer_one);
	this->removeChild(computerPlayer_two);
	players.clear();	/* ɾ�������������������ָ�룬���û����һ����
						�µ�һ�ֿ�ʼʱ������޷�����*/
	this->removeChild(playerHeadImage);
	this->removeChild(computerPlayer_one_headImage);
	this->removeChild(computerPlayer_two_headImage);

	deleteCardInTop();	/* ����ڶ����ĵ����˿� */

	deleteCardInScene();	/* ɾ����Scene���˿� */

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