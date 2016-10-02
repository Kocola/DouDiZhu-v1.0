#ifndef __LOAD_SCENE_H__
#define __LOAD_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class LoadScene : public Layer{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(LoadScene);
public:
	void gotoLobby();	/* ×ªÈë´óÌü */
};

#endif