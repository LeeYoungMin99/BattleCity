#include "GameEntity.h"
#include "Config.h"
#include <vector>

#pragma once
class Image;
class TankFactorial;
class Tank;
class Stage1Scene : public GameEntity
{
private:
	TILE_INFO tileInfo[TILE_COUNT_Y * TILE_COUNT_X];
	Image* sampleImage;
	
	//GameBoard ฐüทร
	Image *spawnMonsterImage;
	int remainSpawnMonster;
	int remainMonster;

	Image *lifeImage;
	Image *stageImage;
	Image* stageLevel;
	Image* backGround;

	Tank* tank;
	vector<TankFactorial*> vecTankFactorial;

	POINTFLOAT spawnPos[3];

	bool check = false;
public:
	void Load(int index);
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
	void Spawn();
};

