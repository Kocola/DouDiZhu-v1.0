#ifndef __GAME_RULES_H__
#define __GAME_RULES_H__

#include "cocos2d.h"
#include "Poker.h"
#include "PokerValueType.h"

using namespace cocos2d;

class GameRules{
public:
	PokerValueType analysePokerValueType(Vector<Poker*> _pokers);	/* ���������˿˵����� */
	bool isPokerValueType(Vector<Poker*> _pokers, PokerValueType pokerValueType);	/* �������˿��Ƿ���ĳ������ */
	/* ��ȡָ�������͵��˿ˣ�����VectorΪ��ʱ��ʾ����ʧ�� 
		��һ�����������ӵ�е��˿� */
	Vector<Poker*> calcPokerWithValueType(Vector<Poker*> _pokers, PokerValueType pokerValueType);
private:
	/* �������º�����value��������ʾ��Ҫ���ҵ���������λ������ֵ */

	/* ��һ����������ҵ����棬�ڶ������ϼҶ�Ӧ���͵�ֵ��Ĭ����0����ʾ���������Ӧ����ֵ��������Ҫ����value�Ķ�Ӧ���Ͳſ��� */
	/* search������search�����ĵ��ö��󣬸��ݵڶ��������������ҷ���Ҫ������ͣ������������� 
		search���������ڲ��� ���ţ� ���ӣ� ���ţ��Լ�ը��*/
	Vector<Poker*> search(Vector<Poker*> _pokers, PokerValueType pokerValueType, int value = 0);
	Vector<Poker*> searchSingle(Vector<Poker*> _pokers, int value = 0);	/* ���ҵ��� */
	Vector<Poker*> searchPair(Vector<Poker*> _pokers, int value = 0);		/* ���Ҷ���  */
	Vector<Poker*> searchTriple(Vector<Poker*> _pokers, int value = 0); /* �������� */
	Vector<Poker*> searchBomb(Vector<Poker*> _pokers, int value = 0); /* ����ը�� */
	/* searchStraight�ǲ�������˳�ӵĻ����������������ֶ�Ҫ�����������, step���ڿ��Ʋ���ʲô���͵�˳��
		length��������˳�ӵĳ����Ƕ��٣�����56789 ���Ⱦ���5
		��˳����������5��˫˳����˳�ĳ���������3*/
	Vector<Poker*> searchStraight(Vector<Poker*> _pokers, int step, int length, int value = 0); 
	Vector<Poker*> searchSingleStraight(Vector<Poker*> _pokers, int length, int value = 0);	/* ����˳�� */
	Vector<Poker*> searchPairStraight(Vector<Poker*> _pokers, int length, int value = 0); /* ����˫˳ */
	Vector<Poker*> searchTripleStraight(Vector<Poker*> _pokers, int length, int value = 0); /* ������˳ */

	Vector<Poker*> searchKingBomb(Vector<Poker*> _pokers, int value = 0); /* ������ը */
private:
	bool isSingle(Vector<Poker*> _pokers);	/* �Ƿ��ǵ��� */
	bool isPair(Vector<Poker*> _pokers);	/* �Ƿ��Ƕ��� */
	bool isTriple(Vector<Poker*> _pokers);	/* �Ƿ������� */
	bool isStraight(Vector<Poker*> _pokers, int step = 1);	/* �Ƿ���˳��,�ڶ�����������˫˳����˳���Ե����������������������� */
	bool isPairStraight(Vector<Poker*> _pokers);	/* �Ƿ���˫˳ */
	bool isTripleStraight(Vector<Poker*> _pokers);	/* �Ƿ�����˳ */
	bool isBomb(Vector<Poker*> _pokers);		/* �Ƿ���ը�� */
	bool isKingBomb(Vector<Poker*> _pokers);	/* �Ƿ�����ը */
};

#endif