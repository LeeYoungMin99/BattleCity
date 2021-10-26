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

	if (GameManager::GetSingleton()->stageLevel < 10)
	{
		stageLevel->Render(hdc, WIN_SIZE_X / 2 + 60, WIN_SIZE_Y / 2 - 10, GameManager::GetSingleton()->stageLevel % 5, GameManager::GetSingleton()->stageLevel / 5);
	}
	else if (GameManager::GetSingleton()->stageLevel >= 10)
	{
		stageLevel->Render(hdc, WIN_SIZE_X / 2 + 60, WIN_SIZE_Y / 2 - 10, GameManager::GetSingleton()->stageLevel / 10, GameManager::GetSingleton()->stageLevel / 50);
		stageLevel->Render(hdc, WIN_SIZE_X / 2 + 72, WIN_SIZE_Y / 2 - 10, (GameManager::GetSingleton()->stageLevel % 10) % 5, (GameManager::GetSingleton()->stageLevel%10) / 5);
	}

}

void LoadingScene::Release()
{
}
