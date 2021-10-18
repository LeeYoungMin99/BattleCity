#pragma once
#include "Config.h"
#include "GameEntity.h"

class Image;
class Tank;
class ItemManager;
typedef struct BoomImageInfo
{
	POINTFLOAT imgPos = {};
	bool bRenderSmallBoomImg = false;
	bool bRenderBigBoomImg = false;
	Image* bigBoomImg = nullptr;
	Image* smallBoomImg = nullptr;
	int elapsedCount = 0;
	int addImgFrameCount = 5;
	int bigBoomImgCurrFrame = 0;
	int bigBoomImgMaxFrame = 2;
	int smallBoomImgCurrFrame = 0;
	int smallBoomImgMaxFrame = 3;
}BOOM_IMAGE_INFO;

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

public:
	vector<Tank*> vecEnemys;
	vector<Tank*>::iterator itEnemys;

	HRESULT Init(TILE_INFO* tilemap, Tank* playerTank , GameEntity* stageInfo);
	void Update();
	void Render(HDC hdc);
	void Release();

	void AddEnemy(Tank* tank, POINTFLOAT pos);

	inline vector<Tank*> GetVecEnemys() { return vecEnemys; }
};

