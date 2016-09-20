#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Poker.h"

using namespace cocos2d;

class Player;

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
	void outCard(int order);
public:
	CC_SYNTHESIZE(Vector<Poker*>, arrWaitPlayOut, ArrWaitPlayOut);
public:
	void addWaitPlayOut(Poker* poker) { this->arrWaitPlayOut.pushBack(poker); }
	void deleteWaitPlayOut(Poker* poker) { this->arrWaitPlayOut.eraseObject(poker); }

	void sort();	/* 对扑克进行排序 */
public:
	Player* getPlayer() const { return player; }
private:
	/* discarded */
	//bool cmp_sort(const Poker* a, const Poker* b);		/* 扑克排序依赖的函数 */
	void updatePokerPos(Ref* data);
	int randomInt(int begin, int end);
	//void Knuth_Durstenfeld_Shuffle();		/* 洗牌算法 */
private:
	Player* player;	/* 手动玩家 */
	Player* computerPlayer_one;	/* 电脑玩家1 */
	Player* computerPlayer_two;	/* 电脑玩家2 */
	Vector<Poker*> pokers;	/* 扑克 */
	MenuItemSprite* pass;	/* 不出按钮 */
	MenuItemSprite* out;	/* 出牌按钮 */
private:
	GAMESTATE gameState;
};

#endif