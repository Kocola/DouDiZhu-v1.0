#include "MusicController.h"
#include "SimpleAudioEngine.h"

MusicController* MusicController::musicController = nullptr;

MusicController* MusicController::getInstance(){
	if (musicController == nullptr){
		musicController = new MusicController();
		if (musicController && musicController->init()){
			musicController->autorelease();
			musicController->retain();		/* 防止被内存管理器回收 */
		}else{
			CC_SAFE_DELETE(musicController);
		}
	}
	return musicController;
}

bool MusicController::init(){

	return true;
}

void MusicController::preLoadMusic(){
	/* 加载plist文件，读取文件中的游戏资源名称列表，返回一个ValeuMap对象 */
	ValueMap _map = FileUtils::getInstance()->getValueMapFromFile("music/music.plist");
	/* 加载全局音乐 */
	ValueMap _musicMap = _map.at("global").asValueMap();	
	this->loadMusics(_musicMap);
	/* 加载斗地主音乐 */
	_musicMap = _map.at("man").asValueMap().at("calllandlord").asValueMap();
	this->loadEffects(_musicMap);
	/* 加载出牌音乐 */
	_musicMap = _map.at("man").asValueMap().at("outcard").asValueMap();
	this->loadEffects(_musicMap);
}

void MusicController::loadMusics(const ValueMap& _musicFiles){
	for (auto it : _musicFiles){
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(
			it.second.asString().c_str());
	}
}

void MusicController::loadEffects(const ValueMap& _effectFiles){
	for (auto it : _effectFiles){
		CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(
			it.second.asString().c_str());
	}
}

void MusicController::playBackgroundMusic(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/background.mp3",true);
}

void MusicController::playStartMusic(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/start.mp3");
}

void MusicController::playWinMusic(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/win.mp3");
}

void MusicController::playLostMusic(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/lost.mp3");
}

void MusicController::playOutCardEffect(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/outcard.mp3");
}

void MusicController::playPassEffect(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/pass.mp3");
}

void MusicController::playPressButtonEffect(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/pressbutton.mp3");
}

void MusicController::playTouchCardEffect(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/tounchcard.mp3");
}

void MusicController::playCallLandlordEffect(CallLandlordEffect _effectType){
	auto _simpleAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	std::string _directory = "music/man/CallLandlord/";
	switch (_effectType)
	{
	case NOCALL_MUSIC:_simpleAudioEngine->playEffect((_directory + std::string("man_nocall.mp3")).c_str());break;
	case CALLONE_MUSIC:_simpleAudioEngine->playEffect((_directory + std::string("man_1fen.mp3")).c_str()); break;
	case CALLTWO_MUSIC:_simpleAudioEngine->playEffect((_directory + std::string("man_2fen.mp3")).c_str()); break;
	case CALLTHREE_MUSIC:_simpleAudioEngine->playEffect((_directory + std::string("man_3fen.mp3")).c_str()); break;
	case LANDLORD_MUSIC:_simpleAudioEngine->playEffect((_directory + std::string("man_landlord.mp3")).c_str()); break;
	default:break;
	}
}

void MusicController::playOutCardEffect(OutCardEffect _effectType){
	auto _simpleAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	std::string _directory = "music/man/OutCard/";
	switch (_effectType){
	case A:_simpleAudioEngine->playEffect((_directory + std::string("mancard_A.mp3")).c_str()); break;
	case TWO:_simpleAudioEngine->playEffect((_directory + std::string("mancard_2.mp3")).c_str()); break;
	case THREE:_simpleAudioEngine->playEffect((_directory + std::string("mancard_3.mp3")).c_str()); break;
	case FOUR:_simpleAudioEngine->playEffect((_directory + std::string("mancard_4.mp3")).c_str()); break;
	case FIVE:_simpleAudioEngine->playEffect((_directory + std::string("mancard_5.mp3")).c_str()); break;
	case SIX:_simpleAudioEngine->playEffect((_directory + std::string("mancard_6.mp3")).c_str()); break;
	case SEVEN:_simpleAudioEngine->playEffect((_directory + std::string("mancard_7.mp3")).c_str()); break;
	case EIGHT:_simpleAudioEngine->playEffect((_directory + std::string("mancard_8.mp3")).c_str()); break;
	case NINE:_simpleAudioEngine->playEffect((_directory + std::string("mancard_9.mp3")).c_str()); break;
	case TEN:_simpleAudioEngine->playEffect((_directory + std::string("mancard_10.mp3")).c_str()); break;
	case J:_simpleAudioEngine->playEffect((_directory + std::string("mancard_J.mp3")).c_str()); break;
	case Q:_simpleAudioEngine->playEffect((_directory + std::string("mancard_Q.mp3")).c_str()); break;
	case K:_simpleAudioEngine->playEffect((_directory + std::string("mancard_K.mp3")).c_str()); break;
	case AA:_simpleAudioEngine->playEffect((_directory + std::string("mancard_AA.mp3")).c_str()); break;
	case PAIRTWO:_simpleAudioEngine->playEffect((_directory + std::string("mancard_22.mp3")).c_str());break;
	case PAIRTHREE:_simpleAudioEngine->playEffect((_directory + std::string("mancard_33.mp3")).c_str()); break;
	case PAIRFOUR:_simpleAudioEngine->playEffect((_directory + std::string("mancard_44.mp3")).c_str()); break;
	case PAIRFIVE:_simpleAudioEngine->playEffect((_directory + std::string("mancard_55.mp3")).c_str()); break;
	case PAIRSIX:_simpleAudioEngine->playEffect((_directory + std::string("mancard_66.mp3")).c_str()); break;
	case PAIRSEVEN:_simpleAudioEngine->playEffect((_directory + std::string("mancard_77.mp3")).c_str()); break;
	case PAIREIGHT:_simpleAudioEngine->playEffect((_directory + std::string("mancard_88.mp3")).c_str()); break;
	case PAIRNINE:_simpleAudioEngine->playEffect((_directory + std::string("mancard_99.mp3")).c_str()); break;
	case PAIRTEN:_simpleAudioEngine->playEffect((_directory + std::string("mancard_1010.mp3")).c_str()); break;
	case JJ:_simpleAudioEngine->playEffect((_directory + std::string("mancard_JJ.mp3")).c_str()); break;
	case QQ:_simpleAudioEngine->playEffect((_directory + std::string("mancard_QQ.mp3")).c_str()); break;
	case KK:_simpleAudioEngine->playEffect((_directory + std::string("mancard_KK.mp3")).c_str()); break;
	case TRIPLE_EFFECT:_simpleAudioEngine->playEffect((_directory + std::string("mancard_triple.mp3")).c_str()); break;
	case BOMB_EFFECT:_simpleAudioEngine->playEffect((_directory + std::string("mancard_bomb_1.mp3")).c_str()); break;
	case BLACKKING:_simpleAudioEngine->playEffect((_directory + std::string("mancard_blackjoker.mp3")).c_str()); break;
	case REDKING:_simpleAudioEngine->playEffect((_directory + std::string("mancard_redjoker.mp3")).c_str()); break;
	case PAIRSTRAIGHT_EFFECT:_simpleAudioEngine->playEffect((_directory + std::string("mancard_pairstraight.mp3")).c_str()); break;
	case TRIPLESTRAIGHT_EFFECT:_simpleAudioEngine->playEffect((_directory + std::string("mancard_triplestraight.mp3")).c_str()); break;
	case THREEWITHTWO:_simpleAudioEngine->playEffect((_directory + std::string("mancard_3with2_1.mp3")).c_str()); break;
	default:break;
	}
}