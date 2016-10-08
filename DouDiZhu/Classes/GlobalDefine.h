#ifndef __GLOBAL_DEFINE_H__
#define __GLOBAL_DEFINE_H__

#define SINGLETYPECARDNUM 13	/* 每种类型的牌个数 */
#define DISPLAYCARDHEIGHT 10	/* 玩家卡牌显示高于屏幕底部的高度 */
#define DISPLAYCARDINTOP_INTERVAL 10	/* 属于地主的卡牌显示在低于屏幕顶部的距离 */
#define MIMIUM_CARDS_OVERLAPWIDTH 20	/* 卡牌之间最小的重叠长度 */
#define TOTAL_POKER_NUM 54	/* 扑克牌的总个数 */
#define POKER_WIDTH 108	/* 扑克宽度 */
#define POKER_HEIGHT 149	/* 扑克高度 */
#define HEIGHTDISTANCE_HEADIMAGEANDPLAYER 0		/* 玩家和头像之间高度 */
#define BUTTON_INTERVAL 20		/* 按钮之间的间隙 */

enum PokerType{
	DIAMOND = 0,		/* 方块 */
	CLUB,		/* 梅花 */
	HEART,		/* 红桃 */
	SPADE = 3,		/* 黑桃 */
	BLACKJOKER = 52,	/* 小王 */
	REDJOKER = 53,		/* 大王 */
	BACK = 54	/* 扑克背面 */
};

enum PlayerType{
	PLAYER,	/* 人 */
	COMPUTER	/* 电脑 */
};

//enum CallLandlordOrder{
//	PLAYER_CALL,	/* 手动玩家 */
//	COMPUTER_ONE_CALL,	/* 电脑玩家1 */
//	COMPUTER_TWO_CALL	/* 电脑玩家2 */
//};

enum GAMESTATE{
	READY,  /* 准备 */
	DEAL,	/* 发牌 */
	CALLLANDLORD,  /* 叫地主 */
	CHOOSELANDLORD,	/* 决定地主，并且多的三张牌发给地主 */
	OUTCARD, /* 出牌 */
	END /* 结束 */
};

enum HeadImageType{	/* 头像类型 */
	LANDLORD,	/* 地主 */
	FARMER	/* 农民 */
};

enum HeadImageOrientation{	/* 头像朝向 */
	LEFT, /* 向左 */
	RIGHT  /* 向右 */
};

enum PlayerSexuality{
	MALE,		/* 男性 */
	FEMALE	/* 女性 */
};

//enum CallLandlordScore{
//	ZERO,	/* 零分 */
//	ONE,		/* 一分 */
//	TWO,  /* 两分 */
//	THREE		/* 三分 */
//};

enum PlayerOrderState{
	ORDERREADY,		/* 准备 */
	NOCALL,	/* 不叫 */
	CALLONE,/* 一分 */
	CALLTWO,/* 二分 */
	CALLTHREE, /* 三分 */
	PASS, /* 不出牌 */
};

/* 玩家命令状态的位置：左边Or右边 */
enum PlayerOrderPos{
	ORDERINLEFT,		/* 左边的 */
	ORDERINRIGHT	/* 右边的 */
};

enum PlayerPosType{
	PLAYERINLEFT,	/* 左边 */
	PLAYERINMIDDLE,	/* 中间 */
	PLAYERINRIGHT	/* 右边 */
};

#endif