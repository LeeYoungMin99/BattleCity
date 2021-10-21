#include "ImageManager.h"
#include "Image.h"

void ImageManager::Init()
{
	AddImage("Image/Player/Player.bmp", 256, 128, 8, 4, true, RGB(255, 0, 255));
	AddImage("Image/Effect/Shield.bmp", 64, 32, 2, 1, true, RGB(255, 0, 255));
	AddImage("Image/Effect/Spawn_Effect.bmp", 128, 32, 4, 1, true, RGB(255, 0, 255));
	AddImage("Image/Enemy/Enemy.bmp", 512, 384, 8, 6, true, RGB(255, 0, 255));
	AddImage("Image/Enemy/Enemy_Item.bmp", 128, 128, 8, 8, true, RGB(255, 0, 255));

	//��������
	AddImage("Image/Icon/Icon_Enemy.bmp", 16, 16, 1, 1, true, RGB(255, 0, 255));
	AddImage("Image/Icon/player1Life.bmp", 35, 40, 1, 1, true, RGB(255, 0, 255));
	AddImage("Image/Icon/StageFlag.bmp", 35, 40, 1, 1, true, RGB(255, 0, 255));
	AddImage("Image/Text/Number.bmp", 60 /*40*/, 28 /*14*/, 5, 2, true, RGB(255, 0, 255));
	AddImage("Image/mapImage.bmp", 1024, 768, 1, 1, true, RGB(255, 0, 255));
	AddImage("Image/Text/Game_Over.bmp", 64, 30, 1, 1, true, RGB(255, 0, 255));
	AddImage("Image/Effect/Integrated_Boom_Effect.bmp", 320, 64, 5, 1, true, RGB(255, 0, 255));

	//������
	AddImage("Image/Item/Item1.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage("Image/Item/Item2.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage("Image/Item/Item3.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage("Image/Item/Item4.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage("Image/Item/Item5.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage("Image/Item/Item6.bmp", 32, 32, true, RGB(255, 0, 255));
	AddImage("Image/Item/Item7.bmp", 32, 32, true, RGB(255, 0, 255));

	//�����ؽ�Ʈ
	AddImage("Image/Icon/Point.bmp", 120, 24, 5, 1, true, RGB(255, 0, 255));

	//EnemyManager
	ImageManager::GetSingleton()->AddImage("Image/Effect/Integrated_Boom_Effect.bmp", 320, 64, 5, 1, true, RGB(255, 0, 255));


}

void ImageManager::Release()
{
	map<string, Image*>::iterator it;
	for (it = mapImages.begin(); it != mapImages.end(); /*it++*/)
	{
		SAFE_RELEASE(it->second);
		it = mapImages.erase(it);	// ���� ������ �ּ�(iterator)�� ���ŵȴ�.
	}
	mapImages.clear();
}

//void ImageManager::AddImage()
//{
//	// �߰��Ϸ��� �̹����� �ڷᱸ������ ã�´�.
//
//	// ������ ����.
//
//	// ������ �߰��Ѵ�.
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

	SAFE_RELEASE(it->second);	// Image*	// Image �����Ҵ� ����
	mapImages.erase(it);		// �ʿ��� ��� ����
}
