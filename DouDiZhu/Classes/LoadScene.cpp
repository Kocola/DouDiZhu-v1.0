#include "LoadScene.h"
#include "Lobby.h"

Scene* LoadScene::createScene(){
	auto scene = Scene::create();
	auto layer = LoadScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoadScene::init(){
	auto logoSprite = Sprite::create("Image/loading_logo.png");
	logoSprite->setPosition(Director::getInstance()->getVisibleSize() / 2);
	this->addChild(logoSprite, 1);

	auto fadeOut = FadeOut::create(3);	/* 创建FadeOut动作，执行该动作的节点会在3秒内淡出 */
	auto sequenceAction = Sequence::create(fadeOut, CallFunc::create([&](){gotoLobby(); }), nullptr);
	logoSprite->runAction(sequenceAction);
	
	return true;
}

void LoadScene::gotoLobby(){
	Director::getInstance()->replaceScene(Lobby::createScene());	/* 转入游戏场景 */
}