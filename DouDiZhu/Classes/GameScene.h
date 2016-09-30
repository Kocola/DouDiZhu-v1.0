#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Poker.h"

using namespace cocos2d;

class Player;
class GameRules;
class OutCards;
class HeadImage;
class PlayerOrder;

class GameScene : public Layer{
public:
	GameScene();
	~GameScene();
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameScene);

	virtual void update(float delta);
private:
	bool initBackground();	/* ��ʼ������ */
	bool initPlayer();	/* ��ʼ����� */
	bool initPoker();	/* ��ʼ���˿� */
	bool shuffleCards();	/* ϴ�Ƴ��� */
	bool initButton();	/* ������ť */
	bool initHeadImage();	/* ��ʼ��ͷ�񣬰���ͷ���ͷ��� */
	bool initPlayerOrder();	/* ��ʼ���������״̬����ʼ��ȫ��Ϊ ׼�� */
	bool initCallLandlord();	/* ��ʼ���е��� */
private:
	/* ׼�� */
	void ready();
	bool checkAllReady();	/* ����������Ƿ�׼���� */
	/* ���� */
	void dealCard();	/* ���� */
	//void dealCard(Player* _player, Vector<Poker*>& _pokers, bool displayFront = false);/*Player::insertCards��� */
	void initLandlordCard();	/* ��ʼ�����ڵ����������� */
	void displayLandlordCard();	/*��ĳ��λ����ʾ���ڵ����������� */
	/* �е��� */
	void setCallLandlordOrderState(Player* _player, int _score);	/* ���ýе���ʱ��ҵ�����״̬ */
	void computerCallLandlord(Player* _computer);
	void playerCallLandlord();	/* �ֶ���ҽе��� */
	void callLandlord();	/* �е���ģ�� */
	/* ѡ����� */
	void chooseLandlord();
	void outLandlordCard();	/* �������ڵ����������� */
	/* ���� */
	void initOutCardOrder();
	Vector<Poker*> searchOutCard(Player* _player);	/* ����ϼҵĳ��ƣ�����Ƿ����ܹ������ƣ������ܳ����Ƶļ��� */
	void outCardInOrder(float delta);
	void outCardForPlayer(Player* _player);	/* �ֵ���ҳ��� */
	void outCardForComputer(Player* _computer);	/* �ֵ����Գ��� */
	void outCardForLandlord();	/* ���������� */
private:
	/* �ص����� */
	void start_callback(Ref*);

	void pass_callback(Ref*);
	void hint_callback(Ref*);
	void out_callback(Ref*);

	void nocall_callback(Ref*);
	void callone_callback(Ref*);
	void calltwo_callback(Ref*);
	void callthree_callback(Ref*);
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
	/* ���ؽзְ�ť */
	void setCallLandlordButtonUnVisible();
	/* �����������״̬ */
	void setPlayerOrderStateUnVisible();
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
	void runWinAnimation();
	void runLostAnimation();

	void updateHeadImage(); /* ����ͷ�� */

	void deleteCardInTop();		/* ɾ����ʾ����Ļ�������˿� */
private:
	/* ���½зֵ�ID��ʹ��һ���˽з֣������ǰ˳����4����ô������������׶� */
	void updateCallLandlordOrder();
	/* �ж���߷֣�ʹ�õ�ĳ����Һ�����߷�ʱ������ֹ�з� */
	void isMaxCallLandlordScore(Player* player);
	/* ���Զ˽зֳ��� */
	int automaticCallLandlord();
private:
	Player* player;	/* �ֶ���� */
	HeadImage* playerHeadImage;		/* �ֶ����ͷ�� */
	PlayerOrder* playerOrder;	/* �ֶ���ҵĵ�ǰ����״̬������׼�����������зֵ� */
	Player* computerPlayer_one;	/* �������1 */
	HeadImage* computerPlayer_one_headImage;		/* �������1ͷ�� */
	PlayerOrder* computerPlayer_one_order;	/* �������1����״̬ */
	Player* computerPlayer_two;	/* �������2 */
	HeadImage* computerPlayer_two_headImage;		/* �������2ͷ�� */
	PlayerOrder* computerPlayer_two_order;	/* �������2����״̬ */

	Player* landlordPlayer;	/* �������ָ�� */

	Vector<Player*> players;	/* ��ŵ�ǰ���е���λ��ң����е�һ���ǵ��� */
	int outcardOrder; /* ��ǰӦ�ó��Ƶ���ң���playersЭ�� */
	Vector<Poker*> pokers;	/* �˿� */
	Vector<Poker*> hintPokers;		/* ��ʾҪ������ */
	int callLandlordOrder;	/* �е���˳�� */
private:
	MenuItemSprite* pass;	/* ������ť */
	MenuItemSprite* hint;	/* ��ʾ��ť */
	MenuItemSprite* out;	/* ���ư�ť */

	MenuItemSprite* nocall;	/* ���а�ť */
	MenuItemSprite* call_one;	/* ��һ�� */
	MenuItemSprite* call_two;	/* �ж��� */
	MenuItemSprite* call_three;	/* ������ */

	MenuItemSprite* btn_start;	/* ��ʼ��Ϸ��ť */
private:
	GAMESTATE gameState;
	OutCards* lastOutCards;	/* ��һ�εĳ�����Ϣ */
	Vector<Poker*> cardsInScene;	/* ����Scene���Ƽ��� */
	Vector<Poker*> cardForLandlord;	 /* ���ڵ����Ķ��������� */
	Vector<Poker*> cardDisplayInTop;	/* ��ʾ����Ļ���������ڵ����������� */
	Sprite* winSprite;
	Sprite* lostSprite;
	Sprite* passHint;	/* û���ƴ�ù��ϼҵ���ʾ */
};

#endif