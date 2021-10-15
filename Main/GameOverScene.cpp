#include "GameOverScene.h"
#include "Image.h"

HRESULT GameOverScene::Init()
{
	ImageManager::GetSingleton()->AddImage("Image/GameOver.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	gameOver = ImageManager::GetSingleton()->FindImage("Image/GameOver.bmp");
	
	ImageManager::GetSingleton()->AddImage("Image/mapImage.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	backGround = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");

	elapsedcount = 0;

	//test_1 = -(backGround->GetHeight())+200;
	//test_2 = (backGround->GetHeight()*2)-200;//backGround->GetHeight();	//´Ý

	test_1 = 0;
	test_2 = WIN_SIZE_Y ;	//¿­
	return S_OK;
}

void GameOverScene::Update()
{
	elapsedcount++;
	if (elapsedcount >= 200)
	{
		SceneManager::GetSingleton()->ChangeScene("Stage1");
	}
	//test_1 += 1;
	//test_2 -= 1;	//´Ý

	test_1 -= 1;
	test_2 += 1;	//¿­

	/*if (test_1 >= 0)
	{
		test_1 = 0;
	}*/

}

void GameOverScene::Render(HDC hdc)
{
	gameOver->Render(hdc);

	backGround->Render(hdc, backGround->GetWidth() / 2, test_1);
	backGround->Render(hdc, backGround->GetWidth() / 2, test_2);
}

void GameOverScene::Release()
{
}
