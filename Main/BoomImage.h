#pragma once
#include "Config.h"

typedef struct BoomImageInfo
{
	POINTFLOAT imgPos = {};
	bool bRenderBoomImg = false;
	Image* BoomImg = nullptr;
	int elapsedCount = 0;
	int addImgFrameCount = 5;
	int BoomImgCurrFrame = 0;
	int BoomImgMaxFrame = 5;
}BOOM_IMAGE_INFO;