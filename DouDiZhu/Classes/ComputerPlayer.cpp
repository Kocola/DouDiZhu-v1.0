#include "ComputerPlayer.h"
#include "GameRules.h"
#include "MusicController.h"
#include "OutCards.h"
#include "PlayerOrder.h"

ComputerPlayer* ComputerPlayer::create(PlayerPosType _playerPosType){
	auto pRet = new(std::nothrow)ComputerPlayer();
	if (pRet && pRet->init(_playerPosType)){
		pRet->autorelease();
		return pRet;
	}else{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool ComputerPlayer::init(PlayerPosType _playerPosType){
	if (!Player::init(_playerPosType)) return false;
	return true;
}


int ComputerPlayer::automaticCallLandlord(){
	srand((unsigned)time(0));	/* ����������� */
	int score = rand() % 4;	/* �з�һ���в��У�1��2��3�������������0��ʾ���� */
	return score;
}

void ComputerPlayer::callLandlord(){
	int score = this->automaticCallLandlord();
	this->setCallLandlordScore(score);
	this->updateCallLandlordState();
	if (score == MAXCALLLANDLORDSCORE){
		this->setGameStateChooseLandlord();
	}else{
		this->updateCallLandlordOrder();	/* ���½���һ���е�����ID */
	}
}

void ComputerPlayer::outCard(OutCards* _lastOutcards){

	this->deleteOutcardInScene();

	Vector<Poker*> _pokers = this->searchOutCard(_lastOutcards);/* ������Ϸ��һ���Ʋ��ҷ���Ҫ����� */

	auto _playerOrder = this->getPlayerOrder();

	/* �������ʱ���� 3 ~ 8 */
	this->startCountDown(rand() % 5 + 3, [=]() mutable{/* ������ʱ��������������& */
		if (_pokers.size() == 0){
			_playerOrder->setPlayerOrderState(PASS);	/* ��ʾ������״̬ */
			_playerOrder->setVisible(true);

			/* ���Ų����Ƶ���Ч */
			MusicController::getInstance()->playPassEffect();
		}
		else{
			/* �Ե�ǰ���Ƹ�����Ϸ����һ���� */
			_lastOutcards = OutCards::create(this, GameRules::getInstance()->analysePokerValueType(_pokers), _pokers.size(), _pokers.at(_pokers.size() - 1));
			_lastOutcards->retain();
			this->updateLastOutCards(_lastOutcards);
			/* �������ƴӵ�ǰ��ҵ��˿���ɾ�� */
			for (int i = 0; i < _pokers.size(); ++i){
				_pokers.at(i)->removeFromParent();
				this->removeCard(_pokers.at(i));
			}
			/* ��Ҫ�������ڳ�������ʾ���� */
			/*this->deleteOutcardInScene();*/
			this->setOutcardInScene(_pokers);
			this->showOutcardInScene();
			this->playOutCardInSceneMusic();	/* ���ų������� */

			if (this->getPoker().size() == 0){
				this->setGameOver();
				return;
			}
		}
		this->updateOutOrder();	/* ���³���˳������һ����ҳ��� */
	});
}

Vector<Poker*> ComputerPlayer::searchOutCard(OutCards* _lastOutcards){
	Vector<Poker*> _pokers;
	/* �����һ����Ҳ���Լ��� */
	if (_lastOutcards->getPokerOwner() == this || _lastOutcards->getPokerValueType() == NONE){
		//_pokers = GameRules::getInstance()->calcPokerWithValueType(_computer->getPoker(), SINGLE, nullptr);	/* ����д�ᵼ�µ������Ҳ������ź�һֱ��������ط� */ 
		_pokers = GameRules::getInstance()->searchProperPokers(this->getPoker());
	}
	else{
		PokerValueType _pokerValueType = _lastOutcards->getPokerValueType();
		if (_pokerValueType != KINGBOMB){
			if (_pokerValueType == BOMB){
				_pokers = GameRules::getInstance()->calcPokerWithValueType(this->getPoker(), BOMB, _lastOutcards->getLowestPoker());
			}
			else{
				_pokers = GameRules::getInstance()->calcPokerWithValueType(
					this->getPoker(), _lastOutcards->getPokerValueType(), _lastOutcards->getLowestPoker(), _lastOutcards->getTotalLength());

				if (_pokers.size() == 0){ /* ����Ҳ�����Ӧ���ƣ�����ը�� */
					_pokers = GameRules::getInstance()->calcPokerWithValueType(this->getPoker(), BOMB, nullptr);
					if (_pokers.size() == 0){	/* ����Ҳ�����ͨ��ը��������ը */
						_pokers = GameRules::getInstance()->calcPokerWithValueType(this->getPoker(), KINGBOMB);
					}
				}
			}
		}
	}
	return _pokers;
}