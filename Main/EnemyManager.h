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
	int tankHP = -1;
}SCORE_INFO;

#define NUMBER_OF_IMAGES 6

class Image;
class Tank;
class ItemManager;
class AmmoManager;
class EnemyManager : public GameEntity
{
private:
	int enemyMaxCount = 0;
	int elapsedcount = 0;

	int tlqkf = 0;

	BOOM_IMAGE_INFO boomImg[NUMBER_OF_IMAGES];
	SCORE_INFO scoreImg[NUMBER_OF_IMAGES];

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
	void BoomItem();

	inline vector<Tank*>* GetAddresVecEnemys() { return &vecEnemys; }
};

