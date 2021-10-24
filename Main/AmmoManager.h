#pragma once
#include "Config.h"
#include "GameEntity.h"

class GameObject;
class Ammo;
class Tank;
class EnemyManager;
class AmmoManager : public GameEntity
{
private:
	int ammoMaxCount = 24;

	Tank* playerTank = nullptr;
	vector<Tank*>* enemyTanks = nullptr;
	TILE_INFO* tileInfo = nullptr;
	vector<Ammo*> vecAmmos;
	vector<Ammo*>::iterator itAmmos;
public:
	virtual ~AmmoManager() = default;

	HRESULT Init(TILE_INFO* tileInfo, Tank* playerTank = nullptr, vector<Tank*>* enemyTanks = nullptr);
	void Update();
	void Render(HDC hdc);
	void Release();

	void Fire(Tank* tank, float moveSpeed = 200.0f);
	vector<Ammo*>* GetAddressVecAmmos() { return &vecAmmos; }
	vector<Ammo*>::iterator* GetAddressItAmmos() { return &itAmmos; }
	void SetAddressItAmmos(vector<Ammo*>* vecAmmos) { itAmmos = vecAmmos->begin(); }
	void addAddressItAmmos() { itAmmos++; }
};

