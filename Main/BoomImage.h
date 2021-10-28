#pragma once
#include "Config.h"

// 얘는 왜 필요한 건가
typedef struct BoomImageInfo
{
	bool bRenderBoomImg			= false;
	int elapsedCount			= 0;
	int addImgFrameCount		= 5;
	int BoomImgCurrFrame		= 0;
	int BoomImgMaxFrame			= 5;
	Image* BoomImg				= nullptr;
	POINTFLOAT imgPos			= {};
}BOOM_IMAGE_INFO;