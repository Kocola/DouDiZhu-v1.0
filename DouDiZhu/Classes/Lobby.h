#ifndef __LOBBY_H__
#define __LOBBY_H__

#include "cocos2d.h"

using namespace cocos2d;

class Lobby : public Layer{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(Lobby);
public:
	void startGame(Ref*);
};

#endif