#include "Item.h"
#include "Image.h"
#include "Stage1Scene.h"
#include "Stage2Scene.h"
#include "Stage3Scene.h"
#include "Tank.h"
#include "EnemyManager.h"
#include "ItemManager.h"

HRESULT Item::Init(int type, int tile, Tank* tank, EnemyManager* enemyMgr, TILE_INFO* tileInfo, ItemManager* itemManager)
{
	ImageManager::GetSingleton()->AddImage("Image/Item/Item1.bmp", 32, 32, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Item/Item2.bmp", 32, 32, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Item/Item3.bmp", 32, 32, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Item/Item4.bmp", 32, 32, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Item/Item5.bmp", 32, 32, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Item/Item6.bmp", 32, 32, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("Image/Item/Item7.bmp", 32, 32, true, RGB(255, 0, 255));
	itemImage[0] = ImageManager::GetSingleton()->FindImage("Image/Item/Item1.bmp");
	itemImage[1] = ImageManager::GetSingleton()->FindImage("Image/Item/Item2.bmp");
	itemImage[2] = ImageManager::GetSingleton()->FindImage("Image/Item/Item3.bmp");
	itemImage[3] = ImageManager::GetSingleton()->FindImage("Image/Item/Item4.bmp");
	itemImage[4] = ImageManager::GetSingleton()->FindImage("Image/Item/Item5.bmp");
	itemImage[5] = ImageManager::GetSingleton()->FindImage("Image/Item/Item6.bmp");
	itemImage[6] = ImageManager::GetSingleton()->FindImage("Image/Item/Item7.bmp");

	itemType = type;
	itemTile = tile;

	this->tank = tank;
	this->enemyMgr = enemyMgr;
	//this->stageInfo = stageInfo;
	rc.left = ((itemTile % 26) * 16) + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16;
	rc.top = ((itemTile / 26) * 16) + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y;
	rc.right = ((itemTile % 26) * 16) + 32 + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16;
	rc.bottom = ((itemTile / 26) * 16) + 32 + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y;

	this->itemManager = itemManager;

	return S_OK;

}

void Item::Update()
{

}

void Item::Render(HDC hdc)
{
	itemImage[itemType]->Render(hdc,
		((itemTile % 26) * 16) + 32 / 2 + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16,
		((itemTile / 26) * 16) + 32 / 2 + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y);

}

void Item::Release()
{
}

void Item::UseItem()
{
	//switch (itemType)
	//{
	//case 0:		// 헬멧 : 일정시간 플레이어 탱크 무적
	//	cout << "Helmet" << endl;
	//	break;
	//case 1:		// 시계 : 일정시간 적 탱크들 모든 행동멈춤
	//	cout << "Clock" << endl;
	//	enemyMgr->SetClockItem(true);
	//	//((Stage1Scene*)stageInfo)->UseItem(1);
	//	break;
	//case 2:		// 삽 : 일정시간 넥서스 주변 흰색타일로 강화
	//	cout << "Shovel" << endl;
	//	itemManager->Fortification();
	//	break;
	//case 3:		// 별 : 플레이어 탱크 업그레이드
	//	cout << "Star" << endl;
	//	tank->enforceCount++;
	//	break;
	//case 4:		// 수류탄 : 이것은 수류탄이여
	//	cout << "Grenade" << endl;
	//	enemyMgr->BoomItem();
	//	break;
	//case 5:		// 탱크 : 플레이어 목숨 +1 증가
	//	cout << "Tank" << endl;
	//	GameManager::GetSingleton()->player1Life++;
	//	break;
	//case 6:		// 권총 : 모름띠..
	//	cout << "Gun" << endl;
	//	break;
	//}
}

void HelmetItem::UseItem()
{
	GetTank()->bCheckShieldOn = true;
	GetTank()->elapsedCount = 0;
}

void ClockItem::UseItem()
{
	GetEnemyManager()->SetClockItem(true);
}

void ShovelItem::UseItem()
{
	GetItemManager()->Fortification();
}

void StarItem::UseItem()
{
	GetTank()->enforceCount++;
}

void GrenadeItem::UseItem()
{
	GetEnemyManager()->BoomItem();
}

void TankLifeItem::UseItem()
{
	GameManager::GetSingleton()->player1Life++;
}
