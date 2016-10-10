#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "Poker.h"

using namespace cocos2d;

class ComputerPlayer;
class GameRules;
class OutCards;
class HeadImage;
class HolderPlayer;
class PlayerOrder;

class GameScene : public Layer{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);
public:
	virtual void update(float delta);
private:
	void initObserver();	/* 初始化观察者 */
private:
	void initBackground();	/* 初始化背景 */
private:
	/************************************************************************/
	/*                                   游戏开始                                                 */
	/************************************************************************/
	void initPlayer();	/* 初始化玩家 */
	void initPoker();	/* 初始化扑克 */
	void initCallLandlord() { this->callLandlordOrder = 0; }	/* 初始化叫地主 */
	void gameStart(float delta);/* 游戏开始，或者重新开始 */
	void gameStart();
private:
	/************************************************************************/
	/*                                 游戏运行                                                           */
	/************************************************************************/
	/*                                     准备                                                              */
	void ready();
	bool checkAllReady();	/* 检查所有人是否都准备好 */
	/*                                    发牌                                                               */
	void dealCard();	/* 发牌 */
	void initLandlordCard();	/* 初始化属于地主的三张牌 */
	void displayLandlordCard();	/*在某个位置显示属于地主的三张牌 */
	/*                                  叫地主                                                              */
	void callLandlord();	/* 叫地主模块 */
	/*                                选择地主                                                             */
	void chooseLandlord();
	void updateHeadImage();
	void outCardForLandlord();	/* 发放属于地主的三张牌 */
	/*                                  出牌                                                                 */
	void initOutCardOrder();	/* 初始化第一个出牌的是Players的第0个 */
	void initPlayerVector();	/* 初始化玩家容器，这是为了通过outCardOrder控制出牌， 便于管理*/
	void initLastOutCard();	/* 初始化上一手牌 */
	void outCardInOrder(float delta);
public:
	void outCardInSceneMusic();/* 播放出牌在场景上时的音效 */
	void deleteCardInScene();/* 当前玩家出牌时，删除留在场景中的上一手扑克 */
	
private:
	/************************************************************************/
	/*                                        游戏结束                                                    */
	/************************************************************************/
	bool isWin();	/* 判断玩家是胜利还是输 */
	void deletePlayer();	/* 游戏结束时，清理Player资源 */
	void deleteCardInTop();		/* 删除显示在屏幕顶部的扑克 */
	void gameOver();/* 游戏结束 */
private:
	/************************************************************************/
	/*                                        游戏动画                                                    */
	/************************************************************************/
	void runWinAnimation();
	void runLostAnimation();
public:
	/************************************************************************/
	/*                                       获取和设置状态                                          */
	/************************************************************************/
	/* 获取当前游戏状态 */
	GAMESTATE getGameState() const { return this->gameState; }
	void setGameState(GAMESTATE _gameState) { this->gameState = _gameState; }
	/* 获取当前的游戏玩家 */
	Player* getCurPlayer() const { CC_ASSERT(players.size() != 0); return players.at(outcardOrder); }
	/* 获取上一手扑克 */
	OutCards* getLastOutCard() const { CC_ASSERT(lastOutCard != nullptr); return this->lastOutCard; }
	void setPlayerOrderStateUnVisible();
public:
	/************************************************************************/
	/*                              观察者回调函数                                        */
	/************************************************************************/
	/* 更新叫分的ID，使下一个人叫分，如果当前顺序是4，那么进入决定地主阶段 */
	void updateCallLandlordOrder(Ref*);
	/* 更新出牌的顺序 */
	void updateOutCardOrder(Ref*);
	/* 更新桌面上一手牌 */
	void updateLastOutCards(Ref* _data);
	/* 设置游戏结束的状态 */
	void setGameOver(Ref*);
	/* 设置游戏状态为选择地主的状态 */
	void setGameStateChooseLandlord(Ref*);
	/* 播放出牌在桌面上的对应音乐 */
	void playOutCardInSceneMusic(Ref*);
	/************************************************************************/
	/*                                成员变量                                      */
	/************************************************************************/
private:
	HolderPlayer* player;	/* 手动玩家 */
	ComputerPlayer* computerPlayer_one;	/* 电脑玩家1 */
	ComputerPlayer* computerPlayer_two;	/* 电脑玩家2 */

	Player* landlordPlayer;	/* 保存地主指针 */
	Vector<Player*> players;	/* 存放当前所有的三位玩家，其中第一个是地主 */
private:
	int outcardOrder; /* 当前应该出牌的玩家，和players协作 */
	int callLandlordOrder;	/* 叫地主顺序 */
	GAMESTATE gameState;
private:
	Vector<Poker*> pokers;	/* 扑克 */
	OutCards* lastOutCard;	/* 上一次的出牌信息 */
	Vector<Poker*> cardForLandlord;	 /* 属于地主的多余三张牌 */
	Vector<Poker*> cardDisplayInTop;	/* 显示在屏幕顶部的属于地主的三张牌 */
private:
	Sprite* winSprite;
	Sprite* lostSprite;
};

#endif