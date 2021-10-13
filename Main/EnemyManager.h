#pragma once
#include "Config.h"
#include "GameEntity.h"

class Tank;
class EnemyManager : public GameEntity
{
private:
	vector<Tank*> vecEnemys;
	vector<Tank*>::iterator itEnemys;

	int enemyMaxCount;

	TILE_INFO* tileInfo = nullptr;

public:
	HRESULT Init(TILE_INFO* tilemap);
	void Update();
	void Render(HDC hdc);
	void Release();

	void AddEnemy(Tank* tank,POINTFLOAT pos);
	
	inline vector<Tank*> GetVecEnemys() { return vecEnemys; }
};

