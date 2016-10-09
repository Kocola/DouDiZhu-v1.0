#include "ComputerPlayer.h"
#include "GameRules.h"
#include "OutCards.h"

void ComputerPlayer::outCard(OutCards* _lastOutcards,
	std::function<void(OutCards*)>& _updateOutcardsCallback, std::function<void>& _updateOutcardOrderCallback){
	Vector<Poker*> _pokers = this->searchOutCard(_lastOutcards);/* ������Ϸ��һ���Ʋ��ҷ���Ҫ����� */

	if (_pokers.size() == 0){
		return;
	}else{
		/* �Ե�ǰ���Ƹ�����Ϸ����һ���� */
		_lastOutcards = OutCards::create(this, GameRules::getInstance()->analysePokerValueType(_pokers), _pokers.size(), _pokers.at(_pokers.size() - 1));
		_lastOutcards->retain();
		/* �������ƴӵ�ǰ��ҵ��˿���ɾ�� */
		for (int i = 0; i < _pokers.size(); ++i){
			_pokers.at(i)->removeFromParent();
			this->removeCard(_pokers.at(i));
		}
		/* ��Ҫ�������ڳ�������ʾ���� */
		this->deleteOutcardInScene();
		this->setOutcardInScene(_pokers);
		this->showOutcardInScene();
	}
}

Vector<Poker*> ComputerPlayer::searchOutCard(OutCards* _lastOutcards){
	Vector<Poker*> _pokers;
	/* �����һ����Ҳ���Լ��� */
	if (_lastOutcards->getPokerOwner() == this || _lastOutcards->getPokerValueType() == NONE){
		//_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), SINGLE, nullptr);	/* ����д�ᵼ�µ������Ҳ������ź�һֱ��������ط� */ 
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

				if (_pokers.size() == 0){ /* ����Ҳ�����Ӧ���ƣ�����ը�� */
					_pokers = GameRules::getInstance()->calcPokerWithValueType(this->getPoker(), BOMB, nullptr);
					if (_pokers.size() == 0){	/* ����Ҳ�����ͨ��ը��������ը */
						_pokers = GameRules::getInstance()->calcPokerWithValueType(this->getPoker(), KINGBOMB);
					}
				}
			}
		}
	}
	return _pokers;
}