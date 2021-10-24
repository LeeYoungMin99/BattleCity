#include "ImageManager.h"
#include "Image.h"

void ImageManager::Init()
{
	//탱크관련
	AddImage("Image/Player/Player.bmp", 256, 128, 8, 4, true, RGB(255, 0, 255));
	AddImage("Image/Effect/Shield.bmp", 64, 32, 2, 1, true, RGB(255, 0, 255));
	AddImage("Image/Effect/Spawn_Effect.bmp", 128, 32, 4, 1, true, RGB(255, 0, 255));
	AddImage("Image/Enemy/Enemy.bmp", 512, 384, 8, 6, true, RGB(255, 0, 255));
	AddImage("Image/Enemy/Enemy_Item.bmp", 128, 128, 8, 8, true, RGB(255, 0, 255));

	//총알관련
	AddImage("Image/Bullet/Missile_Down.bmp", 6, 8, true, RGB(255, 0, 255));
	AddImage("Image/Bullet/Missile_Left.bmp", 6, 8, true, RGB(255, 0, 255));
	AddImage("Image/Bullet/Missile_Right.bmp", 6, 8, true, RGB(255, 0, 255));
	AddImage("Image/Bullet/Missile_Up.bmp", 6, 8, true, RGB(255, 0, 255));
	AddImage("Image/Effect/Boom_Effect.bmp", 96, 32, 3, 1, true, RGB(255, 0, 255));

	//스테이지
	AddImage("Image/Icon/Icon_Enemy.bmp", 16, 16, 1, 1, true, RGB(255, 0, 255));
	AddImage("Image/Icon/player1Life.bmp", 35, 40, 1, 1, true, RGB(255, 0, 255));
	AddImage("Image/Icon/StageFlag.bmp", 35, 40, 1, 1, true, RGB(255, 0, 255));
	AddImage("Image/Text/Number.bmp", 60 /*40*/, 28 /*14*/, 5, 2, true, RGB(255, 0, 255));
	AddImage("Image/mapImage.bmp", 1024, 768, 1, 1, true, RGB(255, 0, 255));
	AddImage("Image/Text/Game_Over.bmp", 64, 30, 1, 1, true, RGB(255, 0, 255));
	AddImage("Image/Effect/Integrated_Boom_Effect.bmp", 320, 64, 5, 1, true, RGB(255, 0, 255));

	//아이템
	AddImage("Image/Item/Item1.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage("Image/Item/Item2.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage("Image/Item/Item3.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage("Image/Item/Item4.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage("Image/Item/Item5.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage("Image/Item/Item6.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage("Image/Item/Item7.bmp", 32, 32, true, RGB(255, 0, 255));

	//점수텍스트
	AddImage("Image/Icon/Point.bmp", 120, 24, 5, 1, true, RGB(255, 0, 255));

	//EnemyManager
	AddImage("Image/Effect/Integrated_Boom_Effect.bmp", 320, 64, 5, 1, true, RGB(255, 0, 255));

	//스코어씬
	AddImage("Image/mapImage.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	AddImage("Image/Text/HISocre.bmp", 235 /*470*/, 25/*50*/, true, RGB(255, 0, 255));
	AddImage("Image/Text/HISocreText.bmp", 133 /*266*/, 25/*50*/, true, RGB(255, 0, 255));
	AddImage("Image/Text/TotalScore.bmp", 180 /*360*/, 20 /*40*/, true, RGB(255, 0, 255));
	AddImage("Image/Text/Stage_w.bmp", 55 /*37*/, 10 /*7*/, true, RGB(255, 0, 255));
	AddImage("Image/Text/Player1.bmp", 115 /*230*/, 20 /*40*/, true, RGB(255, 0, 255));
	AddImage("Image/Text/ScoreNumber.bmp", 60 /*40*/, 21 /*14*/, 5, 2, true, RGB(255, 0, 255));
	AddImage("Image/Text/Number_w.bmp", 60 /*40*/, 21 /*14*/, 5, 2, true, RGB(255, 0, 255));
	AddImage("Image/Enemy/Enemy.bmp", 256 /*512*/, 192 /*384*/, 8, 6, true, RGB(255, 0, 255));
	AddImage("Image/Icon/Arrow.bmp", 12 /*23*/, 10 /*20*/, true, RGB(255, 0, 255));
	AddImage("Image/Text/PTS.bmp", 45 /*90*/, 20 /*40*/, true, RGB(255, 0, 255));
}

void ImageManager::Release()
{
	for (auto& elem : mapImages)
	{
		SAFE_RELEASE(elem.second);
	}

	mapImages.clear();
}

//void ImageManager::AddImage()
//{
//	// 추가하려는 이미지를 자료구조에서 찾는다.
//
//	// 있으면 종료.
//
//	// 없으면 추가한다.
//	//new Image();
//	//img->Init
//
//}

Image* ImageManager::AddImage(const char* fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	if (FindImage(fileName))
	{
		return nullptr;
	}

	Image* img = new Image;
	if (FAILED(img->Init(fileName, width, height, isTrans, transColor)))
	{
		SAFE_RELEASE(img);
		return nullptr;
	}

	mapImages.insert(make_pair(fileName, img));

	return img;
}

Image* ImageManager::AddImage(const char* fileName, int width, int height,
	int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
	if (FindImage(fileName))
	{
		return nullptr;
	}

	Image* img = new Image;
	if (FAILED(img->Init(fileName, width, height, maxFrameX,
		maxFrameY, isTrans, transColor)))
	{
		SAFE_RELEASE(img);
		return nullptr;
	}

	mapImages.insert(make_pair(fileName, img));

	return img;
}

Image* ImageManager::FindImage(const char* fileName)
{
	map<string, Image*>::iterator it = mapImages.find(fileName);
	if (it == mapImages.end())
	{
		return nullptr;
	}

	return it->second;
}

void ImageManager::DeleteImage(const char* fileName)
{
	map<string, Image*>::iterator it = mapImages.find(fileName);
	if (it == mapImages.end())
	{
		return;
	}

	SAFE_RELEASE(it->second);	// Image*	// Image 동적할당 해제
	mapImages.erase(it);		// 맵에서 노드 삭제
}
