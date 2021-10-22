#include "ScoreScene.h"
#include "Image.h"

HRESULT ScoreScene::Init()
{
	backGround = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");
	noneHiScore = ImageManager::GetSingleton()->FindImage("Image/Text/HISocre.bmp");
	hiScore = ImageManager::GetSingleton()->FindImage("Image/Text/HISocreText.bmp");
	totalScore = ImageManager::GetSingleton()->FindImage("Image/Text/TotalScore.bmp");
	stage = ImageManager::GetSingleton()->FindImage("Image/Text/Stage_w.bmp");
	player = ImageManager::GetSingleton()->FindImage("Image/Text/Player1.bmp");
	playerScore = ImageManager::GetSingleton()->FindImage("Image/Text/ScoreNumber.bmp");
	number = ImageManager::GetSingleton()->FindImage("Image/Text/Number_w.bmp");
	enemyTank = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");
	arrow = ImageManager::GetSingleton()->FindImage("Image/Icon/Arrow.bmp");
	pts = ImageManager::GetSingleton()->FindImage("Image/Text/PTS.bmp");


	KNE = GameManager::GetSingleton()->defeatNormalTank;
	KSE = GameManager::GetSingleton()->defeatSpeedTank;
	KRE = GameManager::GetSingleton()->defeatRapidTank;
	KBE = GameManager::GetSingleton()->defeatDefensiveTank;
	round = GameManager::GetSingleton()->stageLevel;
	
	CNE = CSE = CRE = CBE = 0;
	SNE = SSE = SRE = SBE = 0;
	bSNE = bSSE = bSRE = bSBE = false;
	bTotalScore = false;
	gameOver = false;
	TK = 0;
	elapsedcount = 0;
	player1Score = 0;
	hightScore = 0;
	return S_OK;
}

void ScoreScene::Update()
{
	if (!bSNE)
	{
		hightScore = GameManager::GetSingleton()->GetHightScore();		
		
		bSNE = true; 
		player1Score = KNE + (KSE * 2) + (KRE * 3) + (KBE * 4) + GameManager::GetSingleton()->GetScore();
	}

	if (!bTotalScore)
		ScoreCalculate();

	if (hightScore < player1Score)
	{
		hightScore = player1Score;
	}
	
	if (bTotalScore)
	{
		elapsedcount++;
		if (elapsedcount >= 100)
		{	
			if (GameManager::GetSingleton()->player1Life >= 0 && GameManager::GetSingleton()->state != GameState::GameOver)
			{

				GameManager::GetSingleton()->player1Score = player1Score;

				GameManager::GetSingleton()->defeatDefensiveTank = 0;
				GameManager::GetSingleton()->defeatNormalTank = 0;
				GameManager::GetSingleton()->defeatRapidTank = 0;
				GameManager::GetSingleton()->defeatSpeedTank = 0;

				if (GameManager::GetSingleton()->player1Score > GameManager::GetSingleton()->GetHightScore())
				{
					GameManager::GetSingleton()->ScoreSave();
				}
				SceneManager::GetSingleton()->ChangeScene("Stage");
				return;
			}

			if (GameManager::GetSingleton()->player1Life < 0 || GameManager::GetSingleton()->state == GameState::GameOver)
				gameOver = true;


			if (gameOver)
			{
				GameManager::GetSingleton()->defeatDefensiveTank = 0;
				GameManager::GetSingleton()->defeatNormalTank = 0;
				GameManager::GetSingleton()->defeatRapidTank = 0;
				GameManager::GetSingleton()->defeatSpeedTank = 0;
				if (GameManager::GetSingleton()->player1Score > GameManager::GetSingleton()->GetHightScore())
				{

					GameManager::GetSingleton()->ScoreSave();
				}
				player1Score = 0;	
				GameManager::GetSingleton()->SetScore(player1Score);	//������ ������ �����ʱ�ȭ
				SceneManager::GetSingleton()->ChangeScene("GameOverScene");
			}
			else
			{
				GameManager::GetSingleton()->defeatDefensiveTank = 0;
				GameManager::GetSingleton()->defeatNormalTank = 0;
				GameManager::GetSingleton()->defeatRapidTank = 0;
				GameManager::GetSingleton()->defeatSpeedTank = 0;
				SceneManager::GetSingleton()->ChangeScene("TitleScene");  //��������������.
			}
		}
	}
}

