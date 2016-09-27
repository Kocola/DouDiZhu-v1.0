#ifndef __GLOBAL_DEFINE_H__
#define __GLOBAL_DEFINE_H__

#define SINGLETYPECARDNUM 13	/* 每种类型的牌个数 */
#define DISPLAYCARDHEIGHT 10	/* 玩家卡牌显示高于屏幕底部的高度 */
#define MIMIUM_CARDS_OVERLAPWIDTH 20	/* 卡牌之间最小的重叠长度 */
#define TOTAL_POKER_NUM 54	/* 扑克牌的总个数 */
#define POKER_WIDTH 108	/* 扑克宽度 */
#define POKER_HEIGHT 149	/* 扑克高度 */
#define HEIGHTDISTANCE_HEADIMAGEANDPLAYER 0		/* 玩家和头像之间高度 */

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

enum GAMESTATE{
	DEAL,	/* 发牌 */
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

#endif