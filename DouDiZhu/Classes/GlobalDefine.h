#ifndef __GLOBAL_DEFINE_H__
#define __GLOBAL_DEFINE_H__

#define SINGLETYPECARDNUM 13	/* ÿ�����͵��Ƹ��� */
#define DISPLAYCARDHEIGHT 10	/* ��ҿ�����ʾ������Ļ�ײ��ĸ߶� */
#define DISPLAYCARDINTOP_INTERVAL 10	/* ���ڵ����Ŀ�����ʾ�ڵ�����Ļ�����ľ��� */
#define MIMIUM_CARDS_OVERLAPWIDTH 20	/* ����֮����С���ص����� */
#define TOTAL_POKER_NUM 54	/* �˿��Ƶ��ܸ��� */
#define POKER_WIDTH 108	/* �˿˿�� */
#define POKER_HEIGHT 149	/* �˿˸߶� */
#define HEIGHTDISTANCE_HEADIMAGEANDPLAYER 0		/* ��Һ�ͷ��֮��߶� */
#define BUTTON_INTERVAL 20		/* ��ť֮��ļ�϶ */

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

//enum CallLandlordOrder{
//	PLAYER_CALL,	/* �ֶ���� */
//	COMPUTER_ONE_CALL,	/* �������1 */
//	COMPUTER_TWO_CALL	/* �������2 */
//};

enum GAMESTATE{
	READY,  /* ׼�� */
	DEAL,	/* ���� */
	CALLLANDLORD,  /* �е��� */
	CHOOSELANDLORD,	/* �������������Ҷ�������Ʒ������� */
	OUTCARD, /* ���� */
	END /* ���� */
};

enum HeadImageType{	/* ͷ������ */
	LANDLORD,	/* ���� */
	FARMER	/* ũ�� */
};

enum HeadImageOrientation{	/* ͷ���� */
	LEFT, /* ���� */
	RIGHT  /* ���� */
};

enum PlayerSexuality{
	MALE,		/* ���� */
	FEMALE	/* Ů�� */
};

//enum CallLandlordScore{
//	ZERO,	/* ��� */
//	ONE,		/* һ�� */
//	TWO,  /* ���� */
//	THREE		/* ���� */
//};

enum PlayerOrderState{
	ORDERREADY,		/* ׼�� */
	NOCALL,	/* ���� */
	CALLONE,/* һ�� */
	CALLTWO,/* ���� */
	CALLTHREE, /* ���� */
	PASS, /* ������ */
};

/* �������״̬��λ�ã����Or�ұ� */
enum PlayerOrderPos{
	ORDERINLEFT,		/* ��ߵ� */
	ORDERINRIGHT	/* �ұߵ� */
};

#endif