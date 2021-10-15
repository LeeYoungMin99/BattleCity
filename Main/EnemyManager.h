#pragma once
#include "Config.h"
#include "GameEntity.h"

class Tank;
class EnemyManager : public GameEntity
{
private:
	int enemyMaxCount;
	int elapsedcount;

	TILE_INFO* tileInfo = nullptr;
	Tank* playerTank = nullptr;

public:
	vector<Tank*> vecEnemys;
	vector<Tank*>::iterator itEnemys;

	HRESULT Init(TILE_INFO* tilemap, Tank* playerTank);
	void Update();
	void Render(HDC hdc);
	void Release();

	void AddEnemy(Tank* tank,POINTFLOAT pos);
	
	inline vector<Tank*> GetVecEnemys() { return vecEnemys; }
};

