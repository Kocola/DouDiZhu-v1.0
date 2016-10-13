#ifndef __POKER_VALUE_TYPE_H__
#define __POKER_VALUE_TYPE_H__

enum PokerValueType{
	NONE, /* 什么都不是 */
	SINGLE, /* 单张 */
	PAIR,	/* 对子 */
	TRIPLE,	/* 三张 */
	TRIPLEWITHSINGLE, /* 三带一 */
	TRIPLEWITHPAIR, /* 三带二 */
	BOMB,	/* 炸弹 */
	KINGBOMB,	/* 王炸 */
	STRAIGHT,	/* 顺子 */
	PAIRSRAIGHT,	/* 双顺 */
	TRIPLESTRAIGHT	/* 三顺 */
};

#endif