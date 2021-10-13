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
	player = ImageManager::GetSingleton()->FindImage("Image/Text/Player1.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Text/ScoreNumber.bmp", 80 /*40*/, 28 /*14*/, 5, 2, true, RGB(255, 0, 255));
	playerScore = ImageManager::GetSingleton()->FindImage("Image/Text/ScoreNumber.bmp");
	
	ImageManager::GetSingleton()->AddImage("Image/Text/Number_w.bmp", 120 /*40*/, 56 /*14*/,	5, 2, true, RGB(255, 0, 255));
	number = ImageManager::GetSingleton()->FindImage("Image/Text/Number_w.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Enemy/Enemy.bmp", 256 /*512*/, 192 /*384*/, 8, 6, true, RGB(255, 0, 255));
	enemyTank = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");

	ImageManager::GetSingleton()->AddImage("Image/Icon/Arrow.bmp", 12 /*23*/, 10 /*20*/,true, RGB(255, 0, 255));
	arrow = ImageManager::GetSingleton()->FindImage("Image/Icon/Arrow.bmp");
	
	ImageManager::GetSingleton()->AddImage("Image/Text/PTS.bmp", 45 /*90*/, 20 /*40*/, true, RGB(255, 0, 255));
	pts = ImageManager::GetSingleton()->FindImage("Image/Text/PTS.bmp");

	/*killNormalEnemy = killSpeedEnemy = killRapidEnemy = killBossEnemy = 9;
	scoreNormalEnemy = scoreSpeedEnemy = scoreRapidEnemy = scoreBossEnemy = 0;
	bScoreNormalEnemy =  bScoreSpeedEnemy = bScoreRapidEnemy = bScoreBossEnemy = false;  ������ */ 

	
	// ������ 
	KNE = 15;
	KSE = 14;
	KRE = 13;
	KBE = 11;
	CNE = CSE = CRE = CBE = 0;
	SNE = SSE = SRE = SBE = 0;
	bSNE = bSSE = bSRE = bSBE = false;
	bTotalScore = false;
	TK_1 = TK_10 = 0;
	elapsedcount = 0;
	round = 9;
	return S_OK;
}

void ScoreScene::Update()
{
	bSNE = true;
	if (CNE < KNE)		//�Ϲ� ��ũ
	{
		elapsedcount++;
		if (elapsedcount >= elapsedTest)
		{
			CNE++;
			elapsedcount = 0;
		}
	}

	if(bSSE && CSE < KSE)	//���ǵ� ��ũ
	{
		elapsedcount++;
		if (elapsedcount >= elapsedTest)
		{
			CSE++;
			SSE += 2;
			elapsedcount = 0;
		}
	}

	if (bSRE && CRE < KRE)	//���ǵ� ��ũ
	{
		elapsedcount++;
		if (elapsedcount >= elapsedTest)
		{
			CRE++;
			elapsedcount = 0;
			SRE += 3;
		}
	}

	if (bSBE && CBE < KBE)	//���� ��ũ
	{
		elapsedcount++;
		if (elapsedcount >= elapsedTest)
		{
			CBE++;
			elapsedcount = 0;
			SBE += 4;
		}
	}


	if (CNE == KNE && bSSE == false)	//�븻 ���� ��
	{
		elapsedcount++;
		if (elapsedcount >= 50)
		{
			bSSE = true;
			elapsedcount = 0;
		}
	}
	else if (CSE == KSE && bSRE == false)	//�ι�° ���� ��곡
	{
		elapsedcount++;
		if (elapsedcount >= 50)
		{
			bSRE = true;
			elapsedcount = 0;
		}
	}
	else if (CRE == KRE && bSBE== false)	//����° ���� ��� ��
	{
		elapsedcount++;
		if (elapsedcount >= 50)
		{
			bSBE = true;
			elapsedcount = 0;
		}
	}
	else if (CBE == KBE && bTotalScore==false)
	{
		elapsedcount++;
		if (elapsedcount >= 50)
		{
			bTotalScore = true;
			elapsedcount = 0;
			TK_1 = (KNE + KSE + KRE + KBE) % 10;
			TK_10 = (KNE + KSE + KRE + KBE) / 10;
		}
		
	}

	if (bTotalScore)
	{
		elapsedcount++;
		if (elapsedcount >= 100)
		{
			SceneManager::GetSingleton()->ChangeScene("GameOverScene");
		}
	}
	
}

