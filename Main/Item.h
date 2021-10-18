#pragma once
#include "Config.h"
#include "GameObject.h"

class Image;
class Item : public GameObject
{
private:
	Image* itemImage[7];
	int itemType = 0;
	int itemTile = 0;


public:
	RECT rc;
	HRESULT Init(int type, int tile);
	void Update();
	void Render(HDC hdc);
	void Release();
};

