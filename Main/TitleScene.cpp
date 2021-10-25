#include "TitleScene.h"
#include "Image.h"



HRESULT TitleScene::Init()
{
	ImageManager::GetSingleton()->AddImage("Image/Title.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	title = ImageManager::GetSingleton()->FindImage("Image/Title.bmp");

	ImageManager::GetSingleton()->AddImage("Image/mapImage.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	backGround = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Player/Player.bmp", 256/*512*/, 128/*256*/, 8, 4, true, RGB(255, 0, 255));
	tankUi = ImageManager::GetSingleton()->FindImage("Image/Player/Player.bmp");

	pos.x = 150;
	pos.y = 470;

	pos.x = (LONG)(WIN_SIZE_X * 0.3);
	pos.y = (LONG)(WIN_SIZE_Y * 0.59);

	// �Ű����� (� ������, � �ε�����)
	arg = DBG_NEW ArgumentFuncPtr;
	arg->sceneName = "������";
	arg->loadingSceneName = "�ε���";

	// ����� �� 
	slidePos = (int)(WIN_SIZE_Y * 1.5);	//�����̵� ȿ��
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
			case selectedTitle::player_1:
				selecTitle = selectedTitle::player_1;
				SceneManager::GetSingleton()->ChangeScene("LoadingScene");
				break;
			case selectedTitle::player_2:
				selecTitle = selectedTitle::player_1;
				break;
			case selectedTitle::CONSTRUCTION:
				selecTitle = selectedTitle::player_2;
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
			case selectedTitle::player_1:
				selecTitle = selectedTitle::player_2;
				break;
			case selectedTitle::player_2:
				selecTitle = selectedTitle::CONSTRUCTION;
				break;
			case selectedTitle::CONSTRUCTION:
				selecTitle = selectedTitle::CONSTRUCTION;
				break;
			}
		}
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_UP))
		{
			if (pos.y > WIN_SIZE_Y * 0.59)
				pos.y -= 32;

			switch (selecTitle)
			{
			case selectedTitle::player_1:
				selecTitle = selectedTitle::player_1;
				break;
			case selectedTitle::player_2:
				selecTitle = selectedTitle::player_1;
				break;
			case selectedTitle::CONSTRUCTION:
				selecTitle = selectedTitle::player_2;
				break;
			}
		}
	}
	// ����� ��
	if (slidePos >= WIN_SIZE_Y / 2)
		slidePos -= 3;
	else
		titleStart = true;

	if (KeyManager::GetSingleton()->IsOnceKeyDown('Q'))
	{
		titleStart = true;
	}

}

void TitleScene::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, WIN_SIZE_X, WIN_SIZE_Y, BLACKNESS);

	if (!titleStart)
	{
		if (title)
			title->Render(hdc, title->GetWidth() / 2, slidePos);
	}
	else
	{
		if (title)
			title->Render(hdc);

		if (tankUi)
			tankUi->Render(hdc, pos.x, pos.y, 7, 1);
	}
}

void TitleScene::Release()
{
	SAFE_DELETE(arg);
}
