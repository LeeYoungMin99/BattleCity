#include "ScoreScene.h"
#include "Image.h"

HRESULT ScoreScene::Init()
{
	backGround = ImageManager::GetSingleton()->FindImage("Image/mapImage.bmp");
	if (backGround == nullptr)
	{
		return E_FAIL;
	}

	hiScore = ImageManager::GetSingleton()->FindImage("Image/Text/HISocreText.bmp");
	if (hiScore == nullptr)
	{
		return E_FAIL;
	}

	totalScore = ImageManager::GetSingleton()->FindImage("Image/Text/TotalScore.bmp");
	if (totalScore == nullptr)
	{
		return E_FAIL;
	}

	stage = ImageManager::GetSingleton()->FindImage("Image/Text/Stage_w.bmp");
	if (stage == nullptr)
	{
		return E_FAIL;
	}

	player = ImageManager::GetSingleton()->FindImage("Image/Text/Player1.bmp");
	if (player == nullptr)
	{
		return E_FAIL;
	}

	playerScore = ImageManager::GetSingleton()->FindImage("Image/Text/ScoreNumber.bmp");
	if (playerScore == nullptr)
	{
		return E_FAIL;
	}

	number = ImageManager::GetSingleton()->FindImage("Image/Text/Number_w.bmp");
	if (number == nullptr)
	{
		return E_FAIL;
	}

	enemyTank = ImageManager::GetSingleton()->FindImage("Image/Enemy/Enemy.bmp");
	if (enemyTank == nullptr)
	{
		return E_FAIL;
	}

	arrow = ImageManager::GetSingleton()->FindImage("Image/Icon/Arrow.bmp");
	if (arrow == nullptr)
	{
		return E_FAIL;
	}

	pts = ImageManager::GetSingleton()->FindImage("Image/Text/PTS.bmp");
	if (pts == nullptr)
	{
		return E_FAIL;
	}


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
	highScore = 0;
	return S_OK;
}

void ScoreScene::Update()
{
	if (!bNormalTankScoreFinish)
	{
		highScore = GameManager::GetSingleton()->GetHightScore();
		bNormalTankScoreFinish = true;
		player1Score = killNormalTank + (killSpeedTank * 2) + (killRapidTank * 3) + (killDefensiveTank * 4) + GameManager::GetSingleton()->GetScore() +
			GameManager::GetSingleton()->player1GetItemCount * 5;
		GameManager::GetSingleton()->SetScore(player1Score);

	}
	if (!bTotalScore)
		ScoreCalculate();

	if (highScore < player1Score)
	{
		highScore = player1Score;
		GameManager::GetSingleton()->ScoreSave();
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
				GameManager::GetSingleton()->player1GetItemCount = 0;

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
				GameManager::GetSingleton()->player1GetItemCount = 0;
				if (GameManager::GetSingleton()->player1Score > GameManager::GetSingleton()->GetHightScore())
				{

					GameManager::GetSingleton()->ScoreSave();
				}
				player1Score = 0;
				GameManager::GetSingleton()->SetScore(player1Score);	//?????? ?????? ??????????
				SceneManager::GetSingleton()->ChangeScene("GameOverScene");
			}
			else
			{
				GameManager::GetSingleton()->defeatDefensiveTank = 0;
				GameManager::GetSingleton()->defeatNormalTank = 0;
				GameManager::GetSingleton()->defeatRapidTank = 0;
				GameManager::GetSingleton()->defeatSpeedTank = 0;
				SceneManager::GetSingleton()->ChangeScene("TitleScene");  //??????????????.
			}
		}
	}
}

