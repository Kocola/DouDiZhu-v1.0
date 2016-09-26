#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Poker.h"

using namespace cocos2d;

class Player;
class GameRules;
class OutCards;

class GameScene : public Layer{
public:
	GameScene();
	~GameScene();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);

	virtual void update(float delta);
private:
	bool initBackground();
	bool initPlayer();
	bool initPoker();
	bool shuffleCards();
	bool initButton();	/* 创建出牌和不出按钮 */
private:
	void dealCard();	/* 发牌 */
	void dealCard(Player* _player, Vector<Poker*>& _pokers, bool displayFront = false);
	void initOutCard();
	void outCardInOrder(float delta);
	void outCardForPlayer(Player* _player);	/* 轮到玩家出牌 */
	void outCardForComputer(Player* _computer);	/* 轮到电脑出牌 */
	void test();	/* 测试模块 */
private:
	/* 回调函数 */
	void pass_callback(Ref*);
	void out_callback(Ref*);
public:
	CC_SYNTHESIZE(Vector<Poker*>, arrWaitPlayOut, ArrWaitPlayOut);
public:
	void addWaitPlayOut(Poker* poker) { this->arrWaitPlayOut.pushBack(poker); }
	void deleteWaitPlayOut(Poker* poker) { this->arrWaitPlayOut.eraseObject(poker); }

	void sort();	/* 对扑克进行排序 */
	/* 更新出牌按钮的状态，该函数供Poker::updateOutState调用 */
	void updateOutState();
	/* 更新出牌按钮的状态*/
	void setOutState(bool state){ this->out->setEnabled(state); }
public:
	/* 出牌，将牌放在场景上 */
	void outCardInScene();
	/* 当前玩家出牌时，删除留在场景中的上一手扑克 */
	void deleteCardInScene();
	/* 游戏开始，或者重新开始 */
	void gameStart(float delta);
	/* 游戏结束 */
	void gameOver();
public:
	/* 手动玩家，这里值的是持有设备的玩家自己 */
	Player* getManualPlayer() const { return player; }
	/* 当前出牌者是否是持有当前设备的玩家，即可按出牌和不出按钮的玩家 */
	bool isCurAndManualPlayer() const;
private:
	/* discarded */
	//bool cmp_sort(const Poker* a, const Poker* b);		/* 扑克排序依赖的函数 */
	void updatePokerPosAndRemovePoker(Ref* data);
	int randomInt(int begin, int end);
	//void Knuth_Durstenfeld_Shuffle();		/* 洗牌算法 */
private:
	Player* player;	/* 手动玩家 */
	Player* computerPlayer_one;	/* 电脑玩家1 */
	Player* computerPlayer_two;	/* 电脑玩家2 */
	Vector<Player*> players;	/* 存放当前所有的三位玩家，其中第一个是地主 */
	int order; /* 当前应该出牌的玩家，和players协作 */
	Vector<Poker*> pokers;	/* 扑克 */
	MenuItemSprite* pass;	/* 不出按钮 */
	MenuItemSprite* out;	/* 出牌按钮 */
private:
	GAMESTATE gameState;
	OutCards* lastOutCards;	/* 上一次的出牌信息 */
	Vector<Poker*> cardsInScene;	/* 出在Scene的牌集合 */
};

#endif