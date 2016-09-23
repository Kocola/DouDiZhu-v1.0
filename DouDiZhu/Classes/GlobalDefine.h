#ifndef __GLOBAL_DEFINE_H__
#define __GLOBAL_DEFINE_H__

#define SINGLETYPECARDNUM 13	/* ÿ�����͵��Ƹ��� */
#define DISPLAYCARDHEIGHT 10	/* ��ҿ�����ʾ������Ļ�ײ��ĸ߶� */
#define MIMIUM_CARDS_OVERLAPWIDTH 20	/* ����֮����С���ص����� */
#define TOTAL_POKER_NUM 54	/* �˿��Ƶ��ܸ��� */
#define POKER_WIDTH 108	/* �˿˿�� */
#define POKER_HEIGHT 149	/* �˿˸߶� */

enum PokerType{
	DIAMOND = 0,		/* ���� */
	CLUB,		/* ÷�� */
	HEART,		/* ���� */
	SPADE = 3,		/* ���� */
	BLACKJOKER = 52,	/* С�� */
	REDJOKER = 53,		/* ���� */
	BACK = 54	/* �˿˱��� */
};

enum PlayerType{
	PLAYER,	/* �� */
	COMPUTER	/* ���� */
};

enum GAMESTATE{
	DEAL,	/* ���� */
	OUTCARD, /* ���� */
	WIN,		/* ʤ�� */
	LOSE,   /* ���� */
	END /* ������ */
};

#endif