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

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [&](Touch* touch, Event* event){
		CC_ASSERT(clickedPokers.size() == 0);
		Vector<Poker*> _pokers = this->getPoker();
		auto touchPos = this->convertToNodeSpace(touch->getLocation());
		for (int i = _pokers.size() - 1; i >= 0; --i){
			auto rect = _pokers.at(i)->getBoundingBox();
			if (rect.containsPoint(touchPos) && _pokers.at(i)->getCanClick()){
				MusicController::getInstance()->playTouchCardEffect();
				clickedPokers.pushBack(_pokers.at(i));
				if (!_pokers.at(i)->getSelect()){
					/* 如果还未选择这张牌，那就将这张牌露出来 */
					_pokers.at(i)->selectedCardOut();
				}
				else{
					/* 否则这张牌已经被选择，将其放回 */
					_pokers.at(i)->selectedCardBack();
				}
				/* 检测当前牌是否可以另出牌按钮可按 */
				this->updateOutState();
				return true;
			}
		}
		return false;	/* 如果点击下的位置不在扑克范围内，那么即使触摸移动后也不会有任何操作 */
	};

	listener->onTouchMoved = [&](Touch* touch, Event* event){
		Vector<Poker*> _pokers = this->getPoker();
		auto touchPos = this->convertToNodeSpace(touch->getLocation());
		for (int i = _pokers.size() - 1; i >= 0; --i){
			auto rect = _pokers.at(i)->getBoundingBox();
			if (rect.containsPoint(touchPos) && _pokers.at(i)->getCanClick()){
				if (clickedPokers.find(_pokers.at(i)) != clickedPokers.end()) break;
				MusicController::getInstance()->playTouchCardEffect();
				clickedPokers.pushBack(_pokers.at(i));
				if (!_pokers.at(i)->getSelect()){
					/* 如果还未选择这张牌，那就将这张牌露出来 */
					_pokers.at(i)->selectedCardOut();
				}else{
					/* 否则这张牌已经被选择，将其放回 */
					_pokers.at(i)->selectedCardBack();
				}
				/* 检测当前牌是否可以另出牌按钮可按 */
				this->updateOutState();
				break;
			}
		}
	};

	listener->onTouchEnded = [&](Touch* touch, Event* event){
		clickedPokers.clear();		/* 触摸结束，清空clickedPokers */
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void HolderPlayer::initResource(){
	initObserver();
	initCardPos();
	initMenuItemSprite();
	initPassHintSprite();
}

void HolderPlayer::initObserver(){
	/* 添加向待出牌容器添加牌的观察者 */
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(HolderPlayer::addCardForWaitOut),
		ADDCARDFORWAITOUT, nullptr);
	/* 添加向待出牌容器删除牌的观察者 */
	NotificationCenter::getInstance()->addObserver(this,
		callfuncO_selector(HolderPlayer::deleteCardForWaitOut),
		DELETECARDFORWAITOUT, nullptr);
	/* 更新出牌状态 */
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
	auto visibleSize = Director::getInstance()->getVisibleSize();	/* 屏幕尺寸 */

	auto _menu = Menu::create();
	_menu->setPosition(Point(0, 0));	/* Menu创建时需要将其位置默认变为0 */

	/* 开始按钮的菜单 */
	auto _start = Sprite::create("Image/btn_start.png");
	auto _start_pressed = Sprite::create("Image/btn_start_selected.png");
	startMenuItem = MenuItemSprite::create(_start, _start_pressed, 
		CC_CALLBACK_1(HolderPlayer::startCallback, this));

	/* 不出按钮 */
	auto _pass = Sprite::create("Image/btn_pass.png");
	auto _pass_pressed = Sprite::create("Image/btn_pass_selected.png");// Sprite::createWithSpriteFrame(_pass->getSpriteFrame());	/* 利用精灵帧来复制创建一个精灵 */
	auto _pass_disabled = Sprite::create("Image/btn_pass.png");
	passMenuItem = MenuItemSprite::create(_pass, _pass_pressed, 
		CC_CALLBACK_1(HolderPlayer::passCallback, this));
	//out->setCallback(CC_CALLBACK_1(GameScene::pass_callback, this)); /* 这样写会出错，为什么？*/ 

	/* 提示按钮 */
	auto _hint = Sprite::create("Image/btn_hint.png");
	auto _hint_pressed = Sprite::create("Image/btn_hint_selected.png");
	auto _hint_disabled = Sprite::create("Image/btn_hint_disabled.png");
	hintMenuItem = MenuItemSprite::create(_hint, _hint_pressed, 
		CC_CALLBACK_1(HolderPlayer::hintCallback, this));
	hintMenuItem->setDisabledImage(_hint_disabled);
	hintMenuItem->setEnabled(false);		/* 提示是否可以点击是依据点前是否存在大于上一家的牌 */

	/* 出牌按钮 */
	auto _out = Sprite::create("Image/btn_out.png");
	auto _out_pressed = Sprite::create("Image/btn_out_selected.png");
	auto _out_disabled = Sprite::create("Image/btn_out_disabled.png");
	outMenuItem = MenuItemSprite::create(_out, _out_pressed, 
		CC_CALLBACK_1(HolderPlayer::outCallback, this));
	outMenuItem->setDisabledImage(_out_disabled);
	outMenuItem->setEnabled(false);		/* 初始化时设置 出牌 按钮不可按下*/

	/* 不叫菜单 */
	auto _nocall = Sprite::create("Image/btn_nocall.png");
	auto _nocall_pressed = Sprite::create("Image/btn_nocall_selected.png");
	noCallMenuItem = MenuItemSprite::create(_nocall, _nocall_pressed, 
		CC_CALLBACK_1(HolderPlayer::noCallCallback, this));

	/* 一分菜单 */
	auto _call_one = Sprite::create("Image/btn_one.png");
	auto _call_one_selected = Sprite::create("Image/btn_one_selected.png");
	oneScoreMenuItem = MenuItemSprite::create(_call_one, _call_one_selected, 
		CC_CALLBACK_1(HolderPlayer::oneScoreCallback, this));

	/* 二分菜单 */
	auto _call_two = Sprite::create("Image/btn_two.png");
	auto _call_two_selected = Sprite::create("Image/btn_two_selected.png");
	twoScoreMenuItem = MenuItemSprite::create(_call_two, _call_two_selected, 
		CC_CALLBACK_1(HolderPlayer::twoScoreCallback, this));

	/* 三分菜单 */
	auto _call_three = Sprite::create("Image/btn_three.png");
	auto _call_three_selected = Sprite::create("Image/btn_three_selected.png");
	threeScoreMenuItem = MenuItemSprite::create(_call_three, _call_three_selected, 
		CC_CALLBACK_1(HolderPlayer::threeScoreCallback, this));

	/*                          开始按钮                                            */

	/* 设置开始按钮不可见 */
	startMenuItem->setVisible(false);

	/* 设置开始按钮位置 ： 宽在屏幕中间，高度和玩家头像对齐*/
	auto start_pos = Point(visibleSize.width / 2,
		DISPLAYCARDHEIGHT + POKER_HEIGHT + 20 + startMenuItem->getContentSize().height / 2);
	start_pos = this->convertToNodeSpace(start_pos);
	startMenuItem->setPosition(start_pos);

	/* 将 开始  MenuItemSprite添加到Menu里显示 */
	_menu->addChild(startMenuItem);

	/*                               叫分按钮                                       */

	/* 叫分按钮全部设置位不可见 */
	noCallMenuItem->setVisible(false);
	oneScoreMenuItem->setVisible(false);
	twoScoreMenuItem->setVisible(false);
	threeScoreMenuItem->setVisible(false);

	/* 设置按钮位置 */
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

	/* 将MenuItemSprite添加到Menu里显示 */
	_menu->addChild(noCallMenuItem);
	_menu->addChild(oneScoreMenuItem);
	_menu->addChild(twoScoreMenuItem);
	_menu->addChild(threeScoreMenuItem);

	/*                                不出，提示，出牌按钮                                      */

	passMenuItem->setVisible(false);
	hintMenuItem->setVisible(false);
	outMenuItem->setVisible(false);/* 初始化时设置 两个按钮 不可见 */

	/* 设置按钮位置 */
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

	/* 将MenuItemSprite添加到Menu里显示 */
	_menu->addChild(passMenuItem);
	_menu->addChild(hintMenuItem);
	_menu->addChild(outMenuItem);

	this->addChild(_menu);	/* MenuItemSprite对象必须存放在Menu中才能正常使用 */
}

