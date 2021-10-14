#pragma once
#include "Config.h"
#include "GameEntity.h"

enum selectedTitle{player_1, player_2, CONSTRUCTION};

class Image;
class TitleScene : public GameEntity
{
private:
	Image* title;
	Image* backGround;
	Image* tankUi;

	POINT pos;

	LPARGUMENT_PTR arg;
	selectedTitle selecTitle;
	int test; //슬라이드효과 테스트
	bool titleStart; //디버깅용

public:
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

};