void ScoreScene::Render(HDC hdc)
{
	int intervalPosX = 0;

	if (backGround)
		backGround->Render(hdc);

	if (hiScore)																//?????????? ???? 
		hiScore->Render(hdc, WIN_SIZE_X / 3, WIN_SIZE_Y / 7);

	//playerScore->Render(hdc, WIN_SIZE_X / 2 + 30, WIN_SIZE_Y / 7, 0, 0);
	int tmpHighScore = highScore;
	tmpHighScore *= 100;
	do
	{
		playerScore->Render(hdc, WIN_SIZE_X / 2 +30 - 10 * intervalPosX++, WIN_SIZE_Y / 7, (tmpHighScore % 10) % 5, (tmpHighScore % 10) / 5);
		tmpHighScore /= 10;
	} while (tmpHighScore != 0);
	

	if (stage)		//???????? ??????
		stage->Render(hdc, WIN_SIZE_X / 2 - 30, WIN_SIZE_Y / 5);

	intervalPosX = 0;
	int tmpRound = round;
	do
	{
		number->Render(hdc, WIN_SIZE_X / 2 + 50 - (intervalPosX++)*12, WIN_SIZE_Y / 5, ((tmpRound) %10) % 5, ((tmpRound) % 10) / 5);
		tmpRound /= 10;
	} while (tmpRound != 0);


	if (player)		//???????? ??????
		player->Render(hdc, WIN_SIZE_X / 5 + 15, WIN_SIZE_Y / 4);

	if (playerScore)
	{
		int tmpPlayer1Score = player1Score;
		
		intervalPosX = 0;
		tmpPlayer1Score *= 100;
		do
		{
			playerScore->Render(hdc, WIN_SIZE_X / 4 + (10 - (intervalPosX++)*10), WIN_SIZE_Y / 4 + 15, ((tmpPlayer1Score) % 10) % 5, (tmpPlayer1Score % 10) / 5);
			tmpPlayer1Score /= 10;

		} while (tmpPlayer1Score != 0);

	}

	for (int i = 0; i < 4; i++)		// ?????? ???? ????
	{
		if (enemyTank)
			enemyTank->Render(hdc, WIN_SIZE_X / 2 + 30, (WIN_SIZE_Y / 3 + 35) + (i * 45), 0, i, 0.5f);
	}

	for (int i = 0; i < 4; i++)		// ?????? ????
	{
		if (arrow)
			arrow->Render(hdc, WIN_SIZE_X / 2 - 20, (WIN_SIZE_Y / 3 + 20) + (i * 45));
	}

	for (int i = 0; i < 4; i++)		// ????(pts) ?????? 
	{
		if (pts)
			pts->Render(hdc, WIN_SIZE_X / 2 - 90, (WIN_SIZE_Y / 3 + 20) + (i * 45));
	}

	if (totalScore)		//?????????? ??????
		totalScore->Render(hdc, (int)(WIN_SIZE_X / 3 + 35), (int)(WIN_SIZE_Y * 0.8 - 15));

	if (bTotalScore)	//???? ???? ????
	{
		intervalPosX = 0;
		int tmpTotalKill = totalKill;
		do
		{
			number->Render(hdc, WIN_SIZE_X / 3 + 40 - 10 * intervalPosX++, WIN_SIZE_Y *0.8-15, (tmpTotalKill % 10) % 5, (tmpTotalKill % 10) / 5);
			tmpTotalKill /= 10;
		} while (tmpTotalKill != 0);
	}

	// ???? ????
	if (bNormalTankScoreFinish)		// ????
	{
		// ???? ?????? ????

		intervalPosX = 0;
		int tmpNormalTankCount = normalTankCount;
		do
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40 - (intervalPosX++ *10), WIN_SIZE_Y / 3 + 20, (tmpNormalTankCount % 10) % 5, (tmpNormalTankCount % 10) / 5);
			tmpNormalTankCount /= 10;
		} while (tmpNormalTankCount != 0);

		//???? ????

		intervalPosX = 0;
		int tmpnormalTankCount = normalTankCount*100;
		do
		{
			number->Render(hdc, WIN_SIZE_X / 4 - 10- (12 * intervalPosX++), WIN_SIZE_Y / 3 + 20, (tmpnormalTankCount %10) % 5, (tmpnormalTankCount % 10) / 5);
			tmpnormalTankCount /= 10;
		} while (tmpnormalTankCount != 0);
	}

	if (bSpeedTankScoreFinish)		// ??????
	{
		// ???? ?????? ????
		intervalPosX = 0;
		int tmpSpeedTankCount = speedTankCount;
		do
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40 - (intervalPosX++ * 10), (WIN_SIZE_Y / 3 + 20) + 45, (tmpSpeedTankCount % 10) % 5, (tmpSpeedTankCount % 10) / 5);
			tmpSpeedTankCount /= 10;
		} while (tmpSpeedTankCount != 0);


		intervalPosX = 0;
		int tmpSpeedTankScore = speedTankScore * 100;
		do
		{
			number->Render(hdc, WIN_SIZE_X / 4 - 10-(12 * intervalPosX++), (WIN_SIZE_Y / 3 + 20) + 45, (tmpSpeedTankScore % 10) % 5, (tmpSpeedTankScore % 10) / 5);
			tmpSpeedTankScore /= 10;
		} while (tmpSpeedTankScore != 0);
	}

	if (bRapidTankScoreFinish)		// ??????
	{
		// ???? ?????? ????
		intervalPosX = 0;
		int tmpRapidTankCount = rapidTankCount;
		do
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40 - (intervalPosX++ * 10), (WIN_SIZE_Y / 3 + 20) + 90, (tmpRapidTankCount % 10) % 5, (tmpRapidTankCount % 10) / 5);
			tmpRapidTankCount /= 10;
		} while (tmpRapidTankCount != 0);

		// ???? ????
		intervalPosX = 0;
		int tmpRapidTankScore = rapidTankScore * 100;
		do
		{
			number->Render(hdc, WIN_SIZE_X / 4 - 10 -(12 * intervalPosX++), (WIN_SIZE_Y / 3 + 20) + 90, (tmpRapidTankScore % 10) % 5, (tmpRapidTankScore % 10) / 5);
			tmpRapidTankScore /= 10;
		} while (tmpRapidTankScore != 0);
	}

	if (bDefensiveTankScoreFinish)		//????????
	{
		// ???? ?????? ????
		intervalPosX = 0;
		int tmpDefensiveTankCount = defensiveTankCount;
		do
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40 - (intervalPosX++ * 10), (WIN_SIZE_Y / 3 + 20) + 135, (tmpDefensiveTankCount % 10) % 5, (tmpDefensiveTankCount % 10) / 5);
			tmpDefensiveTankCount /= 10;
		} while (tmpDefensiveTankCount != 0);

		//???? ????
		// ???? ????
		intervalPosX = 0;
		int tmpDefensivaTanKScore = defensiveTankScore * 100;
		do
		{
			number->Render(hdc, WIN_SIZE_X / 4 - 10 - (12 * intervalPosX++), (WIN_SIZE_Y / 3 + 20) + 135, (tmpDefensivaTanKScore % 10) % 5, (tmpDefensivaTanKScore % 10) / 5);
			tmpDefensivaTanKScore /= 10;
		} while (tmpDefensivaTanKScore != 0);
	}
}

