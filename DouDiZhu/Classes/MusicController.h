#ifndef __MUSIC_CONTROLLER_H__
#define __MUSIC_CONTROLLER_H__

#include "cocos2d.h"

using namespace cocos2d;

enum OutCardEffect{
	A, TWO, THREE, FOUR, FIVE,
	SIX, SEVEN, EIGHT, NINE, TEN,
	J, Q, K,
	AA, PAIRTWO, PAIRTHREE, PAIRFOUR, PAIRFIVE,
	PAIRSIX, PAIRSEVEN, PAIREIGHT, PAIRNINE, PAIRTEN,
	JJ, QQ, KK,
	TRIPLE_EFFECT, BOMB_EFFECT, BLACKKING, REDKING, 
	PAIRSTRAIGHT_EFFECT, TRIPLESTRAIGHT_EFFECT,
	THREEWITHTWO
};

enum CallLandlordEffect{
	NOCALL_MUSIC,
	CALLONE_MUSIC,
	CALLTWO_MUSIC,
	CALLTHREE_MUSIC,
	LANDLORD_MUSIC
};

enum GlobalMusic{
	START,
	LOST,
	WIN,
	OUTCARD_MUSIC,
	PASS_MUSIC,
	PRESSBUTTON,
	TOUCHCARD
};

class MusicController : public Ref{
public:
	static MusicController* getInstance();
	virtual bool init();
public:
	/* ���ű������� */
	void playBackgroundMusic();
	/* ������Ϸ��ʼ������ */
	void playStartMusic();
	/* ����ʤ������ */
	void playWinMusic();
	/* �������˵����� */
	void playLostMusic();
	/* ���ų�����Ч */
	void playOutCardEffect();
	/* ���Ų�����Ч */
	void playPassEffect();
	/* ���Ű��°�ť����Ч */
	void playPressButtonEffect();
	/* ���Ŵ����Ƶ���Ч */
	void playTouchCardEffect();
public:
	/* ���Žе���ʱ����Ч */
	void playCallLandlordEffect(CallLandlordEffect _effectType);
	/* ���ų���ʱ����Ч */
	void playOutCardEffect(OutCardEffect _effectType);
private:
	void preLoadMusic();	/* Ԥ�ȼ����������� */
	void loadMusics(const ValueMap& _musicFiles);	/* �����ļ��������� */
	void loadEffects(const ValueMap& _effectFiles);	/* �����ļ�������Ч */
private:
	
private:
	static MusicController* musicController;
};

#endif