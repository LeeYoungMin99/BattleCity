#pragma once
#include "Config.h"
#include "GameEntity.h"
#include "BoomImage.h"

class Image;
class Tank;
class ItemManager;
class AmmoManager;
class EnemyManager : public GameEntity
{
private:
	int enemyMaxCount = 0;
	int elapsedcount = 0;

	BOOM_IMAGE_INFO boomImg[3];

	TILE_INFO* tileInfo = nullptr;

	Tank* playerTank = nullptr;
	GameEntity* stageInfo = nullptr;
	ItemManager* itemManager = nullptr;
	AmmoManager* ammoManager = nullptr;
	AmmoManager* targetAmmoManager = nullptr;
	string nextStage;
	bool clockItem = false;
	int elapsedcount_2 = 0;

	vector<Tank*> vecEnemys;
	vector<Tank*>::iterator itEnemys;
public:
	HRESULT Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tilemap, Tank* playerTank, GameEntity* stageInfo);
	void Update();
	void Render(HDC hdc);
	void Release();

	void AddEnemy(Tank* tank, POINTFLOAT pos);
	inline void SetClockItem(bool clockItem) { this->clockItem = clockItem; }

	inline vector<Tank*>* GetAddresVecEnemys() { return &vecEnemys; }
};

