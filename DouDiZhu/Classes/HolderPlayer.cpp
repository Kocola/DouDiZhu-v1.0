#include "GameScene.h"
#include "GameRules.h"
#include "HolderPlayer.h"
#include "MusicController.h"
#include "OutCards.h"
#include "PlayerOrder.h"

HolderPlayer* HolderPlayer::create(PlayerPosType _playerPosType){
	auto pRet = new HolderPlayer();
	if (pRet && pRet->init(_playerPosType)){
		pRet->autorelease();
		return pRet;
	}else{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool HolderPlayer::init(PlayerPosType _playerPosType){
	if (!Player::init(_playerPosType)) return false;

	lastOutCard = nullptr;

	return true;
}

void HolderPlayer::initResource(){
	initObserver();
	initCardPos();
	initMenuItemSprite();
	initPassHintSprite();
}

void HolderPlayer::initObserver(){
	/* ��������������������ƵĹ۲��� */
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(HolderPlayer::addCardForWaitOut),
		ADDCARDFORWAITOUT, nullptr);
	/* ���������������ɾ���ƵĹ۲��� */
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(HolderPlayer::deleteCardForWaitOut),
		DELETECARDFORWAITOUT, nullptr);
	/* ���³���״̬ */
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(HolderPlayer::updateOutState),
		UPDATEOUTSTATE, nullptr);
}

void HolderPlayer::initCardPos(){
	displayCardMiddleX = 0;
	auto _visibleWidth = Director::getInstance()->getVisibleSize().width;
	displayCardStartX = displayCardMiddleX - 5.0 / 12 * _visibleWidth;
}