void HolderPlayer::initPassHintSprite(){
	passHintSprite = Sprite::create("Image/passhint.png");	/* 无牌打得过上家的提示 精灵 */
	auto _hint_btn_pos = hintMenuItem->getPosition();
	auto _hint_btn_size = hintMenuItem->getContentSize();
	auto _pass_hint_pos = Point(_hint_btn_pos.x, _hint_btn_pos.y + _hint_btn_size.height / 2 + 15 + passHintSprite->getContentSize().height / 2);
	//_pass_hint_pos = this->convertToNodeSpace(_pass_hint_pos);
	passHintSprite->setPosition(_pass_hint_pos);
	this->addChild(passHintSprite);
	passHintSprite->setVisible(false);	/* 初始不可见 */
}

void HolderPlayer::updateOutState(){
	/* 这里的lastOutCard每次只有待轮到自己出牌时才会非空，如果不是自己出牌必须置空 */

	OutCards* _lastOutCard = this->lastOutCard;
	if (_lastOutCard == nullptr) return;	/* 如果当前还不是出牌状态，那么直接返回 */
	/* 如果当前出牌玩家不是player，那么不需要更新出牌按钮的可按性 */
	//if (_lastOutCard->getPokerOwner() != this) return;
	if (_lastOutCard->getPokerValueType() == NONE || _lastOutCard->getPokerOwner() == this){
		/* 如果上一次的出牌是NONE（表示刚开始）或者 上一次出牌的持有者还是player，
		那么判断当前待出的牌是不是正确的牌型，如果是，就令出牌按钮可按，否则不可按 */
		if (GameRules::getInstance()->isPokerValueType(cardsForWaitOut) == true){
			outMenuItem->setEnabled(true);
		}else{
			outMenuItem->setEnabled(false);
		}
		return;
	}
	else{
		PokerValueType _pokerValueType = GameRules::getInstance()->analysePokerValueType(cardsForWaitOut);
		if (_pokerValueType == NONE){	/* 如果当前不是任何牌型 */
			outMenuItem->setEnabled(false);
		}else{
			outMenuItem->setEnabled(GameRules::getInstance()->canOutCards(cardsForWaitOut, _lastOutCard));
		}
	}
}

