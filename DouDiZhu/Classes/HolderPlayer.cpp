#include "GameRules.h"
#include "HolderPlayer.h"
#include "OutCards.h"

void HolderPlayer::outCard(OutCards* _lastOutcards, std::function<void(OutCards*)>& _updateOutcardsCallback, std::function<void>& _updateOutcardOrderCallback){

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