void HolderPlayer::initMenuItemSprite(){
	auto visibleSize = Director::getInstance()->getVisibleSize();	/* ��Ļ�ߴ� */

	auto _menu = Menu::create();
	_menu->setPosition(Point(0, 0));	/* Menu����ʱ��Ҫ����λ��Ĭ�ϱ�Ϊ0 */

	/* ��ʼ��ť�Ĳ˵� */
	auto _start = Sprite::create("Image/btn_start.png");
	auto _start_pressed = Sprite::create("Image/btn_start_selected.png");
	startMenuItem = MenuItemSprite::create(_start, _start_pressed, 
		CC_CALLBACK_1(HolderPlayer::startCallback, this));

	/* ������ť */
	auto _pass = Sprite::create("Image/btn_pass.png");
	auto _pass_pressed = Sprite::create("Image/btn_pass_selected.png");// Sprite::createWithSpriteFrame(_pass->getSpriteFrame());	/* ���þ���֡�����ƴ���һ������ */
	auto _pass_disabled = Sprite::create("Image/btn_pass.png");
	passMenuItem = MenuItemSprite::create(_pass, _pass_pressed, 
		CC_CALLBACK_1(HolderPlayer::passCallback, this));
	//out->setCallback(CC_CALLBACK_1(GameScene::pass_callback, this)); /* ����д�������Ϊʲô��*/ 

	/* ��ʾ��ť */
	auto _hint = Sprite::create("Image/btn_hint.png");
	auto _hint_pressed = Sprite::create("Image/btn_hint_selected.png");
	auto _hint_disabled = Sprite::create("Image/btn_hint_disabled.png");
	hintMenuItem = MenuItemSprite::create(_hint, _hint_pressed, 
		CC_CALLBACK_1(HolderPlayer::hintCallback, this));
	hintMenuItem->setDisabledImage(_hint_disabled);
	hintMenuItem->setEnabled(false);		/* ��ʾ�Ƿ���Ե�������ݵ�ǰ�Ƿ���ڴ�����һ�ҵ��� */

	/* ���ư�ť */
	auto _out = Sprite::create("Image/btn_out.png");
	auto _out_pressed = Sprite::create("Image/btn_out_selected.png");
	auto _out_disabled = Sprite::create("Image/btn_out_disabled.png");
	outMenuItem = MenuItemSprite::create(_out, _out_pressed, 
		CC_CALLBACK_1(HolderPlayer::outCallback, this));
	outMenuItem->setDisabledImage(_out_disabled);
	outMenuItem->setEnabled(false);		/* ��ʼ��ʱ���� ���� ��ť���ɰ���*/

	/* ���в˵� */
	auto _nocall = Sprite::create("Image/btn_nocall.png");
	auto _nocall_pressed = Sprite::create("Image/btn_nocall_selected.png");
	noCallMenuItem = MenuItemSprite::create(_nocall, _nocall_pressed, 
		CC_CALLBACK_1(HolderPlayer::noCallCallback, this));

	/* һ�ֲ˵� */
	auto _call_one = Sprite::create("Image/btn_one.png");
	auto _call_one_selected = Sprite::create("Image/btn_one_selected.png");
	oneScoreMenuItem = MenuItemSprite::create(_call_one, _call_one_selected, 
		CC_CALLBACK_1(HolderPlayer::oneScoreCallback, this));

	/* ���ֲ˵� */
	auto _call_two = Sprite::create("Image/btn_two.png");
	auto _call_two_selected = Sprite::create("Image/btn_two_selected.png");
	twoScoreMenuItem = MenuItemSprite::create(_call_two, _call_two_selected, 
		CC_CALLBACK_1(HolderPlayer::twoScoreCallback, this));

	/* ���ֲ˵� */
	auto _call_three = Sprite::create("Image/btn_three.png");
	auto _call_three_selected = Sprite::create("Image/btn_three_selected.png");
	threeScoreMenuItem = MenuItemSprite::create(_call_three, _call_three_selected, 
		CC_CALLBACK_1(HolderPlayer::threeScoreCallback, this));

	/*                          ��ʼ��ť                                            */

	/* ���ÿ�ʼ��ť���ɼ� */
	startMenuItem->setVisible(false);

	/* ���ÿ�ʼ��ťλ�� �� ������Ļ�м䣬�߶Ⱥ����ͷ�����*/
	auto start_pos = Point(visibleSize.width / 2,
		DISPLAYCARDHEIGHT + POKER_HEIGHT + 20 + startMenuItem->getContentSize().height / 2);
	start_pos = this->convertToNodeSpace(start_pos);
	startMenuItem->setPosition(start_pos);

	/* �� ��ʼ  MenuItemSprite���ӵ�Menu����ʾ */
	_menu->addChild(startMenuItem);

	/*                               �зְ�ť                                       */

	/* �зְ�ťȫ������λ���ɼ� */
	noCallMenuItem->setVisible(false);
	oneScoreMenuItem->setVisible(false);
	twoScoreMenuItem->setVisible(false);
	threeScoreMenuItem->setVisible(false);

	/* ���ð�ťλ�� */
	auto _nocall_pos = Point(visibleSize.width / 2 - oneScoreMenuItem->getContentSize().width - BUTTON_INTERVAL - noCallMenuItem->getContentSize().width / 2 - BUTTON_INTERVAL / 2,
		POKER_HEIGHT + DISPLAYCARDHEIGHT + noCallMenuItem->getContentSize().height / 2 + 15);
	_nocall_pos = this->convertToNodeSpace(_nocall_pos);
	noCallMenuItem->setPosition(_nocall_pos);
	auto _call_one_pos = Point(visibleSize.width / 2 - oneScoreMenuItem->getContentSize().width / 2 - BUTTON_INTERVAL / 2,
		POKER_HEIGHT + DISPLAYCARDHEIGHT + oneScoreMenuItem->getContentSize().height / 2 + 15);
	_call_one_pos = this->convertToNodeSpace(_call_one_pos);
	oneScoreMenuItem->setPosition(_call_one_pos);
	auto _call_two_pos = Point(visibleSize.width / 2 + twoScoreMenuItem->getContentSize().width / 2 + BUTTON_INTERVAL / 2,
		POKER_HEIGHT + DISPLAYCARDHEIGHT + twoScoreMenuItem->getContentSize().height / 2 + 15);
	_call_two_pos = this->convertToNodeSpace(_call_two_pos);
	twoScoreMenuItem->setPosition(_call_two_pos);
	auto _call_three_pos = Point(visibleSize.width / 2 + BUTTON_INTERVAL / 2 + twoScoreMenuItem->getContentSize().width + BUTTON_INTERVAL + threeScoreMenuItem->getContentSize().width / 2,
		POKER_HEIGHT + DISPLAYCARDHEIGHT + threeScoreMenuItem->getContentSize().height / 2 + 15);
	_call_three_pos = this->convertToNodeSpace(_call_three_pos);
	threeScoreMenuItem->setPosition(_call_three_pos);

	/* ��MenuItemSprite���ӵ�Menu����ʾ */
	_menu->addChild(noCallMenuItem);
	_menu->addChild(oneScoreMenuItem);
	_menu->addChild(twoScoreMenuItem);
	_menu->addChild(threeScoreMenuItem);

	/*                                ��������ʾ�����ư�ť                                      */

	passMenuItem->setVisible(false);
	hintMenuItem->setVisible(false);
	outMenuItem->setVisible(false);/* ��ʼ��ʱ���� ������ť ���ɼ� */

	/* ���ð�ťλ�� */
	auto _posY = POKER_HEIGHT + DISPLAYCARDHEIGHT + passMenuItem->getContentSize().height / 2 + 15;
	auto _hint_width = hintMenuItem->getContentSize().width;
	auto _pass_pos = Point(visibleSize.width / 2 - (_hint_width / 2 + BUTTON_INTERVAL + _pass->getContentSize().width / 2), _posY);
	_pass_pos = this->convertToNodeSpace(_pass_pos);
	auto _hint_pos = Point(visibleSize.width / 2, _posY);
	_hint_pos = this->convertToNodeSpace(_hint_pos);
	auto _out_pos = Point(visibleSize.width / 2 + (_hint_width / 2 + BUTTON_INTERVAL + _out->getContentSize().width / 2), _posY);
	_out_pos = this->convertToNodeSpace(_out_pos);
	passMenuItem->setPosition(_pass_pos);
	hintMenuItem->setPosition(_hint_pos);
	outMenuItem->setPosition(_out_pos);

	/* ��MenuItemSprite���ӵ�Menu����ʾ */
	_menu->addChild(passMenuItem);
	_menu->addChild(hintMenuItem);
	_menu->addChild(outMenuItem);

	this->addChild(_menu);	/* MenuItemSprite�����������Menu�в�������ʹ�� */
}

