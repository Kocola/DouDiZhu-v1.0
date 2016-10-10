#ifndef __COMPUTER_PLAYER_H__
#define __COMPUTER_PLAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

#include "Player.h"

class OutCards;

class ComputerPlayer : public Player{
public:
	static ComputerPlayer* create(PlayerPosType _playerPosType);
	virtual bool init(PlayerPosType _playerPosType);
public:
	virtual void outCard(OutCards* _lastOutcards) override;
	virtual void callLandlord() override;
private:
	Vector<Poker*> searchOutCard(OutCards* lastOutcards);
	/* µçÄÔ¶Ë½Ð·Ö³ÌÐò */
	int automaticCallLandlord();
};

#endif