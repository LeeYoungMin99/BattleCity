#include "LoadingScene.h"
#include "Image.h"

HRESULT LoadingScene::Init()
{
	ImageManager::GetSingleton()->AddImage("Image/mapImage.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	backGround = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");
	
	ImageManager::GetSingleton()->AddImage("Image/Text/Number.bmp", 60 /*40*/, 28 /*14*/, 5, 2, true, RGB(255, 0, 255));
	stageLevel = ImageManager::GetSingleton()->FindImage("Image/Text/Number.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Text/Stage.bmp", 74 /*40*/, 14 /*14*/, 5, 2, true, RGB(255, 0, 255));
	stageText = ImageManager::GetSingleton()->FindImage("Image/Text/Stage.bmp");

	if (backGround == nullptr)
		return E_FAIL;

	if (stageLevel == nullptr)
		return E_FAIL;

	if (stageText == nullptr)
		return E_FAIL;


	elapsedCount = 0;

	nextStage = "Stage" + to_string((GameManager::GetSingleton()->stageLevel-1)%3+1);
	return S_OK;
}

void LoadingScene::Update()
{
	elapsedCount++;
	if (elapsedCount >= 100)
	{
		SceneManager::GetSingleton()->ChangeScene(nextStage);
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
		cout << GameManager::GetSingleton()->stageLevel << endl;
		stageLevel->Render(hdc, WIN_SIZE_X / 2 + 60, WIN_SIZE_Y / 2 - 10, GameManager::GetSingleton()->stageLevel / 10, GameManager::GetSingleton()->stageLevel / 50);
		stageLevel->Render(hdc, WIN_SIZE_X / 2 + 72, WIN_SIZE_Y / 2 - 10, (GameManager::GetSingleton()->stageLevel % 10) % 5, (GameManager::GetSingleton()->stageLevel%10) / 5);

	}

}

void LoadingScene::Release()
{

}