void HolderPlayer::initPassHintSprite(){
	passHintSprite = Sprite::create("Image/passhint.png");	/* ���ƴ�ù��ϼҵ���ʾ ���� */
	auto _hint_btn_pos = hintMenuItem->getPosition();
	auto _hint_btn_size = hintMenuItem->getContentSize();
	auto _pass_hint_pos = Point(_hint_btn_pos.x, _hint_btn_pos.y + _hint_btn_size.height / 2 + 15 + passHintSprite->getContentSize().height / 2);
	//_pass_hint_pos = this->convertToNodeSpace(_pass_hint_pos);
	passHintSprite->setPosition(_pass_hint_pos);
	this->addChild(passHintSprite);
	passHintSprite->setVisible(false);	/* ��ʼ���ɼ� */
}

void HolderPlayer::updateOutState(){
	/* �����lastOutCardÿ��ֻ�д��ֵ��Լ�����ʱ�Ż�ǿգ���������Լ����Ʊ����ÿ� */

	OutCards* _lastOutCard = this->lastOutCard;
	if (_lastOutCard == nullptr) return;	/* �����ǰ�����ǳ���״̬����ôֱ�ӷ��� */
	/* �����ǰ������Ҳ���player����ô����Ҫ���³��ư�ť�Ŀɰ��� */
	//if (_lastOutCard->getPokerOwner() != this) return;
	if (_lastOutCard->getPokerValueType() == NONE || _lastOutCard->getPokerOwner() == this){
		/* �����һ�εĳ�����NONE����ʾ�տ�ʼ������ ��һ�γ��Ƶĳ����߻���player��
		��ô�жϵ�ǰ���������ǲ�����ȷ�����ͣ�����ǣ�������ư�ť�ɰ������򲻿ɰ� */
		if (GameRules::getInstance()->isPokerValueType(cardsForWaitOut) == true){
			outMenuItem->setEnabled(true);
		}else{
			outMenuItem->setEnabled(false);
		}
		return;
	}
	else{
		PokerValueType _pokerValueType = GameRules::getInstance()->analysePokerValueType(cardsForWaitOut);
		if (_pokerValueType == NONE){	/* �����ǰ�����κ����� */
			outMenuItem->setEnabled(false);
		}else{
			outMenuItem->setEnabled(GameRules::getInstance()->canOutCards(cardsForWaitOut, _lastOutCard));
		}
	}
}

