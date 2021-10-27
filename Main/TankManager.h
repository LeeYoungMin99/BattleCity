#pragma once
#include "Config.h"
#include "GameEntity.h"
#include "BoomImage.h"
 

typedef struct scoreInfo
{
	POINTFLOAT imgPos = {};
	bool bRenderScore = false;
	Image* scoreImg = nullptr;
	int elapsedCount = 0;
	int maxElapsedCount = 50;
	int scoreFrame = 0;
	int tankHP = 1;
}SCORE_INFO;

#define NUMBER_OF_IMAGES 7

class Image;
class Tank;
class Item;
class TankFactory;
class ItemManager;
class AmmoManager;
class TankManager : public GameEntity
{
private:
	int enemyMaxCount = 0;
	int elapsedcount = 0;

	BOOM_IMAGE_INFO boomImg[NUMBER_OF_IMAGES] = {};
	SCORE_INFO scoreImg[NUMBER_OF_IMAGES] = {};

	TILE_INFO* tileInfo = nullptr;

	TankFactory* tankFactory[5] = {};

	Tank* playerTank = nullptr;
	GameEntity* stageInfo = nullptr;
	AmmoManager* ammoManager = nullptr;
	AmmoManager* targetAmmoManager = nullptr;
	bool clockItem = false;
	int fireElapsedCount = 0;

	bool renderTankCollider = false;

	vector<Tank*> vecEnemys;
	vector<Tank*>::iterator itEnemys;

	vector<Item*>* addressVecItems = nullptr;
public:
	virtual ~TankManager() = default;

	HRESULT Init(AmmoManager* ammoManager, AmmoManager* targetAmmoManager, TILE_INFO* tilemap, GameEntity* stageInfo, vector<Item*>* vecItems);
	void Update();
	void Render(HDC hdc);
	void Release();

	void AddEnemy(TankType type, POINTFLOAT pos);
	inline void SetClockItem(bool clockItem) { this->clockItem = clockItem; }
	void BoomItem();
	void PlayerTankDestroyAnimation();

	inline vector<Tank*>* GetAddresVecEnemys() { return &vecEnemys; }
	inline Tank* GetPlayerTank() { return playerTank; }
	inline TankFactory** GetTankFactory() { return tankFactory; }
};