void ScoreScene::Release()
{
}

void ScoreScene::ScoreCalculate()
{

	elapsedcount++;
	if (normalTankCount < killNormalTank)		//???? ????
	{
		if (elapsedcount >= elapsedTest)
		{
			normalTankCount++;
			elapsedcount = 0;
		}
	}

	if (bSpeedTankScoreFinish && speedTankCount < killSpeedTank)	//?????? ????
	{
		if (elapsedcount >= elapsedTest)
		{
			speedTankCount++;
			speedTankScore += 2;
			elapsedcount = 0;
		}
	}

	if (bRapidTankScoreFinish && rapidTankCount < killRapidTank)	//?????? ????
	{
		if (elapsedcount >= elapsedTest)
		{
			rapidTankCount++;
			elapsedcount = 0;
			rapidTankScore += 3;
		}
	}

	if (bDefensiveTankScoreFinish && defensiveTankCount < killDefensiveTank)	//???? ????
	{
		if (elapsedcount >= elapsedTest)
		{
			defensiveTankCount++;
			elapsedcount = 0;
			defensiveTankScore += 4;
		}
	}


	if (normalTankCount == killNormalTank && bSpeedTankScoreFinish == false)	//???? ???? ??
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bSpeedTankScoreFinish = true;
			elapsedcount = 0;
		}
	}
	else if (speedTankCount == killSpeedTank && bRapidTankScoreFinish == false)	//?????? ???? ??????
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bRapidTankScoreFinish = true;
			elapsedcount = 0;
		}
	}
	else if (rapidTankCount == killRapidTank && bDefensiveTankScoreFinish == false)	//?????? ???? ???? ??
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
