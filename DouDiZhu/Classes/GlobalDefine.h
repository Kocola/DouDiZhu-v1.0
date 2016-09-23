#ifndef __GLOBAL_DEFINE_H__
#define __GLOBAL_DEFINE_H__

#define SINGLETYPECARDNUM 13	/* 每种类型的牌个数 */
#define DISPLAYCARDHEIGHT 10	/* 玩家卡牌显示高于屏幕底部的高度 */
#define MIMIUM_CARDS_OVERLAPWIDTH 20	/* 卡牌之间最小的重叠长度 */
#define TOTAL_POKER_NUM 54	/* 扑克牌的总个数 */
#define POKER_WIDTH 108	/* 扑克宽度 */
#define POKER_HEIGHT 149	/* 扑克高度 */

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
	WIN,		/* 胜利 */
	LOSE,   /* 输了 */
	END /* 测试用 */
};

#endif