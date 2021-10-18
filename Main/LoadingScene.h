#pragma once
#include "GameEntity.h"
#include "Config.h"

class Image;
class LoadingScene : public GameEntity
{
private:
	Image* backGround;
	Image* stageLevel;
	Image* stageText;

	int elapsedCount;
	string nextStage;
public:
	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();
};

