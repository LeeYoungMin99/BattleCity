#include "GameOverScene.h"
#include "Image.h"

HRESULT GameOverScene::Init()
{
	ImageManager::GetSingleton()->AddImage("Image/GameOver.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	gameOver = ImageManager::GetSingleton()->FindImage("Image/GameOver.bmp");
	
	ImageManager::GetSingleton()->AddImage("Image/mapImage.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	backGround = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");

	elapsedcount = 0;

	//overSlatePos = -(backGround->GetHeight())+200;
	//lowSlatePos = (backGround->GetHeight()*2)-200;//backGround->GetHeight();	//´Ý

	overSlatePos = 0;
	lowSlatePos = WIN_SIZE_Y ;	//¿­
	return S_OK;
}

void GameOverScene::Update()
{
	elapsedcount++;
	if (elapsedcount >= 400)
	{
		GameManager::GetSingleton()->GameInit();
		SceneManager::GetSingleton()->ChangeScene("TitleScene");
		return;
	}
	//overSlatePos += 1;
	//lowSlatePos -= 1;	//´Ý

	overSlatePos -= 3;
	lowSlatePos += 3;	//¿­

	/*if (overSlatePos >= 0)
	{
		overSlatePos = 0;
	}*/

}

void GameOverScene::Render(HDC hdc)
{
	gameOver->Render(hdc);

	backGround->Render(hdc, backGround->GetWidth() / 2, overSlatePos);
	backGround->Render(hdc, backGround->GetWidth() / 2, lowSlatePos);
}

void GameOverScene::Release()
{
}
