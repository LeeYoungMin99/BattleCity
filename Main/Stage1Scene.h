#include "GameEntity.h"
#include "Config.h"
#include <vector>

#pragma once
class Image;
class TankFactorial;
class Tank;
class Stage1Scene : public GameEntity
{
	TILE_INFO tileInfo[TILE_COUNT_Y * TILE_COUNT_X];
	Image* sampleImage;

	Tank* tank;
	vector<TankFactorial*> vecTankFactorial;
public:
	void Load(int index);
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

};