void HolderPlayer::callLandlord(){
	/* 显示所有叫分按钮 */
	this->setCallLandLordMenuItemVisible(true);
	/* 等待叫分 */
}

void HolderPlayer::outCard(OutCards* _lastOutcards){
	CC_ASSERT(_lastOutcards != nullptr);

	this->deleteOutcardInScene();	/* 删除之前的牌 */

	/* 设置成员变量lastOutcards，出完牌后必须设置为nullptr */
	this->setLastOutCard(_lastOutcards);

	/* 让不出，提示和出牌按钮显示出来，出牌按钮不可按由出牌和不出按钮的触发事件控制 */
	passMenuItem->setVisible(true);
	hintMenuItem->setVisible(true);
	outMenuItem->setVisible(true);
	/* 智能检查是否有牌打得过上家，控制提示按钮是否可按下，提示的扑克设计成成员变量，
	目的是按下提示按钮时，不需要再调用searchOutCard函数 */
	this->hintCards = searchHintCard(_lastOutcards);
	if (this->hintCards.size() != 0){
		hintMenuItem->setEnabled(true);		/* 如果有可以出的牌，那么提示按钮可按下 */
	}
	else{
		passHintSprite->setVisible(true);	/* 如果没有可以出的牌，就显示"没有牌打得过上家" */
	}

	float _outCardDuration = 35.0f;
	if (this->hintCards.size() == 0) _outCardDuration = 3.0f;

	this->startCountDown(_outCardDuration, [&](){ this->pass(); });

	/* 轮到玩家出牌时，玩家可能已经准备好要出的牌，因此轮到玩家出牌时，需要对此做一次更新 */
	this->updateOutState();
}

