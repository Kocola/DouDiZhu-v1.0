#include "OutCards.h"

OutCards::OutCards(Player* _pokerOwner, PokerValueType _pokerValueType, size_t _totalLength, Poker* _lowestPoker){
	this->pokerOwner = _pokerOwner;
	this->pokerValueType = _pokerValueType;
	this->totalLength = _totalLength;
	this->lowestPoker = _lowestPoker;
}

OutCards* OutCards::create(Player* _pokerOwner, PokerValueType _pokerValueType, size_t _totalLength, Poker* _lowestPoker){
	auto outCards = new(std::nothrow) OutCards(_pokerOwner, _pokerValueType, _totalLength, _lowestPoker);
	if (outCards && outCards->init()){
		outCards->autorelease();
	} else{
		CC_SAFE_DELETE(outCards);
		return nullptr;
	}
	return outCards;
}

bool OutCards::init(){
	return true;
}