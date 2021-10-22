#pragma once
#include "GameEntity.h"
#include "Config.h"
#include "BoomImage.h"
#include <vector>




class Image;
class EnemyManager;
class AmmoManager;
class Tank;
class TankFactory;
class ItemManager;

class Stage : public GameEntity
{
public:
	virtual ~Stage() = default;

	void Load(int index);
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
	void SpawnEnemy(TankType type);
	void CreateItem();

	//Update()
	bool CloseSlate();
	bool RotateGameOverScene();
	inline void SubCurrSpawnEnemy() { this->currSpawnEnemy--; }
	void RotateToGameOverState();
	bool RotateToScoreScene();
	void WaterTileAnimation();
	void NexusDestoryAnimation();
	void ShowBodyCollider();
	void ShowBulletCollider();
	void SpawnEnemyTank();
	void PlayerTankDestroyAnimation();

	//Render()
	void TileRender(HDC hdc);
	void RemainEnemyRender(HDC hdc);
	void GrassTileRender(HDC hdc);
	void PlayerLifeRender(HDC hdc);
	void StageLevelRender(HDC hdc);
	void PlayerTankDestroyRender(HDC hdc);
	void NexusDestroyRender(HDC hdc);

private:
	Image* sampleImage = nullptr;
	Image* spawnMonsterImage = nullptr;
	
	Image* lifeImage = nullptr;
	Image* stageImage = nullptr;
	Image* stageLevel = nullptr;
	Image* backGround = nullptr;

	Image* slate = nullptr;
	int slate1;
	int slate2;

	Tank* tank = nullptr;
	TankFactory* tankFactory[5] = {};
	
	AmmoManager* playerTankAmmoManager = nullptr;
	AmmoManager* enemyTankAmmoManager = nullptr;

	EnemyManager* enemyMgr = nullptr;
	float elapsedCount = 0.0f;
	float spawmElapsedCount = 5.0f;
	int spawnCount = 0;
	int maxSpawnCount = 3;
	int currSpawnEnemy = 0;
	int maxSpawnEnemy = 6;

	POINTFLOAT spawnEnemyPos[3] = {};
	POINTFLOAT spawnPos[3] = {};

	TILE_INFO tileInfo[TILE_COUNT_Y * TILE_COUNT_X] = {};
	BOOM_IMAGE_INFO boomImg[2] = {};





	RECT backGroundRect = {};

	bool check = false;
	bool bShowBodyCollider = false;

	ItemManager* itemManager = nullptr;


	//물 타일 이미지
	vector<pair<int,int>> waterTilePos;
	int waterElapsedCount = 0;


	//GameOver
	Image* gameOver = nullptr;
	int gameOverPosY = 0;
	int stateElapsedCount = 0;
};