Vector<Poker*> HolderPlayer::searchHintCard(OutCards* _lastOutcards){
	Vector<Poker*> _pokers;
	/* 如果上一手牌也是自己的 */
	if (_lastOutcards->getPokerOwner() ==  this || _lastOutcards->getPokerValueType() == NONE){
		//_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), SINGLE, nullptr);	/* 这样写会导致电脑在找不到单张后一直卡在这个地方 */ 
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

				if (_pokers.size() == 0){ /* 如果找不到对应的牌，就找炸弹 */
					_pokers = GameRules::getInstance()->calcPokerWithValueTypeInSplit(this->getPoker(), BOMB, nullptr);
					if (_pokers.size() == 0){	/* 如果找不到普通的炸，就找王炸 */
						_pokers = GameRules::getInstance()->calcPokerWithValueTypeInSplit(this->getPoker(), KINGBOMB);
					}
				}
			}
		}
	}
	return _pokers;
}

void HolderPlayer::pass(){
	/* 播放点击按钮的音效 */
	MusicController::getInstance()->playPassEffect();

	passMenuItem->setVisible(false);		/* 不出按钮不可见 */
	hintMenuItem->setVisible(false);		/* 提示按钮不可见 */
	hintMenuItem->setEnabled(false);	/* 提示按钮不可按下 */
	outMenuItem->setVisible(false);		/* 出牌按钮不可见 */
	outMenuItem->setEnabled(false);	/* 每次出牌或者pass后，将out按钮设为不可按 */

	passHintSprite->setVisible(false);

	this->getPlayerOrder()->setPlayerOrderState(PASS);	/* 显示不出的状态 */
	this->getPlayerOrder()->setVisible(true);

	this->setLastOutCardNull();	/* 不出后置lastOutcard成员变量为nullptr */
	this->updateOutOrder();	/* 更新出牌顺序 */
}

void HolderPlayer::updateCardPos(){
	Vector<Poker*> _pokers = this->getPoker();
	if (_pokers.size() == 0) return;	/* 如果没有扑克了，直接返回，一般不会运行这句的 */
	float _displayCardMaxWidth = Director::getInstance()->getVisibleSize().width * 5.0 / 6;
	float _displayCardY = 0;
	displayCard(_pokers, _displayCardMaxWidth, displayCardStartX, displayCardMiddleX, _displayCardY, true);
}

/************************************************************************/
/*                              回调函数                                        */
/************************************************************************/
void HolderPlayer::setCallLandLordMenuItemVisible(bool _visible){
	noCallMenuItem->setVisible(_visible);
	oneScoreMenuItem->setVisible(_visible);
	twoScoreMenuItem->setVisible(_visible);
	threeScoreMenuItem->setVisible(_visible);
}

void HolderPlayer::startCallback(Ref*){
	/* 播放点击按钮的音效 */
	MusicController::getInstance()->playPressButtonEffect();

	this->getPlayerOrder()->setVisible(true);
	startMenuItem->setVisible(false);
	Player::setReady(true);	/* 设置手动玩家已经准备好 */
}

void HolderPlayer::passCallback(Ref*){
	this->stopCountDown();
	this->pass();
}

