#include "LoadingScene.h"
#include "Image.h"

HRESULT LoadingScene::Init()
{
	backGround = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");
	if (backGround == nullptr)
	{
		return E_FAIL;
	}
	
	stageLevel = ImageManager::GetSingleton()->FindImage("Image/Text/Number.bmp");
	if (stageLevel == nullptr)
	{
		return E_FAIL;
	}

	stageText = ImageManager::GetSingleton()->FindImage("Image/Text/Stage.bmp");
	if (stageText == nullptr)
	{
		return E_FAIL;
	}

	elapsedCount = 0;

	return S_OK;
}

void LoadingScene::Update()
{
	elapsedCount++;
	if (elapsedCount >= 100)
	{
		SceneManager::GetSingleton()->ChangeScene("Stage");
		return;
	}


}

void LoadingScene::Render(HDC hdc)
{

	backGround->Render(hdc);


	stageText->Render(hdc, WIN_SIZE_X / 2 - 30, WIN_SIZE_Y / 2 - 10);

	int tmpStageLevel = GameManager::GetSingleton()->stageLevel;
	int intervalPosX = 0;
	do
	{
		stageLevel->Render(hdc, WIN_SIZE_X / 2 + 60 - (12*intervalPosX++), WIN_SIZE_Y / 2 - 10, (tmpStageLevel % 10) % 5, (tmpStageLevel % 10) / 5);
		tmpStageLevel /= 10;
	} while (tmpStageLevel != 0);
}

void LoadingScene::Release()
{
}