void HolderPlayer::callLandlord(){
	/* ��ʾ���нзְ�ť */
	this->setCallLandLordMenuItemVisible(true);
	/* �ȴ��з� */
}

void HolderPlayer::outCard(OutCards* _lastOutcards){
	CC_ASSERT(_lastOutcards != nullptr);

	this->deleteOutcardInScene();	/* ɾ��֮ǰ���� */

	/* ���ó�Ա����lastOutcards�������ƺ��������Ϊnullptr */
	this->setLastOutCard(_lastOutcards);

	/* �ò�������ʾ�ͳ��ư�ť��ʾ���������ư�ť���ɰ��ɳ��ƺͲ�����ť�Ĵ����¼����� */
	passMenuItem->setVisible(true);
	hintMenuItem->setVisible(true);
	outMenuItem->setVisible(true);
	/* ���ܼ���Ƿ����ƴ�ù��ϼң�������ʾ��ť�Ƿ�ɰ��£���ʾ���˿���Ƴɳ�Ա������
	Ŀ���ǰ�����ʾ��ťʱ������Ҫ�ٵ���searchOutCard���� */
	this->hintCards = searchHintCard(_lastOutcards);
	if (this->hintCards.size() != 0){
		hintMenuItem->setEnabled(true);		/* ����п��Գ����ƣ���ô��ʾ��ť�ɰ��� */
	}
	else{
		passHintSprite->setVisible(true);	/* ���û�п��Գ����ƣ�����ʾ"û���ƴ�ù��ϼ�" */
	}

	float _outCardDuration = 15.0f;
	if (this->hintCards.size() == 0) _outCardDuration = 3.0f;

	this->startCountDown(_outCardDuration, [&](){ this->pass(); });

	/* �ֵ���ҳ���ʱ����ҿ����Ѿ�׼����Ҫ�����ƣ�����ֵ���ҳ���ʱ����Ҫ�Դ���һ�θ��� */
	this->updateOutState();
}

Vector<Poker*> HolderPlayer::searchHintCard(OutCards* _lastOutcards){
	Vector<Poker*> _pokers;
	/* �����һ����Ҳ���Լ��� */
	if (_lastOutcards->getPokerOwner() ==  this || _lastOutcards->getPokerValueType() == NONE){
		//_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), SINGLE, nullptr);	/* ����д�ᵼ�µ������Ҳ������ź�һֱ��������ط� */ 
		_pokers = GameRules::getInstance()->searchProperPokers(this->getPoker());
	}
	else{
		PokerValueType _pokerValueType = _lastOutcards->getPokerValueType();
		if (_pokerValueType != KINGBOMB){
			if (_pokerValueType == BOMB){
				_pokers = GameRules::getInstance()->calcPokerWithValueTypeInSplit(
					this->getPoker(), BOMB, _lastOutcards->getLowestPoker());
			}
			else{
				_pokers = GameRules::getInstance()->calcPokerWithValueTypeInSplit(
					this->getPoker(), _lastOutcards->getPokerValueType(), _lastOutcards->getLowestPoker(), _lastOutcards->getTotalLength());

				if (_pokers.size() == 0){ /* ����Ҳ�����Ӧ���ƣ�����ը�� */
					_pokers = GameRules::getInstance()->calcPokerWithValueTypeInSplit(this->getPoker(), BOMB, nullptr);
					if (_pokers.size() == 0){	/* ����Ҳ�����ͨ��ը��������ը */
						_pokers = GameRules::getInstance()->calcPokerWithValueTypeInSplit(this->getPoker(), KINGBOMB);
					}
				}
			}
		}
	}
	return _pokers;
}

