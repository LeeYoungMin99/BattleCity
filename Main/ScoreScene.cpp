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


	killNormalTank = GameManager::GetSingleton()->defeatNormalTank;
	killSpeedTank = GameManager::GetSingleton()->defeatSpeedTank;
	killRapidTank = GameManager::GetSingleton()->defeatRapidTank;
	killDefensiveTank = GameManager::GetSingleton()->defeatDefensiveTank;
	round = GameManager::GetSingleton()->stageLevel;

	normalTankCount = speedTankCount = rapidTankCount = defensiveTankCount = 0;
	normalTankScore = speedTankScore = rapidTankScore = defensiveTankScore = 0;
	bNormalTankScoreFinish = bSpeedTankScoreFinish = bRapidTankScoreFinish = bDefensiveTankScoreFinish = false;
	bTotalScore = false;
	bGameOver = false;
	totalKill = 0;
	elapsedcount = 0;
	player1Score = 0;
	hightScore = 0;
	return S_OK;
}

void ScoreScene::Update()
{
	if (!bNormalTankScoreFinish)
	{
		hightScore = GameManager::GetSingleton()->GetHightScore();

		bNormalTankScoreFinish = true;
		player1Score = killNormalTank + (killSpeedTank * 2) + (killRapidTank * 3) + (killDefensiveTank * 4) + GameManager::GetSingleton()->GetScore();
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
				bGameOver = true;


			if (bGameOver)
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
			hiScore->Render(hdc, WIN_SIZE_X / 3, WIN_SIZE_Y / 7);

		playerScore->Render(hdc, WIN_SIZE_X / 2 + 30, WIN_SIZE_Y / 7, 0, 0);		//���̽��ھ� ���� 1,10...10000�ڸ�
		if (hightScore >= 1)
		{
			playerScore->Render(hdc, WIN_SIZE_X / 2 + 20, WIN_SIZE_Y / 7, 0, 0);
			playerScore->Render(hdc, WIN_SIZE_X / 2 + 10, WIN_SIZE_Y / 7, (hightScore % 10) % 5, (hightScore % 10) / 5);
			if (hightScore >= 10)
				playerScore->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 7, ((hightScore % 100) / 10) % 5, ((hightScore % 100) / 10) / 5);
			if (hightScore >= 100)
			{
				playerScore->Render(hdc, WIN_SIZE_X / 2 - 10, WIN_SIZE_Y / 7, (hightScore / 100) % 5, (hightScore / 100) / 5);
			}
		}
	}


	if (stage)
		stage->Render(hdc, WIN_SIZE_X / 2 - 30, WIN_SIZE_Y / 5);		//�������� �ؽ�Ʈ

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
		playerScore->Render(hdc, WIN_SIZE_X / 4 + 30, WIN_SIZE_Y / 4 + 15, 0, 0);	//�÷��̾� ���� ���� 1,10...10000�ڸ�
		if (player1Score >= 1)
		{
			playerScore->Render(hdc, WIN_SIZE_X / 4 + 20, WIN_SIZE_Y / 4 + 15, 0, 0);
			playerScore->Render(hdc, WIN_SIZE_X / 4 + 10, WIN_SIZE_Y / 4 + 15, (player1Score % 10) % 5, (player1Score % 10) / 5);
			if (player1Score >= 10)
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
		totalScore->Render(hdc, (int)(WIN_SIZE_X / 3 + 35), (int)(WIN_SIZE_Y * 0.8 - 15));	//��Ż���ھ� �ؽ�Ʈ

	if (bTotalScore)													//��Ż ���� ų��
	{
		if (totalKill < 10)
		{
			number->Render(hdc, (int)(WIN_SIZE_X / 3 + 40), (int)(WIN_SIZE_Y * 0.8 - 15), totalKill % 5, totalKill / 5);
		}
		else
		{
			number->Render(hdc, (int)(WIN_SIZE_X / 3 + 40), (int)(WIN_SIZE_Y * 0.8 - 15), (totalKill % 10) % 5, (totalKill % 10) / 5);
			number->Render(hdc, (int)(WIN_SIZE_X / 3 + 25), (int)(WIN_SIZE_Y * 0.8 - 15), (totalKill / 10) % 5, (totalKill / 10) / 5);
		}
	}

	// ���� ���
	if (bNormalTankScoreFinish)		//�븻
	{
		// ���� ������ ǥ��
		if (normalTankCount < 10)	//1���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, WIN_SIZE_Y / 3 + 20, normalTankCount % 5, normalTankCount / 5);
		}
		else //10���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, WIN_SIZE_Y / 3 + 20, (normalTankCount % 10) % 5, (normalTankCount % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 + 20, (normalTankCount / 10) % 5, (normalTankCount / 10) / 5);
		}

		//����-> ������� 1, 10, 100 ,1000 �ڸ�
		number->Render(hdc, WIN_SIZE_X / 4 + 10, WIN_SIZE_Y / 3 + 20, 0, 0);
		if (!normalTankCount == 0)
		{
			number->Render(hdc, WIN_SIZE_X / 4, WIN_SIZE_Y / 3 + 20, 0, 0);
			if (normalTankCount < 10)
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, WIN_SIZE_Y / 3 + 20, normalTankCount % 5, normalTankCount / 5);
			}
			else
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, WIN_SIZE_Y / 3 + 20, (normalTankCount % 10) % 5, (normalTankCount % 10) / 5);
				number->Render(hdc, WIN_SIZE_X / 4 - 22, WIN_SIZE_Y / 3 + 20, (normalTankCount / 10) % 5, (normalTankCount / 10) / 5);
			}

		}

	}

	if (bSpeedTankScoreFinish)		//���ǵ�
	{
		// ���� ������ ǥ��
		if (speedTankCount < 10)	//1���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 45, speedTankCount % 5, speedTankCount / 5);
		}
		else //10���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 45, (speedTankCount % 10) % 5, (speedTankCount % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, (WIN_SIZE_Y / 3 + 20) + 45, (speedTankCount / 10) % 5, (speedTankCount / 10) / 5);
		}

		//���� ������� 1, 10, 100 ,1000 �ڸ�
		number->Render(hdc, WIN_SIZE_X / 4 + 10, (WIN_SIZE_Y / 3 + 20) + 45, 0, 0);
		if (!speedTankCount == 0)
		{
			number->Render(hdc, WIN_SIZE_X / 4, (WIN_SIZE_Y / 3 + 20) + 45, 0, 0);
			if (speedTankScore < 10)
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, (WIN_SIZE_Y / 3 + 20) + 45, speedTankScore % 5, speedTankScore / 5);
			}
			else
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, (WIN_SIZE_Y / 3 + 20) + 45, (speedTankScore % 10) % 5, (speedTankScore % 10) / 5);
				number->Render(hdc, WIN_SIZE_X / 4 - 22, (WIN_SIZE_Y / 3 + 20) + 45, (speedTankScore / 10) % 5, (speedTankScore / 10) / 5);
			}
		}
	}

	if (bRapidTankScoreFinish)		//���ǵ�
	{
		// ���� ������ ǥ��
		if (rapidTankCount < 10)	//1���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 90, rapidTankCount % 5, rapidTankCount / 5);
		}
		else //10���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 90, (rapidTankCount % 10) % 5, (rapidTankCount % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, (WIN_SIZE_Y / 3 + 20) + 90, (rapidTankCount / 10) % 5, (rapidTankCount / 10) / 5);
		}

		//���� ������� 1, 10, 100 ,1000 �ڸ�
		number->Render(hdc, WIN_SIZE_X / 4 + 10, (WIN_SIZE_Y / 3 + 20) + 90, 0, 0);
		if (!rapidTankCount == 0)
		{
			number->Render(hdc, WIN_SIZE_X / 4, (WIN_SIZE_Y / 3 + 20) + 90, 0, 0);

			if (rapidTankScore < 9)
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, (WIN_SIZE_Y / 3 + 20) + 90, rapidTankScore % 5, rapidTankScore / 5);
			}
			else
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, (WIN_SIZE_Y / 3 + 20) + 90, (rapidTankScore % 10) % 5, (rapidTankScore % 10) / 5);
				number->Render(hdc, WIN_SIZE_X / 4 - 22, (WIN_SIZE_Y / 3 + 20) + 90, (rapidTankScore / 10) % 5, (rapidTankScore / 10) / 5);
			}

		}
	}

	if (bDefensiveTankScoreFinish)		//����
	{
		// ���� ������ ǥ��
		if (defensiveTankCount < 10)	//1���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 135, defensiveTankCount % 5, defensiveTankCount / 5);
		}
		else //10���ڸ�
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 135, (defensiveTankCount % 10) % 5, (defensiveTankCount % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, (WIN_SIZE_Y / 3 + 20) + 135, (defensiveTankCount / 10) % 5, (defensiveTankCount / 10) / 5);
		}

		//���� ������� 1, 10, 100 ,1000 �ڸ�
		number->Render(hdc, WIN_SIZE_X / 4 + 10, (WIN_SIZE_Y / 3 + 20) + 135, 0, 0);
		if (!defensiveTankCount == 0)
		{
			number->Render(hdc, WIN_SIZE_X / 4, (WIN_SIZE_Y / 3 + 20) + 135, 0, 0);

			if (defensiveTankScore < 9)
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, (WIN_SIZE_Y / 3 + 20) + 135, defensiveTankScore % 5, defensiveTankScore / 5);
			}
			else
			{
				number->Render(hdc, WIN_SIZE_X / 4 - 10, (WIN_SIZE_Y / 3 + 20) + 135, (defensiveTankScore % 10) % 5, (defensiveTankScore % 10) / 5);
				number->Render(hdc, WIN_SIZE_X / 4 - 22, (WIN_SIZE_Y / 3 + 20) + 135, (defensiveTankScore / 10) % 5, (defensiveTankScore / 10) / 5);
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
	if (normalTankCount < killNormalTank)		//�Ϲ� ��ũ
	{
		if (elapsedcount >= elapsedTest)
		{
			normalTankCount++;
			elapsedcount = 0;
		}
	}

	if (bSpeedTankScoreFinish && speedTankCount < killSpeedTank)	//���ǵ� ��ũ
	{
		if (elapsedcount >= elapsedTest)
		{
			speedTankCount++;
			speedTankScore += 2;
			elapsedcount = 0;
		}
	}

	if (bRapidTankScoreFinish && rapidTankCount < killRapidTank)	//���ǵ� ��ũ
	{
		if (elapsedcount >= elapsedTest)
		{
			rapidTankCount++;
			elapsedcount = 0;
			rapidTankScore += 3;
		}
	}

	if (bDefensiveTankScoreFinish && defensiveTankCount < killDefensiveTank)	//���� ��ũ
	{
		if (elapsedcount >= elapsedTest)
		{
			defensiveTankCount++;
			elapsedcount = 0;
			defensiveTankScore += 4;
		}
	}


	if (normalTankCount == killNormalTank && bSpeedTankScoreFinish == false)	//�븻 ���� ��
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bSpeedTankScoreFinish = true;
			elapsedcount = 0;
		}
	}
	else if (speedTankCount == killSpeedTank && bRapidTankScoreFinish == false)	//�ι�° ���� ��곡
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bRapidTankScoreFinish = true;
			elapsedcount = 0;
		}
	}
	else if (rapidTankCount == killRapidTank && bDefensiveTankScoreFinish == false)	//����° ���� ��� ��
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bDefensiveTankScoreFinish = true;
			elapsedcount = 0;
		}
	}
	else if (defensiveTankCount == killDefensiveTank && bTotalScore == false)
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bTotalScore = true;
			elapsedcount = 0;
			totalKill = killNormalTank + killSpeedTank + killRapidTank + killDefensiveTank;
		}

	}
}
