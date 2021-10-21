#include "AmmoManager.h"
#include "Ammo.h"
#include "GameObject.h"
#include "Tank.h"

HRESULT AmmoManager::Init(TILE_INFO* tileInfo, Tank* playerTank, vector<Tank*>* enemyTanks)
{
	vecAmmos.resize(ammoMaxCount);

	this->tileInfo = tileInfo;
	this->playerTank = playerTank;
	this->enemyTanks = enemyTanks;

	for (itAmmos = vecAmmos.begin();
		itAmmos != vecAmmos.end(); itAmmos++)
	{
		(*itAmmos) = new Ammo;
		(*itAmmos)->Init(tileInfo, nullptr, enemyTanks, playerTank);
	}

	return S_OK;
}

void AmmoManager::Update()
{
	for (itAmmos = vecAmmos.begin();
		itAmmos != vecAmmos.end(); itAmmos++)
	{
		(*itAmmos)->Update();
	}
}

void AmmoManager::Render(HDC hdc)
{
	for (itAmmos = vecAmmos.begin();
		itAmmos != vecAmmos.end(); itAmmos++)
	{
		(*itAmmos)->Render(hdc);
	}
}

void AmmoManager::Release()
{
	for (itAmmos = vecAmmos.begin();
		itAmmos != vecAmmos.end(); itAmmos++)
	{
		SAFE_RELEASE((*itAmmos));
	}
	vecAmmos.clear();
}

void AmmoManager::Fire(Tank* tank, float moveSpeed)
{
	for (itAmmos = vecAmmos.begin();
		itAmmos != vecAmmos.end(); itAmmos++)
	{
		if ((*itAmmos)->GetIsFire())	continue;

		(*itAmmos)->SetOwnerTank(tank);
		(*itAmmos)->SetTargetAmmos(&(tank->GetTargetAmmoManager()->vecAmmos));
		(*itAmmos)->SetMoveSpeed(moveSpeed);

		switch (tank->GetMoveDir())
		{
		case MoveDir::Left:
			tank->SetBarrelPos({ tank->GetPos().x - tank->GetBodySize() / 2 + 3, tank->GetPos().y - tank->GetBodySize() / 4 });
			(*itAmmos)->SetMoveDir("Left");
			break;
		case MoveDir::Right:
			tank->SetBarrelPos({ tank->GetPos().x,  tank->GetPos().y - tank->GetBodySize() / 4 });
			(*itAmmos)->SetMoveDir("Right");
			break;
		case MoveDir::Up:
			tank->SetBarrelPos({ tank->GetPos().x - tank->GetBodySize() / 4,  tank->GetPos().y - tank->GetBodySize() / 2 });
			(*itAmmos)->SetMoveDir("Up");
			break;
		case MoveDir::Down:
			tank->SetBarrelPos({ tank->GetPos().x - tank->GetBodySize() / 4,  tank->GetPos().y });
			(*itAmmos)->SetMoveDir("Down");
			break;
		default:
			break;
		}

		//ammoPack[i].SetIsAlive(true);
		(*itAmmos)->SetPos(tank->GetBarrelPos());	// 미사일 위치 변경
		(*itAmmos)->SetIsFire(true);	// 미사일 상태 변경

		break;
	}
}