void ScoreScene::Render(HDC hdc)
{
	if (backGround)
		backGround->Render(hdc);	//ȸ��ȭ�� ��׶���

	if (hightScore < 1)
	{
		if (noneHiScore)
			noneHiScore->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 7);				//���̽��ھ� �ؽ�Ʈ (����Ʈ)
	}
	else
	{
		if (hiScore)																//���̽��ھ� ���� ��.
			hiScore->Render(hdc, WIN_SIZE_X / 3, WIN_SIZE_Y/7);

		playerScore->Render(hdc, WIN_SIZE_X / 2 + 30, WIN_SIZE_Y / 7, 0, 0);		//���̽��ھ� ���� 1,10...10000�ڸ�
		if (hightScore >= 1)
		{
			playerScore->Render(hdc, WIN_SIZE_X / 2 + 20, WIN_SIZE_Y / 7, 0, 0);
			playerScore->Render(hdc, WIN_SIZE_X / 2 + 10, WIN_SIZE_Y / 7, (hightScore % 10) % 5, (hightScore % 10) / 5);
			if(hightScore >=10)
				playerScore->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 7, ((hightScore % 100) / 10) % 5, ((hightScore % 100) / 10) / 5);
			if (hightScore >= 100)
			{
				playerScore->Render(hdc, WIN_SIZE_X / 2 - 10, WIN_SIZE_Y / 7, (hightScore / 100) % 5, (hightScore / 100) / 5);
			}
		}
	}


	if (stage)
		stage->Render(hdc, WIN_SIZE_X / 2 -30 , WIN_SIZE_Y / 5);		//�������� �ؽ�Ʈ

	if (round < 10)
	{
		if (number)
			number->Render(hdc, WIN_SIZE_X / 2 + 50, WIN_SIZE_Y / 5, round % 5, round / 5);	//�������� �ѹ� �ؽ�Ʈ
	}
	else if (round >= 10)
	{
		number->Render(hdc, WIN_SIZE_X / 2 + 50, WIN_SIZE_Y / 5, round / 10, round / 50);
		number->Render(hdc, WIN_SIZE_X / 2 + 62, WIN_SIZE_Y / 5, (round % 10) % 5, (round % 10) / 5);

	}

	if (player)
		player->Render(hdc, WIN_SIZE_X / 5 + 15, WIN_SIZE_Y / 4);			//�÷��̾� �ؽ�Ʈ

	if (playerScore)
	{
		playerScore->Render(hdc, WIN_SIZE_X / 4+30, WIN_SIZE_Y / 4 + 15, 0, 0);	//�÷��̾� ���� ���� 1,10...10000�ڸ�
		if (player1Score >= 1)
		{
			playerScore->Render(hdc, WIN_SIZE_X / 4 + 20, WIN_SIZE_Y / 4 + 15, 0, 0);
			playerScore->Render(hdc, WIN_SIZE_X / 4 + 10, WIN_SIZE_Y / 4 + 15, (player1Score % 10) % 5, (player1Score % 10) / 5);
			if(player1Score >=10)
				playerScore->Render(hdc, WIN_SIZE_X / 4, WIN_SIZE_Y / 4 + 15, ((player1Score % 100) / 10) % 5, ((player1Score % 100) / 10) / 5);
			if (player1Score >= 100)
				playerScore->Render(hdc, WIN_SIZE_X / 4 - 10, WIN_SIZE_Y / 4 + 15, (player1Score / 100) % 5, (player1Score / 100) / 5);
		}
	}

	for (int i = 0; i < 4; i++)		// ���ʹ� ��ũ �������� ����
	{
		if (enemyTank)
			enemyTank->Render(hdc, WIN_SIZE_X / 2 + 30, (WIN_SIZE_Y / 3 + 35) + (i * 45), 0, i, 0.5f);
	}

	for (int i = 0; i < 4; i++)		// ȭ��ǥ ����
	{
		if (arrow)
			arrow->Render(hdc, WIN_SIZE_X / 2 - 20, (WIN_SIZE_Y / 3 + 20) + (i * 45));
	}

	for (int i = 0; i < 4; i++)		// ����(pts) �ؽ�Ʈ 
	{
		if (pts)
			pts->Render(hdc, WIN_SIZE_X / 2 - 90, (WIN_SIZE_Y / 3 + 20) + (i * 45));
	}

	if (totalScore)
		totalScore->Render(hdc, WIN_SIZE_X/3 + 35, WIN_SIZE_Y *0.8 - 15 );	//��Ż���ھ� �ؽ�Ʈ

	if (bTotalScore)													//��Ż ���� ų��
	{
		if (TK < 10)
		{
			number->Render(hdc, WIN_SIZE_X / 3 + 40, WIN_SIZE_Y * 0.8 - 15 , TK %5, TK /5);
		}
		else
		{
			number->Render(hdc, WIN_SIZE_X / 3 + 40, WIN_SIZE_Y * 0.8 - 15 , (TK%10) % 5, (TK%10) / 5);
			number->Render(hdc, WIN_SIZE_X / 3 + 25, WIN_SIZE_Y * 0.8 - 15 , (TK/10) % 5, (TK/10) / 5);
		}
	}

	// ���� ���
	if (bSNE)		//�븻
	{
		// ���� ������ ǥ��
		if (CNE < 10)	//1���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, WIN_SIZE_Y / 3 + 20, CNE % 5, CNE / 5);
		}
		else //10���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, WIN_SIZE_Y / 3 + 20, (CNE %10) % 5, (CNE%10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 + 20, (CNE /10) % 5, (CNE/10) / 5);
		}

		//����-> ������� 1, 10, 100 ,1000 �ڸ�
		number->Render(hdc, WIN_SIZE_X / 4 + 10, WIN_SIZE_Y / 3 + 20, 0, 0);
		if (!CNE == 0)
		{
			number->Render(hdc, WIN_SIZE_X / 4 , WIN_SIZE_Y / 3 + 20, 0, 0);
			if (CNE < 10)
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10 , WIN_SIZE_Y / 3 + 20, CNE % 5, CNE / 5);
			}
			else
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, WIN_SIZE_Y / 3 + 20, (CNE %10) % 5, (CNE %10) / 5);
				number->Render(hdc, WIN_SIZE_X / 4 - 22, WIN_SIZE_Y / 3 + 20, (CNE /10) % 5, (CNE /10) / 5);
			}
				
		}

	}

	if (bSSE)		//���ǵ�
	{
		// ���� ������ ǥ��
		if (CSE < 10)	//1���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) +45 , CSE % 5, CSE / 5);
		}
		else //10���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) +45, (CSE % 10) % 5, (CSE % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, (WIN_SIZE_Y / 3 + 20) +45, (CSE / 10) % 5, (CSE / 10) / 5);
		}

		//���� ������� 1, 10, 100 ,1000 �ڸ�
		number->Render(hdc, WIN_SIZE_X / 4 + 10, (WIN_SIZE_Y / 3 + 20) + 45, 0, 0);
		if (!CSE == 0)
		{
			number->Render(hdc, WIN_SIZE_X / 4 , (WIN_SIZE_Y / 3 + 20) + 45, 0, 0);
			if (SSE < 10)
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, (WIN_SIZE_Y / 3 + 20) + 45, SSE % 5, SSE / 5);
			}
			else
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, (WIN_SIZE_Y / 3 + 20) + 45, (SSE%10) % 5, (SSE%10) / 5);
				number->Render(hdc, WIN_SIZE_X / 4 - 22, (WIN_SIZE_Y / 3 + 20) + 45, (SSE/10) % 5, (SSE/10) / 5);
			}
		}
	}

	if (bSRE)		//���ǵ�
	{
		// ���� ������ ǥ��
		if (CRE < 10)	//1���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 90, CRE % 5, CRE / 5);
		}
		else //10���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 90, (CRE % 10) % 5, (CRE % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, (WIN_SIZE_Y / 3 + 20) + 90, (CRE / 10) % 5, (CRE / 10) / 5);
		}

		//���� ������� 1, 10, 100 ,1000 �ڸ�
		number->Render(hdc, WIN_SIZE_X / 4 + 10, (WIN_SIZE_Y / 3 + 20) + 90, 0, 0);
		if (!CRE == 0)
		{
			number->Render(hdc, WIN_SIZE_X / 4, (WIN_SIZE_Y / 3 + 20) + 90, 0, 0);
			
			if (SRE < 9)
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, (WIN_SIZE_Y / 3 + 20) + 90, SRE % 5, SRE / 5);
			}
			else
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, (WIN_SIZE_Y / 3 + 20) + 90, (SRE%10) % 5, (SRE%10) / 5);
				number->Render(hdc, WIN_SIZE_X / 4 - 22, (WIN_SIZE_Y / 3 + 20) + 90, (SRE/10) % 5, (SRE/10) / 5);
			}
				
		}
	}

	if (bSBE)		//����
	{
		// ���� ������ ǥ��
		if (CBE < 10)	//1���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 135, CBE % 5, CBE / 5);
		}
		else //10���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 135, (CBE % 10) % 5, (CBE % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, (WIN_SIZE_Y / 3 + 20) + 135, (CBE / 10) % 5, (CBE / 10) / 5);
		}

		//���� ������� 1, 10, 100 ,1000 �ڸ�
		number->Render(hdc, WIN_SIZE_X / 4 + 10, (WIN_SIZE_Y / 3 + 20) + 135, 0, 0);
		if (!CBE == 0)
		{
			number->Render(hdc, WIN_SIZE_X / 4 , (WIN_SIZE_Y / 3 + 20) + 135, 0, 0);
			
			if (SBE < 9)
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, (WIN_SIZE_Y / 3 + 20) + 135, SBE % 5, SBE / 5);
			}
			else
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, (WIN_SIZE_Y / 3 + 20) + 135, (SBE%10) % 5, (SBE%10) / 5);
				number->Render(hdc, WIN_SIZE_X / 4 - 22, (WIN_SIZE_Y / 3 + 20) + 135, (SBE/10) % 5, (SBE/10) / 5);
			}
				
		}
	}
}

