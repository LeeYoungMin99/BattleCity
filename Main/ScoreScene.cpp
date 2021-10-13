#include "ScoreScene.h"
#include "Image.h"

HRESULT ScoreScene::Init()
{
	ImageManager::GetSingleton()->AddImage("Image/mapImage.bmp", WIN_SIZE_X, WIN_SIZE_Y);
	backGround = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");
	
	ImageManager::GetSingleton()->AddImage("Image/Text/HISocre.bmp", 235 /*470*/, 25/*50*/, true, RGB(255, 0, 255));
	hiScore = ImageManager::GetSingleton()->FindImage("Image/Text/HISocre.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Text/TotalScore.bmp", 180 /*360*/, 20 /*40*/, true, RGB(255, 0, 255));
	totalScore = ImageManager::GetSingleton()->FindImage("Image/Text/TotalScore.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Text/Stage_w.bmp", 55 /*37*/, 10 /*7*/, true, RGB(255, 0, 255));
	stage = ImageManager::GetSingleton()->FindImage("Image/Text/Stage_w.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Text/Player1.bmp", 115 /*230*/, 20 /*40*/, true, RGB(255, 0, 255));
	playerScore = ImageManager::GetSingleton()->FindImage("Image/Text/Player1.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Text/Number_w.bmp", 120 /*40*/, 56 /*14*/,	5, 2, true, RGB(255, 0, 255));
	stageNum = ImageManager::GetSingleton()->FindImage("Image/Text/Number_w.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 256 /*512*/, 192 /*384*/, 8, 6, true, RGB(255, 0, 255));
	enemyTank = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Icon/Arrow.bmp", 12 /*23*/, 10 /*20*/,true, RGB(255, 0, 255));
	arrow = ImageManager::GetSingleton()->FindImage("Image/Icon/Arrow.bmp");
	
	ImageManager::GetSingleton()->AddImage("Image/Text/PTS.bmp", 45 /*90*/, 20 /*40*/, true, RGB(255, 0, 255));
	pts = ImageManager::GetSingleton()->FindImage("Image/Text/PTS.bmp");

	/*killNormalEnemy = killSpeedEnemy = killRapidEnemy = killBossEnemy = 9;
	scoreNormalEnemy = scoreSpeedEnemy = scoreRapidEnemy = scoreBossEnemy = 0;
	bScoreNormalEnemy =  bScoreSpeedEnemy = bScoreRapidEnemy = bScoreBossEnemy = false;  변수명 */ 

	KNE = KSE =  KRE = KBE = 5;

	// 디버깅용 
	KNE = 13;
	KSE = 8;
	KRE = 0;
	KBE = 5;

	SNE = SSE = SRE = SBE = 0;
	SNE_1 = SSE_1 = SRE_1 = SBE_1 = 0;
	SNE_10 = SSE_10 = SRE_10 = SBE_10 = 0;
	bSNE = bSSE = bSRE = bSBE = false;

	score = 0;
	elapsedcount = 0;

	return S_OK;
}

void ScoreScene::Update()
{
	bSNE = true;
	if (SNE < KNE)
	{
		elapsedcount++;
		if (elapsedcount >= elapsedTest)
		{
			SNE++;
			SNE_1++;
			elapsedcount = 0;
		}
		if (SNE_1 >= 10)
		{
			SNE_10++;
			SNE_1 = 0;
		}
	}

	if(bSSE && SSE < KSE)
	{
		elapsedcount++;
		if (elapsedcount >= elapsedTest)
		{
			SSE++;
			SSE_1++;
			elapsedcount = 0;
		}
		if (SSE_1 >= 10)
		{
			SSE_10++;
			SSE_1 = 0;
		}
	}

	if (bSRE && SRE < KRE)
	{
		elapsedcount++;
		if (elapsedcount >= elapsedTest)
		{
			SRE++;
			SRE_1++;
			elapsedcount = 0;
		}
		if (SRE_1 >= 10)
		{
			SRE_10++;
			SRE_1 = 0;
		}
	}

	if (bSBE && SBE < KBE)
	{
		elapsedcount++;
		if (elapsedcount >= elapsedTest)
		{
			SBE++;
			SBE_1++;
			elapsedcount = 0;
		}
		if (SBE_1 >= 10)
		{
			SBE_10++;
			SBE_1 = 0;
		}
	}


	if (SNE == KNE)	//노말 점수 끝
	{
		elapsedcount++;
		if (elapsedcount >= 50)
		{
			bSSE = true;
			elapsedcount = 0;
			KNE = 0;
		}
	}
	else if (SSE == KSE)	//두번째 점수 계산끝
	{
		elapsedcount++;
		if (elapsedcount >= 50)
		{
			bSRE = true;
			elapsedcount = 0;
			KSE = 0;
		}
	}
	else if (SRE == KRE)	//세번째 점수 계산 끝
	{
		elapsedcount++;
		if (elapsedcount >= 50)
		{
			bSBE = true;
			elapsedcount = 0;
			KRE = 0;
		}
	}


}

void ScoreScene::Render(HDC hdc)
{
	if (backGround)
		backGround->Render(hdc);

	if (hiScore)
		hiScore->Render(hdc, WIN_SIZE_X/2, WIN_SIZE_Y/6);

	if (totalScore)
		totalScore->Render(hdc, WIN_SIZE_X/3 + 25, WIN_SIZE_Y -350);

	if (playerScore)
		playerScore->Render(hdc, WIN_SIZE_X/4,  WIN_SIZE_Y/4);
	
	if (stage)
		stage->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 5);

	if(stageNum)
		stageNum->Render(hdc, WIN_SIZE_X / 2 + 30, WIN_SIZE_Y / 5, 1, 0);

	if (bSNE)
	{
		stageNum->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 /* + (i * 50)*/, SNE_1 %5, SNE_1 /5);
		if(SNE_10 >=1)
			stageNum->Render(hdc, WIN_SIZE_X / 2 - 75, WIN_SIZE_Y / 3 /* + (i * 50)*/, SNE_10 % 5, SNE_10 / 5);

		//점수 순서대로 1, 10, 100 ,1000 자리
		stageNum->Render(hdc, WIN_SIZE_X / 5+25, WIN_SIZE_Y / 3 ,0, 0);
		if (!SNE == 0)
		{
			stageNum->Render(hdc, WIN_SIZE_X / 5, WIN_SIZE_Y / 3 ,0, 0);
			stageNum->Render(hdc, WIN_SIZE_X / 6, WIN_SIZE_Y / 3 , SNE_1 % 5, SNE_1 / 5);
			if (SNE_10 >= 1)
				stageNum->Render(hdc, WIN_SIZE_X / 6-25, WIN_SIZE_Y / 3, SNE_10 % 5, SNE_10 / 5);
		}

	}
	
	if (bSSE)
	{
		stageNum->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 +50, SSE_1 % 5, SSE_1 / 5);
		if (SSE_10 >= 1)
			stageNum->Render(hdc, WIN_SIZE_X / 2 - 75, WIN_SIZE_Y / 3+50, SSE_10 % 5, SSE_10 / 5);

		//점수 순서대로 1, 10, 100 ,1000 자리
		stageNum->Render(hdc, WIN_SIZE_X / 5 + 25, WIN_SIZE_Y / 3 + 50 , 0, 0);
		if (!SSE == 0)
		{
			stageNum->Render(hdc, WIN_SIZE_X / 5, WIN_SIZE_Y / 3 + 50, 0, 0);
			stageNum->Render(hdc, WIN_SIZE_X / 6, WIN_SIZE_Y / 3 + 50, SSE_1 % 5, SSE_1 / 5);
			if (SSE_10 >= 1)
				stageNum->Render(hdc, WIN_SIZE_X / 6 - 25, WIN_SIZE_Y / 3 + 50, SSE_10 % 5, SSE_10 / 5);
		}
	}

	if (bSRE)
	{
		stageNum->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 + 100, SRE_1 % 5, SRE_1 / 5);
		if (SRE_10 >= 1)
			stageNum->Render(hdc, WIN_SIZE_X / 2 - 75, WIN_SIZE_Y / 3 + 100 , SRE_10 % 5, SRE_10 / 5);

		//점수 순서대로 1, 10, 100 ,1000 자리
		stageNum->Render(hdc, WIN_SIZE_X / 5 + 25, WIN_SIZE_Y / 3 + 100 , 0, 0);
		if (!SRE == 0)
		{
			stageNum->Render(hdc, WIN_SIZE_X / 5, WIN_SIZE_Y / 3 + 100, 0, 0);
			stageNum->Render(hdc, WIN_SIZE_X / 6, WIN_SIZE_Y / 3 + 100, SRE_1 % 5, SRE_1 / 5);
			if (SRE_10 >= 1)
				stageNum->Render(hdc, WIN_SIZE_X / 6 - 25, WIN_SIZE_Y / 3 + 100, SRE_10 % 5, SRE_10 / 5);
		}
	}

	if (bSBE)
	{
		stageNum->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 + 150, SBE_1 % 5, SBE_1 / 5);
		if (SBE_10 >= 1)
			stageNum->Render(hdc, WIN_SIZE_X / 2 - 75, WIN_SIZE_Y / 3 + 150, SBE_10 % 5, SBE_10 / 5);
		
		//점수 순서대로 1, 10, 100 ,1000 자리
		stageNum->Render(hdc, WIN_SIZE_X / 5 + 25, WIN_SIZE_Y / 3 + 150, 0, 0);
		if (!SBE == 0)
		{
			stageNum->Render(hdc, WIN_SIZE_X / 5, WIN_SIZE_Y / 3 + 150, 0, 0);
			stageNum->Render(hdc, WIN_SIZE_X / 6, WIN_SIZE_Y / 3 + 150, SBE_1 % 5, SBE_1 / 5);
			if (SBE_10 >= 1)
				stageNum->Render(hdc, WIN_SIZE_X / 6 - 25, WIN_SIZE_Y / 3 + 150, SBE_10 % 5, SBE_10 / 5);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (enemyTank)
			enemyTank->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 3 + (i * 50), 0, i );
	}

	for (int i = 0; i < 4; i++)
	{
		if(arrow)
			arrow->Render(hdc, WIN_SIZE_X / 2 - 30, WIN_SIZE_Y / 3 + (i * 50));
	}

	for (int i = 0; i < 4; i++)
	{
		pts->Render(hdc, WIN_SIZE_X / 2 - 120, WIN_SIZE_Y / 3 + (i * 50));
	}
}

void ScoreScene::Release()
{
}
