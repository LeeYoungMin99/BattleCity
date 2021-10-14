#include "TitleScene.h"
//#include "Config.h"
#include "Image.h"
#include "Button.h"
#include "ButtonFunction.h"

HRESULT TitleScene::Init()
{
	ImageManager::GetSingleton()->AddImage("Image/Title.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	backGround = ImageManager::GetSingleton()->FindImage("Image/Title.bmp");


	ImageManager::GetSingleton()->AddImage("Image/Player/Player.bmp", 256/*512*/, 128/*256*/, 8, 4, true, RGB(255, 0, 255));
	tankUi = ImageManager::GetSingleton()->FindImage("Image/Player/Player.bmp");

	pos.x = 150;
	pos.y = 470;

	pos.x = WIN_SIZE_X * 0.3;
	pos.y = WIN_SIZE_Y * 0.59;

	// ¸Å°³º¯¼ö (¾î¶² ¾ÀÀ¸·Î, ¾î¶² ·Îµù¾ÀÀ»)
	arg = new ArgumentFuncPtr;
	arg->sceneName = "ÀüÅõ¾À";
	arg->loadingSceneName = "·Îµù¾À";

	// µð¹ö±ë ¿ë 
	test = WIN_SIZE_Y*1.5;
	titleStart = false;

	selecTitle = selectedTitle::player_1;
	return S_OK;
}

void TitleScene::Update()
{
	if (titleStart)
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_SPACE))
		{
			switch (selecTitle)
			{
			case player_1:
				selecTitle = selectedTitle::player_1;
				SceneManager::GetSingleton()->ChangeScene("Stage1");
				break;
			case player_2:
				selecTitle = selectedTitle::player_1;
				SceneManager::GetSingleton()->ChangeScene("ScoreScene");
				break;
			case CONSTRUCTION:
				selecTitle = selectedTitle::player_2;
				SceneManager::GetSingleton()->ChangeScene("GameOverScene");
				break;
			}
			return;
		}

		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_DOWN))
		{
			if (pos.y < WIN_SIZE_Y * 0.7)
				pos.y += 32;

			switch (selecTitle)
			{
			case player_1:
				selecTitle = selectedTitle::player_2;
				break;
			case player_2:
				selecTitle = selectedTitle::CONSTRUCTION;
				break;
				/*case CONSTRUCTION:
					selecTitle = selectedTitle::CONSTRUCTION;
					break;*/
			}
		}
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_UP))
		{
			if (pos.y > WIN_SIZE_Y * 0.59)
				pos.y -= 32;

			switch (selecTitle)
			{
			case player_1:
				selecTitle = selectedTitle::player_1;
				break;
			case player_2:
				selecTitle = selectedTitle::player_1;
				break;
			case CONSTRUCTION:
				selecTitle = selectedTitle::player_2;
				break;
			}
		}
	}
	// µð¹ö±ë ¿ë
	if (test >= WIN_SIZE_Y / 2)
		test -= 3;
	else
		titleStart = true;

	if (KeyManager::GetSingleton()->IsOnceKeyDown('Q'))
	{
		titleStart = true;
	}

}

void TitleScene::Render(HDC hdc)
{
	/*if (backGround)
		backGround->Render(hdc);*/
	if (!titleStart)
	{
		if (backGround)
			backGround->Render(hdc, backGround->GetWidth() / 2, test);
	}
	else
	{
		if (backGround)
			backGround->Render(hdc);

		if (tankUi)
			tankUi->Render(hdc, pos.x, pos.y, 7, 1);
	}
}

void TitleScene::Release()
{

	SAFE_DELETE(arg);
}