void HolderPlayer::hintCallback(Ref*){
	/* 播放点击按钮的音效 */
	MusicController::getInstance()->playPressButtonEffect();

	/* 如果点击提示按钮，那么首先要将自己点击的牌恢复到正常位置 */
	Vector<Poker*> _pokers = this->cardsForWaitOut;
	for (int i = 0; i < _pokers.size(); ++i){
		auto _poker = _pokers.at(i);
		_poker->selectedCardBack();		/* 已出的牌恢复位置 */
	}
	//this->arrWaitPlayOut.clear();	/* 清空待出按钮 */

	/* 将提示的扑克上移，变成待出的状态 */
	for (int i = 0; i < this->hintCards.size(); ++i){
		auto _poker = this->hintCards.at(i);
		_poker->selectedCardOut();	/* 扑克变成待出状态 */
	}
	this->updateOutState();	/* 提示按钮后，更新出牌按钮的状态，这里直接调用updateOutState，而不是直接置出牌按钮可按 */
}

void HolderPlayer::outCallback(Ref*){
	/* 播放点击按钮的音效 */
	MusicController::getInstance()->playPressButtonEffect();

	passMenuItem->setVisible(false);		/* 不出按钮不可见 */
	hintMenuItem->setVisible(false);		/* 提示按钮不可见 */
	hintMenuItem->setEnabled(false);	/* 提示按钮不可按下 */
	outMenuItem->setVisible(false);		/* 出牌按钮不可见 */
	outMenuItem->setEnabled(false);	/* 每次出牌或者pass后，将out按钮设为不可按 */

	passHintSprite->setVisible(false);

	//PokerValueType _pokerValueType = GameRules::getInstance()->analysePokerValueType(cardsForWaitOut);
	//Poker* _lowestPoker = GameRules::getInstance()->calcLowestPoker(cardsForWaitOut, _pokerValueType);
	//auto _lastOutCard = OutCards::create(this, _pokerValueType, cardsForWaitOut.size(), _lowestPoker);
	//_lastOutCard->retain(); 		/* 防止被内存管理器回收 */
	auto _lastOutCard = createLastOutCard(cardsForWaitOut);
	CC_ASSERT(_lastOutCard != nullptr);
	this->updateLastOutCards(_lastOutCard);

	this->deleteOutcardInScene();	/* 在将cardInScene存放新的扑克时，先将以前的在Scene的扑克删除 */

	this->setOutcardInScene(cardsForWaitOut);/* 将出的牌放到出牌的容器里，待在出牌区域显示 */

	for (int i = 0; i < cardsForWaitOut.size(); ++i){
		cardsForWaitOut.at(i)->removeFromParent();
		this->removeCard(cardsForWaitOut.at(i));
		this->updateCardPos();
	}
	cardsForWaitOut.clear();	/* 待出的牌出了，就要将待出牌容器清空，不然和下次的重复在一起会导致错误 */

	this->showOutcardInScene();
	this->playOutCardInSceneMusic();	/* 播放出牌音乐 */

	/* 如果玩家已经出完牌，游戏结束 */
	if (this->getPoker().size() == 0){
		this->setGameOver();
		return;
	}

	this->stopCountDown();
	this->setLastOutCardNull();	/* 出牌后必须设置lastOutcard为空，不然游戏逻辑会出错 */
	this->updateOutOrder();	/* 更新出牌顺序 */
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
/*                             观察者                                         */
/************************************************************************/
void HolderPlayer::addCardForWaitOut(Ref* _data){
	auto _poker = dynamic_cast<Poker*>(_data);	/* 转化为Poker*类型 */
	CC_ASSERT(_poker != nullptr);
	cardsForWaitOut.pushBack(_poker);
	GlobalFunc::sort(cardsForWaitOut);	/* 插入后进行排序 */
}

void HolderPlayer::deleteCardForWaitOut(Ref* _data){
	auto _poker = dynamic_cast<Poker*>(_data);
	CC_ASSERT(_poker != nullptr);
	cardsForWaitOut.eraseObject(_poker);
	/* 删除牌不需要重新排序 */
}

void HolderPlayer::updateOutState(Ref*){
	this->updateOutState();
}