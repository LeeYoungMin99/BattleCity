#include "TitleScene.h"
//#include "Config.h"
#include "Image.h"
#include "Button.h"
#include "ButtonFunction.h"

HRESULT TitleScene::Init()
{
	ImageManager::GetSingleton()->AddImage("Image/Title.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	backGround = ImageManager::GetSingleton()->FindImage("Image/Title.bmp");


	ImageManager::GetSingleton()->AddImage("Image/Player/Player.bmp", 512, 256, 8, 4, true, RGB(255, 0, 255));
	tankUi = ImageManager::GetSingleton()->FindImage("Image/Player/Player.bmp");

	pos.x = 150;
	pos.y = 470;

	// ¸Å°³º¯¼ö (¾î¶² ¾ÀÀ¸·Î, ¾î¶² ·Îµù¾ÀÀ»)
	arg = new ArgumentFuncPtr;
	arg->sceneName = "ÀüÅõ¾À";
	arg->loadingSceneName = "·Îµù¾À";


	return S_OK;
}

void TitleScene::Update()
{
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_SPACE))
	{
		SceneManager::GetSingleton()->ChangeScene("ÀüÅõ¾À", "·Îµù¾À");
		return;
	}

	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_DOWN))
	{
		if (pos.y < 580)
			pos.y += 60;
	}
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_UP))
	{
		if (pos.y > 470)
			pos.y -= 60;
	}


}

void TitleScene::Render(HDC hdc)
{
	if (backGround)
		backGround->Render(hdc);

	if (tankUi)
		tankUi->Render(hdc, pos.x, pos.y, 7, 1);
}

void TitleScene::Release()
{

	SAFE_DELETE(arg);
}