void HolderPlayer::pass(){
	/* ���ŵ����ť����Ч */
	MusicController::getInstance()->playPassEffect();

	passMenuItem->setVisible(false);		/* ������ť���ɼ� */
	hintMenuItem->setVisible(false);		/* ��ʾ��ť���ɼ� */
	hintMenuItem->setEnabled(false);	/* ��ʾ��ť���ɰ��� */
	outMenuItem->setVisible(false);		/* ���ư�ť���ɼ� */
	outMenuItem->setEnabled(false);	/* ÿ�γ��ƻ���pass�󣬽�out��ť��Ϊ���ɰ� */

	passHintSprite->setVisible(false);

	this->getPlayerOrder()->setPlayerOrderState(PASS);	/* ��ʾ������״̬ */
	this->getPlayerOrder()->setVisible(true);

	this->setLastOutCardNull();	/* ��������lastOutcard��Ա����Ϊnullptr */
	this->updateOutOrder();	/* ���³���˳�� */
}

void HolderPlayer::updateCardPos(){
	Vector<Poker*> _pokers = this->getPoker();
	if (_pokers.size() == 0) return;	/* ���û���˿��ˣ�ֱ�ӷ��أ�һ�㲻���������� */
	float _displayCardMaxWidth = Director::getInstance()->getVisibleSize().width * 5.0 / 6;
	float _displayCardY = 0;
	displayCard(_pokers, _displayCardMaxWidth, displayCardStartX, displayCardMiddleX, _displayCardY, true);
}

/************************************************************************/
/*                              �ص�����                                        */
/************************************************************************/
void HolderPlayer::setCallLandLordMenuItemVisible(bool _visible){
	noCallMenuItem->setVisible(_visible);
	oneScoreMenuItem->setVisible(_visible);
	twoScoreMenuItem->setVisible(_visible);
	threeScoreMenuItem->setVisible(_visible);
}

void HolderPlayer::startCallback(Ref*){
	/* ���ŵ����ť����Ч */
	MusicController::getInstance()->playPressButtonEffect();

	this->getPlayerOrder()->setVisible(true);
	startMenuItem->setVisible(false);
	Player::setReady(true);	/* �����ֶ�����Ѿ�׼���� */
}

void HolderPlayer::passCallback(Ref*){
	this->stopCountDown();
	this->pass();
}

void HolderPlayer::hintCallback(Ref*){
	/* ���ŵ����ť����Ч */
	MusicController::getInstance()->playPressButtonEffect();

	/* ��������ʾ��ť����ô����Ҫ���Լ�������ƻָ�������λ�� */
	Vector<Poker*> _pokers = this->cardsForWaitOut;
	for (int i = 0; i < _pokers.size(); ++i){
		auto _poker = _pokers.at(i);
		_poker->selectedCardBack();		/* �ѳ����ƻָ�λ�� */
	}
	//this->arrWaitPlayOut.clear();	/* ��մ�����ť */

	/* ����ʾ���˿����ƣ���ɴ�����״̬ */
	for (int i = 0; i < this->hintCards.size(); ++i){
		auto _poker = this->hintCards.at(i);
		_poker->selectedCardOut();	/* �˿˱�ɴ���״̬ */
	}
	this->updateOutState();	/* ��ʾ��ť�󣬸��³��ư�ť��״̬������ֱ�ӵ���updateOutState��������ֱ���ó��ư�ť�ɰ� */
}

