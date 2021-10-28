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
}SCORE_INFO; // 기호상수와 똑같은 컨벤션 사용 중

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
	int fireElapsedCount = 0; // cooltime으로 했으면 좀 더 명확한 의미를 가질 수 있지 않았을까

	bool renderTankCollider = false;

	vector<Tank*> vecEnemys;
	vector<Tank*>::iterator itEnemys; // 들고 있을 이유가 없음

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

	inline vector<Tank*>* GetAddresVecEnemys() { return &vecEnemys; } // 사실 이건 public하게 열어준 것과 다름 없음. 사용에 유의하는 것이 좋음.
	inline Tank* GetPlayerTank() { return playerTank; }
	inline TankFactory** GetTankFactory() { return tankFactory; } // 왜 만든 걸까?
};