void ScoreScene::Release()
{
}

void ScoreScene::ScoreCalculate()
{
	
	elapsedcount++;
	if (CNE < KNE)		//�Ϲ� ��ũ
	{
		if (elapsedcount >= elapsedTest)
		{
			CNE++;
			elapsedcount = 0;
		}
	}

	if (bSSE && CSE < KSE)	//���ǵ� ��ũ
	{
		if (elapsedcount >= elapsedTest)
		{
			CSE++;
			SSE += 2;
			elapsedcount = 0;
		}
	}

	if (bSRE && CRE < KRE)	//���ǵ� ��ũ
	{
		if (elapsedcount >= elapsedTest)
		{
			CRE++;
			elapsedcount = 0;
			SRE += 3;
		}
	}

	if (bSBE && CBE < KBE)	//���� ��ũ
	{
		if (elapsedcount >= elapsedTest)
		{
			CBE++;
			elapsedcount = 0;
			SBE += 4;
		}
	}


	if (CNE == KNE && bSSE == false)	//�븻 ���� ��
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bSSE = true;
			elapsedcount = 0;
		}
	}
	else if (CSE == KSE && bSRE == false)	//�ι�° ���� ��곡
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bSRE = true;
			elapsedcount = 0;
		}
	}
	else if (CRE == KRE && bSBE == false)	//����° ���� ��� ��
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bSBE = true;
			elapsedcount = 0;
		}
	}
	else if (CBE == KBE && bTotalScore == false)
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bTotalScore = true;
			elapsedcount = 0;
			TK = KNE + KSE + KRE + KBE;
		}

	}
}