void HolderPlayer::outCallback(Ref*){
	/* ���ŵ����ť����Ч */
	MusicController::getInstance()->playPressButtonEffect();

	passMenuItem->setVisible(false);		/* ������ť���ɼ� */
	hintMenuItem->setVisible(false);		/* ��ʾ��ť���ɼ� */
	hintMenuItem->setEnabled(false);	/* ��ʾ��ť���ɰ��� */
	outMenuItem->setVisible(false);		/* ���ư�ť���ɼ� */
	outMenuItem->setEnabled(false);	/* ÿ�γ��ƻ���pass�󣬽�out��ť��Ϊ���ɰ� */

	passHintSprite->setVisible(false);

	auto _lastOutCard = OutCards::create(this, GameRules::getInstance()->analysePokerValueType(cardsForWaitOut),
		cardsForWaitOut.size(), cardsForWaitOut.at(cardsForWaitOut.size() - 1));
	_lastOutCard->retain();		/* ��ֹ���ڴ���������� */
	CC_ASSERT(_lastOutCard != nullptr);
	this->updateLastOutCards(_lastOutCard);

	this->deleteOutcardInScene();	/* �ڽ�cardInScene����µ��˿�ʱ���Ƚ���ǰ����Scene���˿�ɾ�� */

	this->setOutcardInScene(cardsForWaitOut);/* �������Ʒŵ����Ƶ���������ڳ���������ʾ */

	for (int i = 0; i < cardsForWaitOut.size(); ++i){
		cardsForWaitOut.at(i)->removeFromParent();
		this->removeCard(cardsForWaitOut.at(i));
		this->updateCardPos();
	}
	cardsForWaitOut.clear();	/* �������Ƴ��ˣ���Ҫ��������������գ���Ȼ���´ε��ظ���һ��ᵼ�´��� */

	this->showOutcardInScene();
	this->playOutCardInSceneMusic();	/* ���ų������� */

	/* �������Ѿ������ƣ���Ϸ���� */
	if (this->getPoker().size() == 0){
		this->setGameOver();
		return;
	}

	this->stopCountDown();
	this->setLastOutCardNull();	/* ���ƺ��������lastOutcardΪ�գ���Ȼ��Ϸ�߼������ */
	this->updateOutOrder();	/* ���³���˳�� */
}

void HolderPlayer::noCallCallback(Ref*){
	this->setCallLandlordScore(0);
	this->updateCallLandlordState();
	this->updateCallLandlordOrder();
	this->setCallLandLordMenuItemVisible(false);
}

void HolderPlayer::oneScoreCallback(Ref*){
	this->setCallLandlordScore(1);
	this->updateCallLandlordState();
	this->updateCallLandlordOrder();
	this->setCallLandLordMenuItemVisible(false);
}

void HolderPlayer::twoScoreCallback(Ref*){
	this->setCallLandlordScore(2);
	this->updateCallLandlordState();
	this->updateCallLandlordOrder();
	this->setCallLandLordMenuItemVisible(false);
}

void HolderPlayer::threeScoreCallback(Ref*){
	this->setCallLandlordScore(3);
	this->updateCallLandlordState();
	this->setCallLandLordMenuItemVisible(false);
	if (this->getCallLandlordScore() == MAXCALLLANDLORDSCORE){
		this->setGameStateChooseLandlord();
	}else{
		this->updateCallLandlordOrder();
	}
}

/************************************************************************/
/*                             �۲���                                         */
/************************************************************************/
void HolderPlayer::addCardForWaitOut(Ref* _data){
	auto _poker = dynamic_cast<Poker*>(_data);	/* ת��ΪPoker*���� */
	CC_ASSERT(_poker != nullptr);
	cardsForWaitOut.pushBack(_poker);
	GlobalFunc::sort(cardsForWaitOut);	/* ������������ */
}

void HolderPlayer::deleteCardForWaitOut(Ref* _data){
	auto _poker = dynamic_cast<Poker*>(_data);
	CC_ASSERT(_poker != nullptr);
	cardsForWaitOut.eraseObject(_poker);
	/* ɾ���Ʋ���Ҫ�������� */
}

void HolderPlayer::updateOutState(Ref*){
	this->updateOutState();
}