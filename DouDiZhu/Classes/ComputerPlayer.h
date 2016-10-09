#ifndef __COMPUTER_PLAYER_H__
#define __COMPUTER_PLAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

#include "Player.h"

class OutCards;

class ComputerPlayer : public Player{
public:
	void outCard(OutCards* _lastOutcards, 
		std::function<void(OutCards*)>& _updateOutcardsCallback, std::function<void>& _updateOutcardOrderCallback);
private:
	Vector<Poker*> searchOutCard(OutCards* lastOutcards);
};

#endif