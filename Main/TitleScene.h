#pragma once
#include "Config.h"
#include "GameEntity.h"

enum class selectedTitle{player_1, player_2, CONSTRUCTION};

class Image;
class TitleScene : public GameEntity
{
private:
	Image* title = nullptr;
	Image* backGround = nullptr;
	Image* tankUi = nullptr;

	POINT pos = {};

	LPARGUMENT_PTR arg = {};
	selectedTitle selecTitle = {};
	int test = 0; //슬라이드효과 테스트
	bool titleStart = false; //디버깅용

public:
	virtual ~TitleScene() = default;

	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

};

