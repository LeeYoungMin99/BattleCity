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
		backGround->Render(hdc);	//회색화면 백그라운드

	if (hightScore < 1)
	{
		if (noneHiScore)
			noneHiScore->Render(hdc, WIN_SIZE_X / 2, WIN_SIZE_Y / 7);				//하이스코어 텍스트 (디폴트)
	}
	else
	{
		if (hiScore)																//하이스코어 있을 때.
			hiScore->Render(hdc, WIN_SIZE_X / 3, WIN_SIZE_Y/7);

		playerScore->Render(hdc, WIN_SIZE_X / 2 + 30, WIN_SIZE_Y / 7, 0, 0);		//하이스코어 점수 1,10...10000자리
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
		stage->Render(hdc, WIN_SIZE_X / 2 -30 , WIN_SIZE_Y / 5);		//스테이지 텍스트

	if (round < 10)
	{
		if (number)
			number->Render(hdc, WIN_SIZE_X / 2 + 50, WIN_SIZE_Y / 5, round % 5, round / 5);	//스테이지 넘버 텍스트
	}
	else if (round >= 10)
	{
		number->Render(hdc, WIN_SIZE_X / 2 + 50, WIN_SIZE_Y / 5, round / 10, round / 50);
		number->Render(hdc, WIN_SIZE_X / 2 + 62, WIN_SIZE_Y / 5, (round % 10) % 5, (round % 10) / 5);

	}

	if (player)
		player->Render(hdc, WIN_SIZE_X / 5 + 15, WIN_SIZE_Y / 4);			//플레이어 텍스트

	if (playerScore)
	{
		playerScore->Render(hdc, WIN_SIZE_X / 4+30, WIN_SIZE_Y / 4 + 15, 0, 0);	//플레이어 누적 점수 1,10...10000자리
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

	for (int i = 0; i < 4; i++)		// 에너미 탱크 종류별로 랜더
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

	if (totalScore)
		totalScore->Render(hdc, WIN_SIZE_X/3 + 35, WIN_SIZE_Y *0.8 - 15 );	//토탈스코어 텍스트

	if (bTotalScore)													//토탈 누적 킬수
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

	// 점수 계산
	if (bSNE)		//노말
	{
		// 잡은 마리수 표기
		if (CNE < 10)	//1의자리
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, WIN_SIZE_Y / 3 + 20, CNE % 5, CNE / 5);
		}
		else //10의자리
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, WIN_SIZE_Y / 3 + 20, (CNE %10) % 5, (CNE%10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, WIN_SIZE_Y / 3 + 20, (CNE /10) % 5, (CNE/10) / 5);
		}

		//점수-> 순서대로 1, 10, 100 ,1000 자리
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

	if (bSSE)		//스피드
	{
		// 잡은 마리수 표기
		if (CSE < 10)	//1의자리
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) +45 , CSE % 5, CSE / 5);
		}
		else //10의자리
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) +45, (CSE % 10) % 5, (CSE % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, (WIN_SIZE_Y / 3 + 20) +45, (CSE / 10) % 5, (CSE / 10) / 5);
		}

		//점수 순서대로 1, 10, 100 ,1000 자리
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

	if (bSRE)		//래피드
	{
		// 잡은 마리수 표기
		if (CRE < 10)	//1의자리
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 90, CRE % 5, CRE / 5);
		}
		else //10의자리
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 90, (CRE % 10) % 5, (CRE % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, (WIN_SIZE_Y / 3 + 20) + 90, (CRE / 10) % 5, (CRE / 10) / 5);
		}

		//점수 순서대로 1, 10, 100 ,1000 자리
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

	if (bSBE)		//보스
	{
		// 잡은 마리수 표기
		if (CBE < 10)	//1의자리
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 135, CBE % 5, CBE / 5);
		}
		else //10의자리
		{
			number->Render(hdc, WIN_SIZE_X / 2 - 40, (WIN_SIZE_Y / 3 + 20) + 135, (CBE % 10) % 5, (CBE % 10) / 5);
			number->Render(hdc, WIN_SIZE_X / 2 - 50, (WIN_SIZE_Y / 3 + 20) + 135, (CBE / 10) % 5, (CBE / 10) / 5);
		}

		//점수 순서대로 1, 10, 100 ,1000 자리
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
	if (CNE < KNE)		//일반 탱크
	{
		if (elapsedcount >= elapsedTest)
		{
			CNE++;
			elapsedcount = 0;
		}
	}

	if (bSSE && CSE < KSE)	//스피드 탱크
	{
		if (elapsedcount >= elapsedTest)
		{
			CSE++;
			SSE += 2;
			elapsedcount = 0;
		}
	}

	if (bSRE && CRE < KRE)	//레피드 탱크
	{
		if (elapsedcount >= elapsedTest)
		{
			CRE++;
			elapsedcount = 0;
			SRE += 3;
		}
	}

	if (bSBE && CBE < KBE)	//보스 탱크
	{
		if (elapsedcount >= elapsedTest)
		{
			CBE++;
			elapsedcount = 0;
			SBE += 4;
		}
	}


	if (CNE == KNE && bSSE == false)	//노말 점수 끝
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bSSE = true;
			elapsedcount = 0;
		}
	}
	else if (CSE == KSE && bSRE == false)	//두번째 점수 계산끝
	{
		if (elapsedcount >= elapsedTest * 2)
		{
			bSRE = true;
			elapsedcount = 0;
		}
	}
	else if (CRE == KRE && bSBE == false)	//세번째 점수 계산 끝
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
