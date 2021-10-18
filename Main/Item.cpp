#include "Item.h"
#include "Image.h"

HRESULT Item::Init(int type, int tile)
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

	rc.left = ((itemTile % 26) * 16) + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16;
	rc.top = ((itemTile / 26) * 16) + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y;
	rc.right = ((itemTile % 26) * 16) + 32 + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16;
	rc.bottom = ((itemTile / 26) * 16) + 32 + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y;

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
	
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
	//cout << rc.left << endl;
	//cout << rc.top << endl;
	//cout << rc.right << endl;
	//cout << rc.bottom<< endl;

}

void Item::Release()
{
}
