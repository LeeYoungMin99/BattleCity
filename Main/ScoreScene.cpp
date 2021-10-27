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
				GameManager::GetSingleton()->SetScore(player1Score);	//게임이 끝나면 점수초기화
				SceneManager::GetSingleton()->ChangeScene("GameOverScene");
			}
			else
			{
				GameManager::GetSingleton()->defeatDefensiveTank = 0;
				GameManager::GetSingleton()->defeatNormalTank = 0;
				GameManager::GetSingleton()->defeatRapidTank = 0;
				GameManager::GetSingleton()->defeatSpeedTank = 0;
				SceneManager::GetSingleton()->ChangeScene("TitleScene");  //다음스테이지씬.
			}
		}
	}
}

void ScoreScene::Render(HDC hdc)
{
	if (backGround)
		backGround->Render(hdc);

	if (hiScore)																//하이스코어 점수 
		hiScore->Render(hdc, WIN_SIZE_X / 3, WIN_SIZE_Y / 7);

	playerScore->Render(hdc, WIN_SIZE_X / 2 + 30, WIN_SIZE_Y / 7, 0, 0);
	if (highScore >= 1)
	{
		playerScore->Render(hdc, WIN_SIZE_X / 2 + 20, WIN_SIZE_Y / 7, 0, 0);
		playerScore->Render(hdc, WIN_SIZE_X / 2 + 10, WIN_SIZE_Y / 7, (highScore % 10) % 5, (highScore % 10) / 5);
		if (highScore >= 10)
			playerScore->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 7, ((highScore % 100) / 10) % 5, ((highScore % 100) / 10) / 5);
		if (highScore >= 100)
		{
			playerScore->Render(hdc, WIN_SIZE_X / 2 - 10, WIN_SIZE_Y / 7, (highScore / 100) % 5, (highScore / 100) / 5);
		}
	}


	if (stage)		//스테이지 텍스트
		stage->Render(hdc, WIN_SIZE_X / 2 - 30, WIN_SIZE_Y / 5);

	if (round < 10)
	{
		if (number)	//스테이지 넘버 텍스트
			number->Render(hdc, WIN_SIZE_X / 2 + 50, WIN_SIZE_Y / 5, round % 5, round / 5);
	}
	else if (round >= 10)
	{
		number->Render(hdc, WIN_SIZE_X / 2 + 50, WIN_SIZE_Y / 5, round / 10, round / 50);
		number->Render(hdc, WIN_SIZE_X / 2 + 62, WIN_SIZE_Y / 5, (round % 10) % 5, (round % 10) / 5);

	}

	if (player)		//플레이어 텍스트
		player->Render(hdc, WIN_SIZE_X / 5 + 15, WIN_SIZE_Y / 4);

	if (playerScore)
	{
		playerScore->Render(hdc, WIN_SIZE_X / 4 + 30, WIN_SIZE_Y / 4 + 15, 0, 0);	//플레이어 점수
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

	for (int i = 0; i < 4; i++)		// 에너미 탱크 랜더
	{
		if (enemyTank)
			enemyTank->Render(hdc, WIN_SIZE_X / 2 + 30, (WIN_SIZE_Y / 3 + 35) + (i * 45), 0, i, 0.5f);
	}

	for (int i = 0; i < 4; i++)		// 화살표 랜더
	{
		if (arrow)
			arrow->Render(hdc, WIN_SIZE_X / 2 - 20, (WIN_SIZE_Y / 3 + 20) + (i * 45));
	}

	for (int i = 0; i < 4; i++)		// 점수(pts) 텍스트 
	{
		if (pts)
			pts->Render(hdc, WIN_SIZE_X / 2 - 90, (WIN_SIZE_Y / 3 + 20) + (i * 45));
	}

	if (totalScore)		//토탈스코어 텍스트
		totalScore->Render(hdc, (int)(WIN_SIZE_X / 3 + 35), (int)(WIN_SIZE_Y * 0.8 - 15));

	if (bTotalScore)	//토탈 누적 킬수
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

	// 점수 계산
	if (bNormalTankScoreFinish)		// 노말
	{
		// 잡은 마리수 표기
		if (normalTankCount < 10)
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, WIN_SIZE_Y / 3 + 20, normalTankCount % 5, normalTankCount / 5);
		}
		else
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, WIN_SIZE_Y / 3 + 20, (normalTankCount % 10) % 5, (normalTankCount % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 + 20, (normalTankCount / 10) % 5, (normalTankCount / 10) / 5);
		}

		//점수 표기
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

	if (bSpeedTankScoreFinish)		// 스피드
	{
		// 잡은 마리수 표기
		if (speedTankCount < 10)
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 45, speedTankCount % 5, speedTankCount / 5);
		}
		else
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 45, (speedTankCount % 10) % 5, (speedTankCount % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, (WIN_SIZE_Y / 3 + 20) + 45, (speedTankCount / 10) % 5, (speedTankCount / 10) / 5);
		}

		//점수 표기
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

	if (bRapidTankScoreFinish)		// 래피드
	{
		// 잡은 마리수 표기
		if (rapidTankCount < 10)
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 90, rapidTankCount % 5, rapidTankCount / 5);
		}
		else
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 90, (rapidTankCount % 10) % 5, (rapidTankCount % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, (WIN_SIZE_Y / 3 + 20) + 90, (rapidTankCount / 10) % 5, (rapidTankCount / 10) / 5);
		}

		// 점수 표기
		number->Render(hdc, WIN_SIZE_X / 4 + 10, (WIN_SIZE_Y / 3 + 20) + 90, 0, 0);
		if (!rapidTankCount == 0)
		{
			number->Render(hdc, WIN_SIZE_X / 4, (WIN_SIZE_Y / 3 + 20) + 90, 0, 0);

			if (rapidTankScore < 10)
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

	if (bDefensiveTankScoreFinish)		//디펜시브
	{
		// 잡은 마리수 표기
		if (defensiveTankCount < 10)
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 135, defensiveTankCount % 5, defensiveTankCount / 5);
		}
		else
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 135, (defensiveTankCount % 10) % 5, (defensiveTankCount % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, (WIN_SIZE_Y / 3 + 20) + 135, (defensiveTankCount / 10) % 5, (defensiveTankCount / 10) / 5);
		}

		//점수 표기
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
	if (normalTankCount < killNormalTank)		//일반 탱크
	{
		if (elapsedcount >= elapsedTest)
		{
			normalTankCount++;
			elapsedcount = 0;
		}
	}

	if (bSpeedTankScoreFinish && speedTankCount < killSpeedTank)	//스피드 탱크
	{
		if (elapsedcount >= elapsedTest)
		{
			speedTankCount++;
			speedTankScore += 2;
			elapsedcount = 0;
		}
	}

	if (bRapidTankScoreFinish && rapidTankCount < killRapidTank)	//레피드 탱크
	{
		if (elapsedcount >= elapsedTest)
		{
			rapidTankCount++;
			elapsedcount = 0;
			rapidTankScore += 3;
		}
	}

	if (bDefensiveTankScoreFinish && defensiveTankCount < killDefensiveTank)	//보스 탱크
	{
		if (elapsedcount >= elapsedTest)
		{
			defensiveTankCount++;
			elapsedcount = 0;
			defensiveTankScore += 4;
		}
	}


	if (normalTankCount == killNormalTank && bSpeedTankScoreFinish == false)	//노말 점수 끝
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bSpeedTankScoreFinish = true;
			elapsedcount = 0;
		}
	}
	else if (speedTankCount == killSpeedTank && bRapidTankScoreFinish == false)	//두번째 점수 계산끝
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bRapidTankScoreFinish = true;
			elapsedcount = 0;
		}
	}
	else if (rapidTankCount == killRapidTank && bDefensiveTankScoreFinish == false)	//세번째 점수 계산 끝
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
