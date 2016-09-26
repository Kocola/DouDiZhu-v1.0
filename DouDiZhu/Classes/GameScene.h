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
	bool initButton();	/* �������ƺͲ�����ť */
private:
	void dealCard();	/* ���� */
	void dealCard(Player* _player, Vector<Poker*>& _pokers, bool displayFront = false);
	void initOutCard();
	void outCardInOrder(float delta);
	void outCardForPlayer(Player* _player);	/* �ֵ���ҳ��� */
	void outCardForComputer(Player* _computer);	/* �ֵ����Գ��� */
	void test();	/* ����ģ�� */
private:
	/* �ص����� */
	void pass_callback(Ref*);
	void out_callback(Ref*);
public:
	CC_SYNTHESIZE(Vector<Poker*>, arrWaitPlayOut, ArrWaitPlayOut);
public:
	void addWaitPlayOut(Poker* poker) { this->arrWaitPlayOut.pushBack(poker); }
	void deleteWaitPlayOut(Poker* poker) { this->arrWaitPlayOut.eraseObject(poker); }

	void sort();	/* ���˿˽������� */
	/* ���³��ư�ť��״̬���ú�����Poker::updateOutState���� */
	void updateOutState();
	/* ���³��ư�ť��״̬*/
	void setOutState(bool state){ this->out->setEnabled(state); }
public:
	/* ���ƣ����Ʒ��ڳ����� */
	void outCardInScene();
	/* ��ǰ��ҳ���ʱ��ɾ�����ڳ����е���һ���˿� */
	void deleteCardInScene();
	/* ��Ϸ��ʼ���������¿�ʼ */
	void gameStart(float delta);
	/* ��Ϸ���� */
	void gameOver();
public:
	/* �ֶ���ң�����ֵ���ǳ����豸������Լ� */
	Player* getManualPlayer() const { return player; }
	/* ��ǰ�������Ƿ��ǳ��е�ǰ�豸����ң����ɰ����ƺͲ�����ť����� */
	bool isCurAndManualPlayer() const;
private:
	/* discarded */
	//bool cmp_sort(const Poker* a, const Poker* b);		/* �˿����������ĺ��� */
	void updatePokerPosAndRemovePoker(Ref* data);
	int randomInt(int begin, int end);
	//void Knuth_Durstenfeld_Shuffle();		/* ϴ���㷨 */
private:
	Player* player;	/* �ֶ���� */
	Player* computerPlayer_one;	/* �������1 */
	Player* computerPlayer_two;	/* �������2 */
	Vector<Player*> players;	/* ��ŵ�ǰ���е���λ��ң����е�һ���ǵ��� */
	int order; /* ��ǰӦ�ó��Ƶ���ң���playersЭ�� */
	Vector<Poker*> pokers;	/* �˿� */
	MenuItemSprite* pass;	/* ������ť */
	MenuItemSprite* out;	/* ���ư�ť */
private:
	GAMESTATE gameState;
	OutCards* lastOutCards;	/* ��һ�εĳ�����Ϣ */
	Vector<Poker*> cardsInScene;	/* ����Scene���Ƽ��� */
};

#endif