void ScoreScene::Render(HDC hdc)
{
	if (backGround)
		backGround->Render(hdc);	//ȸ��ȭ�� ��׶���

	if (hiScore)
		hiScore->Render(hdc, WIN_SIZE_X/2, WIN_SIZE_Y/6);				//���̽��ھ� �ؽ�Ʈ

	if (totalScore)
		totalScore->Render(hdc, WIN_SIZE_X/3 + 25, WIN_SIZE_Y -350);	//��Ż���ھ� �ؽ�Ʈ

	if (bTotalScore)													//��Ż ���� ų��
	{
		if (number)
			number->Render(hdc, WIN_SIZE_X / 3 + 50, WIN_SIZE_Y - 325, TK_1%5, TK_1/5);
		if (number)
			number->Render(hdc, WIN_SIZE_X / 3 + 25, WIN_SIZE_Y - 325, TK_10%5, TK_10/5);
	}
	
		
	if (player)
		player->Render(hdc, WIN_SIZE_X/4,  WIN_SIZE_Y/4);			//�÷��̾� �ؽ�Ʈ

	if (playerScore)
	{
		playerScore->Render(hdc, WIN_SIZE_X / 4+40, WIN_SIZE_Y / 4 + 25, 0, 0);	//�÷��̾� ���� ���� 1,10...10000�ڸ�
		playerScore->Render(hdc, WIN_SIZE_X / 4+20, WIN_SIZE_Y / 4 + 25, 0, 0);
		playerScore->Render(hdc, WIN_SIZE_X / 4, WIN_SIZE_Y / 4 + 25, 0, 0);
		playerScore->Render(hdc, WIN_SIZE_X / 4-20, WIN_SIZE_Y / 4 + 25, 0, 0);
		playerScore->Render(hdc, WIN_SIZE_X / 4-40, WIN_SIZE_Y / 4 + 25, 0, 0);
	}
	
	if (stage)
		stage->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 5);		//�������� �ؽ�Ʈ
		
	if(number)
		number->Render(hdc, WIN_SIZE_X / 2 + 30, WIN_SIZE_Y / 5, round%5, round/5);	//�������� �ѹ� �ؽ�Ʈ

	for (int i = 0; i < 4; i++)		// ���ʹ� ��ũ �������� ����
	{
		if (enemyTank)
			enemyTank->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 3 + (i * 50), 0, i );
	}

	for (int i = 0; i < 4; i++)		// ȭ��ǥ ����
	{
		if(arrow)
			arrow->Render(hdc, WIN_SIZE_X / 2 - 30, WIN_SIZE_Y / 3 + (i * 50));
	}

	for (int i = 0; i < 4; i++)		// ����(pts) �ؽ�Ʈ 
	{
		if(pts)
			pts->Render(hdc, WIN_SIZE_X / 2 - 120, WIN_SIZE_Y / 3 + (i * 50));
	}


	// ���� ���
	if (bSNE)		//�븻
	{
		// ���� ������ ǥ��
		if (CNE < 10)	//1���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 /* + (i * 50)*/, CNE % 5, CNE / 5);
		}
		else //10���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 /* + (i * 50)*/, (CNE %10) % 5, (CNE%10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 75, WIN_SIZE_Y / 3 /* + (i * 50)*/, (CNE /10) % 5, (CNE/10) / 5);
		}

		//����-> ������� 1, 10, 100 ,1000 �ڸ�
		number->Render(hdc, WIN_SIZE_X / 5 + 25, WIN_SIZE_Y / 3, 0, 0);
		if (!CNE == 0)
		{
			number->Render(hdc, WIN_SIZE_X / 5, WIN_SIZE_Y / 3, 0, 0);
			if (CNE < 10)
			{
				number->Render(hdc, WIN_SIZE_X / 6, WIN_SIZE_Y / 3, CNE % 5, CNE / 5);
			}
			else
			{
				number->Render(hdc, WIN_SIZE_X / 6, WIN_SIZE_Y / 3, (CNE %10) % 5, (CNE %10) / 5);
				number->Render(hdc, WIN_SIZE_X / 6 - 25, WIN_SIZE_Y / 3, (CNE /10) % 5, (CNE /10) / 5);
			}
				
		}

	}

	if (bSSE)		//���ǵ�
	{
		// ���� ������ ǥ��
		if (CSE < 10)	//1���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 +50 /* + (i * 50)*/, CSE % 5, CSE / 5);
		}
		else //10���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 +50/* + (i * 50)*/, (CSE % 10) % 5, (CSE % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 75, WIN_SIZE_Y / 3	+50/* + (i * 50)*/, (CSE / 10) % 5, (CSE / 10) / 5);
		}

		//���� ������� 1, 10, 100 ,1000 �ڸ�
		number->Render(hdc, WIN_SIZE_X / 5 + 25, WIN_SIZE_Y / 3 + 50, 0, 0);
		if (!CSE == 0)
		{
			number->Render(hdc, WIN_SIZE_X / 5, WIN_SIZE_Y / 3 + 50, 0, 0);
			if (SSE < 10)
			{
				number->Render(hdc, WIN_SIZE_X / 6, WIN_SIZE_Y / 3 + 50, SSE % 5, SSE / 5);
			}
			else
			{
				number->Render(hdc, WIN_SIZE_X / 6, WIN_SIZE_Y / 3 + 50, (SSE%10) % 5, (SSE%10) / 5);
				number->Render(hdc, WIN_SIZE_X / 6 - 25, WIN_SIZE_Y / 3 + 50, (SSE/10) % 5, (SSE/10) / 5);
			}
		}
	}

	if (bSRE)		//���ǵ�
	{
		// ���� ������ ǥ��
		if (CRE < 10)	//1���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 + 100 /* + (i * 50)*/, CRE % 5, CRE / 5);
		}
		else //10���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 + 100/* + (i * 50)*/, (CRE % 10) % 5, (CRE % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 75, WIN_SIZE_Y / 3 + 100 /* + (i * 50)*/, (CRE / 10) % 5, (CRE / 10) / 5);
		}

		//���� ������� 1, 10, 100 ,1000 �ڸ�
		number->Render(hdc, WIN_SIZE_X / 5 + 25, WIN_SIZE_Y / 3 + 100, 0, 0);
		if (!CRE == 0)
		{
			number->Render(hdc, WIN_SIZE_X / 5, WIN_SIZE_Y / 3 + 100, 0, 0);
			
			if (SRE < 9)
			{
				number->Render(hdc, WIN_SIZE_X / 6, WIN_SIZE_Y / 3 + 100, SRE % 5, SRE / 5);
			}
			else
			{
				number->Render(hdc, WIN_SIZE_X / 6, WIN_SIZE_Y / 3 + 100, (SRE%10) % 5, (SRE%10) / 5);
				number->Render(hdc, WIN_SIZE_X / 6 - 25, WIN_SIZE_Y / 3 + 100, (SRE/10) % 5, (SRE/10) / 5);
			}
				
		}
	}

	if (bSBE)		//����
	{
		// ���� ������ ǥ��
		if (CBE < 10)	//1���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 + 150 /* + (i * 50)*/, CBE % 5, CBE / 5);
		}
		else //10���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 + 150 /* + (i * 50)*/, (CBE % 10) % 5, (CBE % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 75, WIN_SIZE_Y / 3 + 150 /* + (i * 50)*/, (CBE / 10) % 5, (CBE / 10) / 5);
		}

		//���� ������� 1, 10, 100 ,1000 �ڸ�
		number->Render(hdc, WIN_SIZE_X / 5 + 25, WIN_SIZE_Y / 3 + 150, 0, 0);
		if (!CBE == 0)
		{
			number->Render(hdc, WIN_SIZE_X / 5, WIN_SIZE_Y / 3 + 150, 0, 0);
			
			if (SBE < 9)
			{
				number->Render(hdc, WIN_SIZE_X / 6, WIN_SIZE_Y / 3 + 150, SBE % 5, SBE / 5);
			}
			else
			{
				number->Render(hdc, WIN_SIZE_X / 6, WIN_SIZE_Y / 3 + 150, (SBE%10) % 5, (SBE%10) / 5);
				number->Render(hdc, WIN_SIZE_X / 6 - 25, WIN_SIZE_Y / 3 + 150, (SBE/10) % 5, (SBE/10) / 5);
			}
				
		}
	}
}

void ScoreScene::Release()
{
}
