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
	void setHeadImageType(HeadImageType headImageType, HeadImageOrientation headImageOrientation = LEFT);	/* ���õ�ǰ�� */
	Size getHeadBoxSize() { return headBox->getContentSize(); }
private:
	static SpriteFrame* createSpriteFrameWithType(HeadImageType headImageType, HeadImageOrientation headImageOrientation = LEFT);
private:
	Sprite* curHeadImage;		/* ��ǰͷ�� */
	Sprite* headBox;		/* ͷ��� */
private:
	static SpriteFrameCache* spriteFrameCache;
};

#endif