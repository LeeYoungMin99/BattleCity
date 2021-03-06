#pragma once
#include "Config.h"
#include "GameEntity.h"

#define elapsedTest 20		//점수 반영 속도 (적으면 빠름)

class Image;
class ScoreScene : public GameEntity
{
private:
	//Image* hiScoreText;	//하이스코어 갱신될시 사용
	Image* hiScore = nullptr;
	Image* totalScore = nullptr;
	Image* player = nullptr;
	Image* playerScore = nullptr;
	Image* stage = nullptr;
	Image* number = nullptr;
	Image* enemyTank = nullptr;
	Image* arrow = nullptr;
	Image* pts = nullptr;
	Image* backGround = nullptr;

	int elapsedcount = 0;
	int highScore = 0;
	int player1Score = 0;

	// 변수는 한 줄에 하나
	int killNormalTank = 0, killSpeedTank = 0, killRapidTank = 0, killDefensiveTank = 0;		//종류별 죽인 탱크수
	int normalTankCount = 0, speedTankCount = 0, rapidTankCount = 0, defensiveTankCount = 0;	//죽인 탱크를 1씩 카운트하기위한 변수
	int normalTankScore = 0, speedTankScore = 0, rapidTankScore = 0, defensiveTankScore = 0;	//스코어 점수
	int totalKill = 0;						//토탈 킬수
	int round = 0;							// 현재 라운드
	bool bNormalTankScoreFinish = false, bSpeedTankScoreFinish = false,
	bRapidTankScoreFinish = false, bDefensiveTankScoreFinish = false;			//순서대로 점수계산을 위한 불타입
	bool bTotalScore = 0;					// 점수계산 끝나고 토탈점수 
	bool bGameOver = 0;						// 게임오버 알림



public:
	virtual ~ScoreScene() = default;

	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	void ScoreCalculate();
};

