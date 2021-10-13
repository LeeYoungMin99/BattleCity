#include "PixelCollisionScene.h"
#include "Image.h"
#include "CommonFunction.h"
#include "Tank.h"
#include "Ammo.h"

HRESULT PixelCollisionScene::Init()
{
	SetWindowSize(50, 50, PIXELMAP_SIZE_X, PIXELMAP_SIZE_Y);

	ball = ImageManager::GetSingleton()->AddImage("Image/ball.bmp", 60, 60, true, RGB(255, 0, 255));
	mountain = ImageManager::GetSingleton()->AddImage("Image/mountain.bmp", PIXELMAP_SIZE_X, PIXELMAP_SIZE_Y, true, RGB(255, 0, 255));

	posBall.x = PIXELMAP_SIZE_X / 2.0f;
	posBall.y = 100;

	moveSpeed = 0.0f;
	maxMoveSpeed = 200.0f;
	accel = 98.1f;

	tank = new Tank;
	tank->Init(TankType::Player);
	tank->SetPos(POINTFLOAT{ 80.0f,300.0f });

	return S_OK;
}

void PixelCollisionScene::Update()
{
	/*
		1. 방향키 좌우를 써서 볼을 이동시킨다.
		2. 탱크 발사
		3. 미사일 지형 파괴
	*/


	moveSpeed += (accel * TimerManager::GetSingleton()->GetDeltaTime());
	moveSpeed = min(moveSpeed, maxMoveSpeed);

	float tempPosY= posBall.y + moveSpeed * TimerManager::GetSingleton()->GetDeltaTime();
	//posBall.y += moveSpeed * TimerManager::GetSingleton()->GetDeltaTime();
	
	COLORREF color;
	WORD r, g, b;

	color = GetPixel(mountain->GetMemDC(), posBall.x, tempPosY +ball->GetHeight()/2.0f );

	r = GetRValue(color);
	g = GetGValue(color);
	b = GetBValue(color);
	if (!(r == 255 && g == 0 && b == 255))
	{
		posBall.y =posBall.y;
		
	}
	else
	{
		posBall.y = tempPosY;
	}
	
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LEFT))
	{
		color = GetPixel(mountain->GetMemDC(), posBall.x-10, posBall.y + ball->GetHeight() / 2.0f - 5);

		if (!(r == 255 && g == 0 && b == 255))
		{
			posBall.y = posBall.y  - 5 ;
			
		}
		posBall.x -= 100 * TimerManager::GetSingleton()->GetDeltaTime();

	}
	if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RIGHT)) 
	{
		color = GetPixel(mountain->GetMemDC(), posBall.x + 10, posBall.y + ball->GetHeight() / 2.0f - 5);

		if (!(r == 255 && g == 0 && b == 255))
		{
			posBall.y = posBall.y - 5;

		}
		posBall.x += 100 * TimerManager::GetSingleton()->GetDeltaTime();
	}

	tank->Update();


	for (int i = 0; i < 30; i++)
	{
		if (tank->ammoPack[i].GetIsFire())
		{
			color = GetPixel(mountain->GetMemDC(), tank->ammoPack[i].GetPos().x, tank->ammoPack[i].GetPos().y);
			int r = GetRValue(color);
			int g = GetGValue(color);
			int b = GetBValue(color);
			if (!(r == 255 && g == 0 && b == 255))
			{
				hpen = CreatePen(PS_SOLID, 3, RGB(255, 0, 255)); // 선 스타일, 굵기, 색상 
				hbr = CreateSolidBrush(RGB(255, 0, 255)); // 브러쉬 색
				hpenOld = (HPEN)::SelectObject(mountain->GetMemDC(), (HGDIOBJ)hpen ); // 펜 선택 ::
				hpenOld = (HPEN)::SelectObject(mountain->GetMemDC(), (HGDIOBJ)hbr); // 펜 선택 ::


				tank->ammoPack[i].SetIsFire(false);
				Ellipse(mountain->GetMemDC(), tank->ammoPack[i].GetPos().x-20, tank->ammoPack[i].GetPos().y-20, tank->ammoPack[i].GetPos().x + 20, tank->ammoPack[i].GetPos().y + 20);
				
			}
		}
	}
	//if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RETURN))
	//	cout << r << ", " << g << ", " << b << ", " << endl;
}

void PixelCollisionScene::Render(HDC hdc)
{
	PatBlt(hdc, 0, 0, PIXELMAP_SIZE_X, PIXELMAP_SIZE_Y, WHITENESS);

	mountain->Render(hdc, PIXELMAP_SIZE_X / 2, PIXELMAP_SIZE_Y / 2);

	ball->Render(hdc, posBall.x , posBall.y );

	tank->Render(hdc);
}

void PixelCollisionScene::Release()
{
}
