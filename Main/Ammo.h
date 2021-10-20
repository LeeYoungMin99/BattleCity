#pragma once
#include "Config.h"
#include "GameObject.h"

enum class BulletDir { Left, Right, Up, Down };

class Image;
class Tank;		// ���漱�� (Tank��� Ŭ������ �ִ�)
class EnemyManager;
class AmmoManager;
class Ammo : public GameObject
{
private:
	bool isFire = false;
	//bool isAlive;

	float moveAngle = 0.0f;

	Image* img = nullptr;
	Image* boomImg = nullptr;
	int boomImgCurrFrame = 0;
	int boomImgMaxFrame = 3;
	bool bRenderBoomImg = false;
	int currElapsedCount = 0;
	int addImgFrameCount = 5;

	BulletDir bulletDir = BulletDir::Down;

	RECT collision = {}; // �浹 �ݸ���

	vector<Tank*> hitTankList = {};

	TILE_INFO* tile = nullptr;
	Tank* ownerTank = nullptr;
	Tank* playerTank = nullptr;
	EnemyManager* enemyMgr = nullptr;
	vector<Ammo*>* targetAmmos = {};
	vector<Tank*>* vecEnemyTanks = {};
	vector<Ammo*>::iterator itAmmos = {};
	vector<Tank*>::iterator itEnemyTanks = {};
public:
	HRESULT Init(TILE_INFO* tile, Tank* ownerTank, EnemyManager* enemyMgr = nullptr, Tank* playerTank = nullptr);
	void Update();
	void Render(HDC hdc);
	void Release();

	bool CheckCollision(int idX, int idY);
	bool CheckHitTank(Tank* enemyTank);

	void SetMoveDir(string dir);

	// 2. Setter�� ����
	// this : this�� ȣ��� �Լ��� ȣ���� �ν��Ͻ�(�� �޸��ּ�)
	void SetIsFire(bool fire);
	inline bool GetIsFire() { return this->isFire; }

	inline void SetMoveAngle(float angle) { this->moveAngle = angle; }
	inline void SetTarget(Tank* target) { this->playerTank = target; }
	inline void SetOwnerTank(Tank* tank) { this->ownerTank = tank; }
	//inline void SetPlayerAmmoManager(AmmoManager* playerAmmoManager) { this->playerAmmoManager = playerAmmoManager; }
	//inline void SetEnemyAmmoManager(AmmoManager* enemyAmmoManager) { this->enemyAmmoManager = enemyAmmoManager; }

	inline void SetTargetAmmos(vector<Ammo*>* targetAmmos) { this->targetAmmos = targetAmmos; }
	inline void SetVecEnemyTanks(vector<Tank*>* enemyTanks) { this->vecEnemyTanks = enemyTanks; }

	void SetCollider();

	//inline void SetIsAlive(bool alive) { this->isAlive = alive; }
	//inline bool GetIsAlive() { return this->isAlive; }
	Ammo();
	~Ammo();
};

