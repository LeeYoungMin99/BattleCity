#include "GameEntity.h"
#include "Config.h"
#include <vector>




#pragma once
class Image;
class EnemyManager;
class Tank;
class TankFactorial;
class Stage2Scene : public GameEntity
{
private:
	TILE_INFO tileInfo[TILE_COUNT_Y * TILE_COUNT_X];
	Image* sampleImage;

	//GameBoard °?·?
	Image* spawnMonsterImage;
	int remainSpawnMonster;
	int remainMonster;

	Image* lifeImage;
	Image* stageImage;
	Image* stageLevel;
	Image* backGround;

	Tank* tank = nullptr;
	vector<TankFactorial*> vecTankFactorial = {};

	float elapsedCount = 0.0f;
	float spawmElapsedCount = 5.0f;
	int spawnCount = 0;
	int maxSpawnCount = 3;
	int currSpawnEnemy = 0;
	int maxSpawnEnemy = 6;
	POINTFLOAT spawnEnemyPos[3] = {};
	EnemyManager* enemyMgr = nullptr;

	POINTFLOAT spawnPos[3];

	RECT backGroundRect;

	bool check = false;
	bool bShowBodyCollider = false;
	

	//물 타일 이미지
	vector<pair<int, int>> waterTilePos;
	int waterElapsedCount;
public:
	void Load(int index);
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
	void SpawnEnemy(TankType type);
};

