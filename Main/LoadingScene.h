#pragma once
#include "GameEntity.h"
#include "Config.h"

class Image;
class LoadingScene : public GameEntity
{
private:
	Image* backGround = nullptr;
	Image* stageLevel = nullptr;
	Image* stageText = nullptr;

	int elapsedCount = 0;
public:
	virtual ~LoadingScene() = default;

	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
};

