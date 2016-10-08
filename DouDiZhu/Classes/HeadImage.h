#ifndef __HEAD_IMAGE_H__
#define __HEAD_IMAGE_H__

#include "cocos2d.h"

using namespace cocos2d;

#include "GlobalDefine.h"

class HeadImage : public Node{
public:
	CREATE_FUNC(HeadImage);
	virtual bool init();
public:
	void setHeadImageType(HeadImageType headImageType, HeadImageOrientation headImageOrientation = LEFT);	/* 设置当前的 */
	Size getHeadBoxSize() { return headBox->getContentSize(); }
private:
	static SpriteFrame* createSpriteFrameWithType(HeadImageType headImageType, HeadImageOrientation headImageOrientation = LEFT);
private:
	Sprite* curHeadImage;		/* 当前头像 */
	Sprite* headBox;		/* 头像框 */
private:
	static SpriteFrameCache* spriteFrameCache;
};

#endif