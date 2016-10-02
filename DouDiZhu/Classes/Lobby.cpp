#include "GameScene.h"
#include "Lobby.h"

Scene* Lobby::createScene(){
	auto scene = Scene::create();
	auto layer = Lobby::create();
	scene->addChild(layer);
	return scene;
}

bool Lobby::init(){
	auto bgSprite = Sprite::create("Image/background.png");
	bgSprite->setPosition(Director::getInstance()->getVisibleSize() / 2);
	this->addChild(bgSprite, 0);

	auto _menu = Menu::create();
	_menu->setPosition(Point::ZERO);

	auto startGameSprite = Sprite::create("Image/go_game_menu.png");
	auto startGameSprite_selected = Sprite::create("Image/go_game_menu_selected.png");
	auto startGame = MenuItemSprite::create(startGameSprite, startGameSprite_selected, CC_CALLBACK_1(Lobby::startGame, this));
	startGame->setPosition(Director::getInstance()->getVisibleSize() / 2);

	_menu->addChild(startGame);
	
	this->addChild(_menu, 1);

	return true;
}

void Lobby::startGame(Ref*){
	Director::getInstance()->replaceScene(GameScene::createScene());	/* ×ªÈëÓÎÏ· */
}

