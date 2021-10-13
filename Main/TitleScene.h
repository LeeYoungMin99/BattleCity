#pragma once
#include "Config.h"
#include "GameEntity.h"

class Image;
class TitleScene : public GameEntity
{
private:
	Image* backGround;
	Image* tankUi;

	POINT pos;

	LPARGUMENT_PTR arg;

public:
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

};

