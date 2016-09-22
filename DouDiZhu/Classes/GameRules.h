#ifndef __GAME_RULES_H__
#define __GAME_RULES_H__

#include "cocos2d.h"
#include "Poker.h"
#include "PokerValueType.h"

using namespace cocos2d;

class GameRules : public Ref{
public:
	static GameRules* getInstance();
	virtual bool init();
public:
	PokerValueType analysePokerValueType(Vector<Poker*> _pokers);	/* ���������˿˵����� */
	bool isPokerValueType(Vector<Poker*> _pokers, PokerValueType pokerValueType);	/* �������˿��Ƿ���ĳ������ */
	/* ��ȡָ�������͵��˿ˣ�����VectorΪ��ʱ��ʾ����ʧ�� 
		��һ�����������ӵ�е��˿ˣ������������Ƿ��ص����͵��˿�����С���ƣ������nullptr���������Գ�����ֵ�ĸ������˿�
		���ĸ����������˳��ʱ�ĳ��� */
	Vector<Poker*> calcPokerWithValueType(Vector<Poker*> _pokers, PokerValueType pokerValueType, const Poker* _poker = nullptr, int length = 0);
private:
	/* �������º�����value��������ʾ��Ҫ���ҵ���������λ������ֵ */

	/* ��һ����������ҵ����棬�ڶ������ϼҶ�Ӧ���Ƶ���С�˿˵�ָ�룬Ĭ��Ϊnullptr����ʾ���������Ӧ�����ͼ��ɣ�������Ҫ ���� _poker�Ķ�Ӧ���Ͳſ��� */
	//Vector<Poker*> search(Vector<Poker*> _pokers, PokerValueType pokerValueType, int value = 0);	/* �ú����������������ҷֱ�ʵ�� */
	Vector<Poker*> searchSingle(Vector<Poker*> _pokers, const Poker* _poker = nullptr);	/* ���ҵ��� */
	Vector<Poker*> searchPair(Vector<Poker*> _pokers, const Poker* _poker = nullptr);		/* ���Ҷ���  */
	Vector<Poker*> searchTriple(Vector<Poker*> _pokers, const Poker* _poker = nullptr); /* �������� */
	Vector<Poker*> searchBomb(Vector<Poker*> _pokers, const Poker* _poker = nullptr); /* ����ը�� */
	Vector<Poker*> searchKingBomb(Vector<Poker*> _pokers); /* ������ը */
	/* ���ҹ̶�ֵ�ĵ��ţ����ӣ����� */
	Vector<Poker*> searchSpecifiedSingle(Vector<Poker*> _pokers, const Poker* _poker);
	Vector<Poker*> searchSpecifiedPair(Vector<Poker*> _pokers, const Poker* _poker);
	Vector<Poker*> searchSpecifiedTriple(Vector<Poker*> _pokers, const Poker* _poker);
	/* searchStraight�ǲ�������˳�ӵĻ����������������ֶ�Ҫ�����������, step���ڿ��Ʋ���ʲô���͵�˳��
		length��������˳�ӵĳ����Ƕ��٣�����56789 ���Ⱦ���5
		��˳����������5��˫˳����˳�ĳ���������3*/
	//Vector<Poker*> searchStraight(Vector<Poker*> _pokers, int step, int length, int value = 0);	/* �ú������������ҵĶ���˳�ӷֱ�ʵ�� */
	Vector<Poker*> searchSingleStraight(Vector<Poker*> _pokers, int length, const Poker* _poker);	/* ����˳�� */
	Vector<Poker*> searchPairStraight(Vector<Poker*> _pokers, int length, const Poker* _poker); /* ����˫˳ */
	Vector<Poker*> searchTripleStraight(Vector<Poker*> _pokers, int length, const Poker* _poker); /* ������˳ */
private:
	bool isSingle(Vector<Poker*> _pokers);	/* �Ƿ��ǵ��� */
	bool isPair(Vector<Poker*> _pokers);	/* �Ƿ��Ƕ��� */
	bool isTriple(Vector<Poker*> _pokers);	/* �Ƿ������� */
	bool isStraight(Vector<Poker*> _pokers, int step = 1);	/* �Ƿ���˳��,�ڶ�����������˫˳����˳���Ե����������������������� */
	bool isPairStraight(Vector<Poker*> _pokers);	/* �Ƿ���˫˳ */
	bool isTripleStraight(Vector<Poker*> _pokers);	/* �Ƿ�����˳ */
	bool isBomb(Vector<Poker*> _pokers);		/* �Ƿ���ը�� */
	bool isKingBomb(Vector<Poker*> _pokers);	/* �Ƿ�����ը */
private:
	static GameRules* gameRules;
};

#endif