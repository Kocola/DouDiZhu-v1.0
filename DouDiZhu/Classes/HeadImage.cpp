#include "HeadImage.h"

SpriteFrameCache* HeadImage::spriteFrameCache = nullptr;

bool HeadImage::init(){
	headBox = Sprite::create("Image/head_box.png");
	headBox->setScale(0.7);
	this->addChild(headBox);
	curHeadImage = Sprite::create("Image/no_head.png");
	curHeadImage->setScale(0.7);
	this->addChild(curHeadImage);

	/* ��ʼ������֡���� */
	spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->addSpriteFramesWithFile("Image/headImage.plist",
		"Image/headImage.png");

	return true;
}

void HeadImage::setHeadImageType(HeadImageType headImageType, HeadImageOrientation headImageOrientation /* = LEFT */){
	/* ����ͷ�����ͺͳ��򣬽���һ������֡ */
	auto spriteFrame = HeadImage::createSpriteFrameWithType(headImageType, headImageOrientation);
	curHeadImage->setDisplayFrame(spriteFrame);	/* �ı䵱ǰͷ��ľ���֡����ʾ����ͼƬ */
}

SpriteFrame* HeadImage::createSpriteFrameWithType(HeadImageType headImageType, HeadImageOrientation headImageOrientation /* = LEFT */){
	if (headImageType == LANDLORD){
		if (headImageOrientation == LEFT){
			return spriteFrameCache->getSpriteFrameByName("landlord_boy_left.png");
		}else if (headImageOrientation == RIGHT){
			return spriteFrameCache->getSpriteFrameByName("landlord_boy_right.png");
		}
	}else if(headImageType == FARMER){
		if (headImageOrientation == LEFT){
			return spriteFrameCache->getSpriteFrameByName("farmer_boy_left.png");
		}
		else if (headImageOrientation == RIGHT){
			return spriteFrameCache->getSpriteFrameByName("farmer_boy_right.png");
		}
	}
}