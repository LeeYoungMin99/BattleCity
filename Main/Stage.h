#include "GameEntity.h"
#include "Config.h"
#include "BoomImage.h"
#include <vector>




#pragma once
class Image;
class EnemyManager;
class AmmoManager;
class Tank;
class TankFactory;
class ItemManager;
class Stage : public GameEntity
{
private:
	TILE_INFO tileInfo[TILE_COUNT_Y * TILE_COUNT_X];
	Image* sampleImage;

	//GameBoard °?·?
	Image* spawnMonsterImage;
	int remainSpawnMonster;
	int remainMonster;

	BOOM_IMAGE_INFO boomImg[2];

	Image* lifeImage;
	Image* stageImage;
	Image* stageLevel;
	Image* backGround;

	Image* slate;
	int slate1;
	int slate2;

	Tank* tank = nullptr;
	vector<TankFactory*> vecTankFactorial = {};

	AmmoManager* playerTankAmmoManager = nullptr;
	AmmoManager* enemyTankAmmoManager = nullptr;

	float elapsedCount = 0.0f;
	float spawmElapsedCount = 5.0f;
	int spawnCount = 0;
	int maxSpawnCount = 3;
	int currSpawnEnemy = 0;
	int maxSpawnEnemy = 6;
	POINTFLOAT spawnEnemyPos[3] = {};
	EnemyManager* enemyMgr = nullptr;
	vector<Tank*>::iterator itEnemyTanks = {};

	POINTFLOAT spawnPos[3];

	RECT backGroundRect;

	bool check = false;
	bool bShowBodyCollider = false;

	ItemManager* itemManager;


	//물 타일 이미지
	vector<pair<int, int>> waterTilePos;
	int waterElapsedCount;


	//GameOver
	Image* gameOver;
	int gameOverPosY;
	int stateElapsedCount;
public:
	void Load(int index);
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
	void SpawnEnemy(TankType type);
	void CreateItem();

	//Update()
	void CloseSlate();
	void RotateGameOverScene();
	inline void SubCurrSpawnEnemy() { this->currSpawnEnemy--; }
	void RotateToGameOverState();
	void RotateToScoreScene();
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
};

