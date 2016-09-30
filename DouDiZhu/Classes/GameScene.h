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
	bool initBackground();	/* 初始化背景 */
	bool initPlayer();	/* 初始化玩家 */
	bool initPoker();	/* 初始化扑克 */
	bool shuffleCards();	/* 洗牌程序 */
	bool initButton();	/* 创建按钮 */
	bool initHeadImage();	/* 初始化头像，包含头像和头像框 */
	bool initPlayerOrder();	/* 初始化玩家命令状态，初始化全部为 准备 */
	bool initCallLandlord();	/* 初始化叫地主 */
private:
	/* 准备 */
	void ready();
	bool checkAllReady();	/* 检查所有人是否都准备好 */
	/* 发牌 */
	void dealCard();	/* 发牌 */
	//void dealCard(Player* _player, Vector<Poker*>& _pokers, bool displayFront = false);/*Player::insertCards替代 */
	void initLandlordCard();	/* 初始化属于地主的三张牌 */
	void displayLandlordCard();	/*在某个位置显示属于地主的三张牌 */
	/* 叫地主 */
	void setCallLandlordOrderState(Player* _player, int _score);	/* 设置叫地主时玩家的命令状态 */
	void computerCallLandlord(Player* _computer);
	void playerCallLandlord();	/* 手动玩家叫地主 */
	void callLandlord();	/* 叫地主模块 */
	/* 选择地主 */
	void chooseLandlord();
	void outLandlordCard();	/* 发放属于地主的三张牌 */
	/* 出牌 */
	void initOutCardOrder();
	Vector<Poker*> searchOutCard(Player* _player);	/* 针对上家的出牌，检查是否有能够出的牌，返回能出的牌的集合 */
	void outCardInOrder(float delta);
	void outCardForPlayer(Player* _player);	/* 轮到玩家出牌 */
	void outCardForComputer(Player* _computer);	/* 轮到电脑出牌 */
	void outCardForLandlord();	/* 给地主出牌 */
private:
	/* 回调函数 */
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

	void sort();	/* 对扑克进行排序 */
	/* 更新出牌按钮的状态，该函数供Poker::updateOutState调用 */
	void updateOutState();
	/* 更新出牌按钮的状态*/
	void setOutState(bool state){ this->out->setEnabled(state); }
	/* 隐藏叫分按钮 */
	void setCallLandlordButtonUnVisible();
	/* 隐藏玩家命令状态 */
	void setPlayerOrderStateUnVisible();
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
	void runWinAnimation();
	void runLostAnimation();

	void updateHeadImage(); /* 更新头像 */

	void deleteCardInTop();		/* 删除显示在屏幕顶部的扑克 */
private:
	/* 更新叫分的ID，使下一个人叫分，如果当前顺序是4，那么进入决定地主阶段 */
	void updateCallLandlordOrder();
	/* 判断最高分，使得当某个玩家喊出最高分时可以终止叫分 */
	void isMaxCallLandlordScore(Player* player);
	/* 电脑端叫分程序 */
	int automaticCallLandlord();
private:
	Player* player;	/* 手动玩家 */
	HeadImage* playerHeadImage;		/* 手动玩家头像 */
	PlayerOrder* playerOrder;	/* 手动玩家的当前命令状态，包含准备，不出，叫分等 */
	Player* computerPlayer_one;	/* 电脑玩家1 */
	HeadImage* computerPlayer_one_headImage;		/* 电脑玩家1头像 */
	PlayerOrder* computerPlayer_one_order;	/* 电脑玩家1命令状态 */
	Player* computerPlayer_two;	/* 电脑玩家2 */
	HeadImage* computerPlayer_two_headImage;		/* 电脑玩家2头像 */
	PlayerOrder* computerPlayer_two_order;	/* 电脑玩家2命令状态 */

	Player* landlordPlayer;	/* 保存地主指针 */

	Vector<Player*> players;	/* 存放当前所有的三位玩家，其中第一个是地主 */
	int outcardOrder; /* 当前应该出牌的玩家，和players协作 */
	Vector<Poker*> pokers;	/* 扑克 */
	Vector<Poker*> hintPokers;		/* 提示要出的牌 */
	int callLandlordOrder;	/* 叫地主顺序 */
private:
	MenuItemSprite* pass;	/* 不出按钮 */
	MenuItemSprite* hint;	/* 提示按钮 */
	MenuItemSprite* out;	/* 出牌按钮 */

	MenuItemSprite* nocall;	/* 不叫按钮 */
	MenuItemSprite* call_one;	/* 叫一分 */
	MenuItemSprite* call_two;	/* 叫二分 */
	MenuItemSprite* call_three;	/* 叫三分 */

	MenuItemSprite* btn_start;	/* 开始游戏按钮 */
private:
	GAMESTATE gameState;
	OutCards* lastOutCards;	/* 上一次的出牌信息 */
	Vector<Poker*> cardsInScene;	/* 出在Scene的牌集合 */
	Vector<Poker*> cardForLandlord;	 /* 属于地主的多余三张牌 */
	Vector<Poker*> cardDisplayInTop;	/* 显示在屏幕顶部的属于地主的三张牌 */
	Sprite* winSprite;
	Sprite* lostSprite;
	Sprite* passHint;	/* 没有牌打得过上家的提示 */
};

#endif