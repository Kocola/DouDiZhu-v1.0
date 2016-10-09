#ifndef __HOLDER_PLAYER_H__
#define __HOLDER_PLAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

#include "Player.h"	/* �̳�һ����ʱ,���̳е��಻����ǰ������ */

class OutCards;

class HolderPlayer : public Player{
public:
	void outCard(OutCards* _lastOutcards, std::function<void(OutCards*)>& _updateOutcardsCallback, std::function<void>& _updateOutcardOrderCallback);
private:
	Vector<Poker*> HolderPlayer::searchHintCard(OutCards* _lastOutcards);
};

#endif