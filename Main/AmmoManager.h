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
	int ammoMaxCount = 12;

	Tank* playerTank = nullptr;
	EnemyManager* enemyMgr = nullptr;
	TILE_INFO* tileInfo = nullptr;
public:
	vector<Ammo*> vecAmmos;
	vector<Ammo*>::iterator itAmmos;

	HRESULT Init(TILE_INFO* tileInfo, Tank* playerTank = nullptr, EnemyManager* enemyMgr = nullptr);
	void Update();
	void Render(HDC hdc);
	void Release();

	void Fire(Tank* tank, AmmoManager* playerAmmoManager, AmmoManager* enemyAmmoManager);
	vector<Ammo*>* GetAddressVecAmmos() { return &vecAmmos; }
	vector<Ammo*>::iterator* GetAddressItAmmos() { return &itAmmos; }
	void SetAddressItAmmos(vector<Ammo*>* vecAmmos) { itAmmos = vecAmmos->begin(); }
	void addAddressItAmmos() { itAmmos++; }

};

