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
	/* 播放背景音乐 */
	void playBackgroundMusic();
	/* 播放游戏开始的音乐 */
	void playStartMusic();
	/* 播放胜利音乐 */
	void playWinMusic();
	/* 播放输了的音乐 */
	void playLostMusic();
	/* 播放出牌音效 */
	void playOutCardEffect();
	/* 播放不出音效 */
	void playPassEffect();
	/* 播放按下按钮的音效 */
	void playPressButtonEffect();
	/* 播放触摸牌的音效 */
	void playTouchCardEffect();
public:
	/* 播放叫地主时的音效 */
	void playCallLandlordEffect(CallLandlordEffect _effectType);
	/* 播放出牌时的音效 */
	void playOutCardEffect(OutCardEffect _effectType);
private:
	void preLoadMusic();	/* 预先加载所有音乐 */
	void loadMusics(const ValueMap& _musicFiles);	/* 根据文件加载音乐 */
	void loadEffects(const ValueMap& _effectFiles);	/* 根据文件加载音效 */
private:
	
private:
	static MusicController* musicController;
};

#endif