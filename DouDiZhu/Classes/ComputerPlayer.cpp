#include "ComputerPlayer.h"
#include "GameRules.h"
#include "OutCards.h"

void ComputerPlayer::outCard(OutCards* _lastOutcards,
	std::function<void(OutCards*)>& _updateOutcardsCallback, std::function<void>& _updateOutcardOrderCallback){
	Vector<Poker*> _pokers = this->searchOutCard(_lastOutcards);/* 根据游戏上一手牌查找符合要求的牌 */

	if (_pokers.size() == 0){
		return;
	}else{
		/* 以当前的牌更新游戏的上一手牌 */
		_lastOutcards = OutCards::create(this, GameRules::getInstance()->analysePokerValueType(_pokers), _pokers.size(), _pokers.at(_pokers.size() - 1));
		_lastOutcards->retain();
		/* 将出的牌从当前玩家的扑克中删除 */
		for (int i = 0; i < _pokers.size(); ++i){
			_pokers.at(i)->removeFromParent();
			this->removeCard(_pokers.at(i));
		}
		/* 将要出的牌在出牌区显示出来 */
		this->deleteOutcardInScene();
		this->setOutcardInScene(_pokers);
		this->showOutcardInScene();
	}
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