#pragma once
#include "Config.h"
#include "GameEntity.h"

class Image;
class GameOverScene : public GameEntity
{
private:
	Image* gameOver;
	Image* backGround;

	int test_1, test_2;
	int elapsedcount;

public:
	virtual ~GameOverScene() = default;

	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
};

