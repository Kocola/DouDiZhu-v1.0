#include "HeadImage.h"

SpriteFrameCache* HeadImage::spriteFrameCache = nullptr;

#define HEADIMAGE_SCALE 0.7 

bool HeadImage::init(){
	headBox = Sprite::create("Image/head_box.png");
	headBox->setScale(HEADIMAGE_SCALE);
	this->addChild(headBox);
	curHeadImage = Sprite::create("Image/no_head.png");
	curHeadImage->setScale(HEADIMAGE_SCALE);
	this->addChild(curHeadImage);

	this->setContentSize(headBox->getContentSize() * HEADIMAGE_SCALE);

	/* 初始化精灵帧缓存 */
	spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->addSpriteFramesWithFile("Image/headImage.plist",
		"Image/headImage.png");

	return true;
}

void HeadImage::setHeadImageType(HeadImageType headImageType, HeadImageOrientation headImageOrientation /* = LEFT */){
	/* 根据头像类型和朝向，建立一个精灵帧 */
	auto spriteFrame = HeadImage::createSpriteFrameWithType(headImageType, headImageOrientation);
	curHeadImage->setDisplayFrame(spriteFrame);	/* 改变当前头像的精灵帧，显示其他图片 */
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