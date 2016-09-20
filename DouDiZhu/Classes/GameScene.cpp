#include <algorithm>
#include <ctime>
#include "GameScene.h"
#include "GlobalFunc.h"
#include "Player.h"
#include "Poker.h"
#include "PokerController.h"

using namespace std;

GameScene::GameScene(){
	gameState = DEAL;	/* ��ʼ״̬�Ƿ��� */
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
		CC_BREAK_IF(!initPoker());		/* �����˿� */
		CC_BREAK_IF(!shuffleCards());		/* ϴ�� */
		CC_BREAK_IF(!initPlayer());		/* ��ʼ����ң��������� */
		CC_BREAK_IF(!initButton());	/* ��ʼ����ť */

		NotificationCenter::getInstance()->addObserver(this,
			callfuncO_selector(GameScene::updatePokerPos), "UpdatePokerPos", nullptr);

		this->scheduleUpdate();

		tRet = true;
	} while (0);

	return tRet;
}

void GameScene::update(float delta){
	switch (gameState)
	{
	case DEAL: dealCard(); gameState = OUTCARD;
		break;
	case OUTCARD:log("OutCard!!!");
		break;
	case WIN:
		break;
	default:
		break;
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
	//std::sort(arrWaitPlayOut.begin(), arrWaitPlayOut.end(), cmp_sort);
	GlobalFunc::sort(arrWaitPlayOut);
}

void GameScene::updatePokerPos(Ref* data){
	Poker* poker = dynamic_cast<Poker*>(data);
	auto player = dynamic_cast<Player*>(poker->getParent());	/* ����Poker��Player������� */
	player->removePoker(poker);
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

	auto _pass = Sprite::create("Image/btn_pass.png");
	auto _pass_pressed = Sprite::create("Image/btn_pass_selected.png");// Sprite::createWithSpriteFrame(_pass->getSpriteFrame());	/* ���þ���֡�����ƴ���һ������ */
	auto _pass_disabled = Sprite::create("Image/btn_pass.png");
	pass = MenuItemSprite::create(_pass, _pass_pressed, nullptr); 

	auto _out = Sprite::create("Image/btn_out.png");
	auto _out_pressed = Sprite::create("Image/btn_out_selected.png");
	auto _out_disabled = Sprite::create("Image/btn_out_disabled.png");
	out = MenuItemSprite::create(_out, _out_pressed, _out_disabled);
	out->setEnabled(false);		/* ��ʼ��ʱ���� ���� ��ť���ɰ���*/

	pass->setVisible(false);
	out->setVisible(false);/* ��ʼ��ʱ���� ������ť ���ɼ� */

	auto _pass_pos = Point(visibleSize.width / 2 - _pass->getContentSize().width / 2 - 10, POKER_HEIGHT + DISPLAYCARDHEIGHT + _pass->getContentSize().height / 2 + 15);
	auto _out_pos = Point(visibleSize.width / 2 + _pass->getContentSize().width / 2 + 10, POKER_HEIGHT + DISPLAYCARDHEIGHT + out->getContentSize().height / 2 + 15);
	pass->setPosition(_pass_pos);
	out->setPosition(_out_pos);
	_menu->setPosition(Point(0, 0));

	_menu->addChild(pass);
	_menu->addChild(out);
	this->addChild(_menu);	/* MenuItemSprite�����������Menu�в�������ʹ�� */

	return true;
}

void GameScene::dealCard(Player* _player, Vector<Poker*>& _pokers, bool displayFront /* = false */){
	GlobalFunc::sort(_pokers);
	_player->setPoker(_pokers);	/* ������뽫�˿˽������������ӣ���Ȼ����Ϊ���˳���ԭ���¸��ǵ�˳�򲻶� */

	bool isCanClick = displayFront;
	for (auto it : _pokers){
		_player->addChild(it);
		it->setCanClick(isCanClick);
		if(displayFront == true) it->showFront();
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
	dealCard(player, _pokers, true);		/* ����ҷ��� */
	player->updatePokerPos();	/* ��ʼ��ʱ����ҵ��Ƶ�λ�ý��и��£���ֹֻ��ʾһ���� */

	_pokers.clear();
	for (int i = 1; i < TOTAL_POKER_NUM - 3; i += 3){
		_pokers.pushBack(pokers.at(i));
	}
	dealCard(computerPlayer_one, _pokers);	/* �Ե���1���� */ 

	_pokers.clear();
	for (int i = 2; i < TOTAL_POKER_NUM - 3; i += 3){
		_pokers.pushBack(pokers.at(i));
	}
	dealCard(computerPlayer_two, _pokers);
}

void GameScene::outCard(int order){
	switch (order){
	case 0:break;
	case 1:break;
	case 2:break;
	default:break;
	}
}