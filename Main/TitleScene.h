#pragma once
#include "Config.h"
#include "GameEntity.h"

enum class selectedTitle{player_1, player_2, CONSTRUCTION}; // 컨벤션이 일관성 없음

class Image;
class TitleScene : public GameEntity
{
private:
	Image* title = nullptr;
	Image* backGround = nullptr;
	Image* tankUi = nullptr;

	POINT pos = {};

	selectedTitle selecTitle = {};
	int slidePos = 0;		 
	bool titleStart = false; 

public:
	virtual ~TitleScene() = default;

	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

};

