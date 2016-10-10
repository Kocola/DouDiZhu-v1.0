#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "GlobalFunc.h"
#include "Poker.h"

using namespace cocos2d;

class CountDown;
class HeadImage;
class OutCards;
class PlayerOrder;

class Player : public Node{
public:
	static Player* create(PlayerPosType _playerPosType);
	virtual bool init(PlayerPosType _playerPosType);
	/* 具体的一些初始化操作 */
private:
	void initHeadImage();
	void initPlayerOrder();
	void initCountDown();
	/* 位置初始化信息 */
private:
	void initHeadImagePos();
	void initOutcardPos();	/* 根据玩家的位置信息（左，中，右）来计算扑克的位置相关信息，开始，中点等 */
	void initCountDownPos();	/* 根据的位置信息（左，中，右），计算倒计时器的X轴坐标 */
public:
	void insertCards(const Vector<Poker*>& _pokers);
	void removeCard(Poker* poker);
	void removeAllCardsFromParent();
public:
	void startCountDown(float _totalCount = 15, const std::function<void(void)>& _callback = [](){});
	void stopCountDown();
	void setHeadImagePos(Point _pointInWorld);	/* 设置头像的位置，传入的坐标必须是在世界坐标系下的坐标，不然会出错 */
public:
	bool isReady() const { return ready; }
	//void setPlayerPosType(PlayerPosType _playerPos) { this->playerPosType = _playerPos; }
public:
	void setOutcardInScene(const Vector<Poker*>& _pokers);
	void deleteOutcardInScene();
	void showOutcardInScene();
protected:
	void updateCallLandlordState();
	void updateCallLandlordOrder();
	void updateOutOrder();/* 更新出牌顺序，这个函数会用于发送观察者消息 */
	void setGameOver();
	void setGameStateChooseLandlord();
	void updateLastOutCards(OutCards* lastOutcard);
	void playOutCardInSceneMusic();
public:
	void updateHeadImage();	/* 更新头像 */
public:
	/* 更新扑克的位置，供子类重写 */
	virtual void outCard(OutCards* _lastOutcards){ log("outCard Empty"); }
	virtual void updateCardPos() { log("UpdateCardPos"); }
	virtual void callLandlord(){ log("CallLandlord!"); }
	virtual void setReady(bool _ready);
	/* 变更结构添加的函数：获取（Get） */
protected:
	PlayerOrder* getPlayerOrder() const { return this->playerOrder; }
	/*GameScene* getGameScene() const { return this->gameScene; }*/
	PlayerPosType getPlayerPosType() const { return this->playerPosType; }
	/* 变更结构添加的函数：获取（Set） */
public:
	/* 设置命令状态是否可见 */
	void setOrderVisible(bool _isVisible);
private:
	CC_SYNTHESIZE(bool, isLandlord, Landlord);		/* 是否是地主 */
	CC_SYNTHESIZE(bool, isCall, Call);		/* 是否已经叫地主 */
	CC_SYNTHESIZE(Vector<Poker*>, pokers, Poker);		/* 手里有的扑克牌 */
	CC_SYNTHESIZE(PlayerType, playerType, PlayerType);		/* 玩家类型 */
	CC_SYNTHESIZE(bool, isOutPoker, IsOutPoker);	/* 是否出牌 */
	CC_SYNTHESIZE(int, callLandlordScore, CallLandlordScore);	/* 设置叫地主分数 */
protected:
	void displayCard(const Vector<Poker*>& _pokers, float _displayMaxWidth,
		float _displayStartX, float _displayMiddleX, float _displayY, bool isAdded, const float SCALE = 1.0);
private:
	bool ready;	/* 是否准备开始游戏 */
private:
	PlayerPosType playerPosType;	/* 玩家所在的位置：左边，右边，中间？ */
private:
	HeadImage* headImage;		/* 头像对象 */
	/* 头像的X，Y坐标，相对于Player的坐标*/
	float headImagePosX;
	float headImagePosY;
private:
	float displayOutcardStartX;  /* 出牌的起始位置X坐标 */
	float displayOutcardY;	/* 出的牌的Y坐标（相对于头像） */
	float displayOutcardMiddleX;		/* 出牌中间点的X坐标值 */
	float displayOutcardMaxWidth;	/* 显示出牌的最大宽度 */
	Vector<Poker*> cardsInScene;	/* 显示在桌面上已出的牌 */
private:
	float displayTimerStartX;	/* 倒计时器中点的X坐标，Y坐标和头像中间对齐 */
	CountDown* countDown;
private:
	PlayerOrder* playerOrder;
//private:
//	GameScene* gameScene;		/* 更新叫地主顺序，出牌顺序等必需的 */
};

#endif