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
	bool initButton();	/* �������ƺͲ�����ť */
private:
	void dealCard();	/* ���� */
	void dealCard(Player* _player, Vector<Poker*>& _pokers, bool displayFront = false);
	void outCard(int order);
public:
	CC_SYNTHESIZE(Vector<Poker*>, arrWaitPlayOut, ArrWaitPlayOut);
public:
	void addWaitPlayOut(Poker* poker) { this->arrWaitPlayOut.pushBack(poker); }
	void deleteWaitPlayOut(Poker* poker) { this->arrWaitPlayOut.eraseObject(poker); }

	void sort();	/* ���˿˽������� */
public:
	Player* getPlayer() const { return player; }
private:
	/* discarded */
	//bool cmp_sort(const Poker* a, const Poker* b);		/* �˿����������ĺ��� */
	void updatePokerPos(Ref* data);
	int randomInt(int begin, int end);
	//void Knuth_Durstenfeld_Shuffle();		/* ϴ���㷨 */
private:
	Player* player;	/* �ֶ���� */
	Player* computerPlayer_one;	/* �������1 */
	Player* computerPlayer_two;	/* �������2 */
	Vector<Poker*> pokers;	/* �˿� */
	MenuItemSprite* pass;	/* ������ť */
	MenuItemSprite* out;	/* ���ư�ť */
private:
	GAMESTATE gameState;
};

#endif