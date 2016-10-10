#include "ComputerPlayer.h"
#include "GameRules.h"
#include "MusicController.h"
#include "OutCards.h"
#include "PlayerOrder.h"

ComputerPlayer* ComputerPlayer::create(PlayerPosType _playerPosType){
	auto pRet = new(std::nothrow)ComputerPlayer();
	if (pRet && pRet->init(_playerPosType)){
		pRet->autorelease();
		return pRet;
	}else{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool ComputerPlayer::init(PlayerPosType _playerPosType){
	if (!Player::init(_playerPosType)) return false;
	return true;
}


int ComputerPlayer::automaticCallLandlord(){
	srand((unsigned)time(0));	/* 开启随机种子 */
	int score = rand() % 4;	/* 叫分一共有不叫，1，2，3四种情况，其中0表示不叫 */
	return score;
}

void ComputerPlayer::callLandlord(){
	int score = this->automaticCallLandlord();
	this->setCallLandlordScore(score);
	this->updateCallLandlordState();
	if (score == MAXCALLLANDLORDSCORE){
		this->setGameStateChooseLandlord();
	}else{
		this->updateCallLandlordOrder();	/* 更新叫下一个叫地主的ID */
	}
}

void ComputerPlayer::outCard(OutCards* _lastOutcards){

	this->deleteOutcardInScene();

	Vector<Poker*> _pokers = this->searchOutCard(_lastOutcards);/* 根据游戏上一手牌查找符合要求的牌 */

	auto _playerOrder = this->getPlayerOrder();

	/* 随机出牌时间是 3 ~ 8 */
	this->startCountDown(rand() % 5 + 3, [=]() mutable{/* 存在临时变量，不可以用& */
		if (_pokers.size() == 0){
			_playerOrder->setPlayerOrderState(PASS);	/* 显示不出的状态 */
			_playerOrder->setVisible(true);

			/* 播放不出牌的音效 */
			MusicController::getInstance()->playPassEffect();
		}
		else{
			/* 以当前的牌更新游戏的上一手牌 */
			_lastOutcards = OutCards::create(this, GameRules::getInstance()->analysePokerValueType(_pokers), _pokers.size(), _pokers.at(_pokers.size() - 1));
			_lastOutcards->retain();
			this->updateLastOutCards(_lastOutcards);
			/* 将出的牌从当前玩家的扑克中删除 */
			for (int i = 0; i < _pokers.size(); ++i){
				_pokers.at(i)->removeFromParent();
				this->removeCard(_pokers.at(i));
			}
			/* 将要出的牌在出牌区显示出来 */
			/*this->deleteOutcardInScene();*/
			this->setOutcardInScene(_pokers);
			this->showOutcardInScene();
			this->playOutCardInSceneMusic();	/* 播放出牌音乐 */

			if (this->getPoker().size() == 0){
				this->setGameOver();
				return;
			}
		}
		this->updateOutOrder();	/* 更新出牌顺序，让下一个玩家出牌 */
	});
}

Vector<Poker*> ComputerPlayer::searchOutCard(OutCards* _lastOutcards){
	Vector<Poker*> _pokers;
	/* 如果上一手牌也是自己的 */
	if (_lastOutcards->getPokerOwner() == this || _lastOutcards->getPokerValueType() == NONE){
		//_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), SINGLE, nullptr);	/* 这样写会导致电脑在找不到单张后一直卡在这个地方 */ 
		_pokers = GameRules::getInstance()->searchProperPokers(this->getPoker());
	}
	else{
		PokerValueType _pokerValueType = _lastOutcards->getPokerValueType();
		if (_pokerValueType != KINGBOMB){
			if (_pokerValueType == BOMB){
				_pokers = GameRules::getInstance()->calcPokerWithValueType(this->getPoker(), BOMB, _lastOutcards->getLowestPoker());
			}
			else{
				_pokers = GameRules::getInstance()->calcPokerWithValueType(
					this->getPoker(), _lastOutcards->getPokerValueType(), _lastOutcards->getLowestPoker(), _lastOutcards->getTotalLength());

				if (_pokers.size() == 0){ /* 如果找不到对应的牌，就找炸弹 */
					_pokers = GameRules::getInstance()->calcPokerWithValueType(this->getPoker(), BOMB, nullptr);
					if (_pokers.size() == 0){	/* 如果找不到普通的炸，就找王炸 */
						_pokers = GameRules::getInstance()->calcPokerWithValueType(this->getPoker(), KINGBOMB);
					}
				}
			}
		}
	}
	return _pokers;
}