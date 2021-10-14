#pragma once
#include "Config.h"
#include "GameEntity.h"

#define elapsedTest 5		//점수 반영 속도 (적으면 빠름)

class Image;
class ScoreScene : public GameEntity
{
private:
	//Image* hiScoreText;	//하이스코어 갱신될시 사용
	Image* noneHiScore;
	Image* hiScore;
	Image* totalScore;
	Image* player;
	Image* playerScore;
	Image* stage;
	Image* number;
	Image* enemyTank;
	Image* arrow;
	Image* pts;
	Image* backGround;

	int elapsedcount;
	int hightScore;
	int player1Score;
	//변수명 고민
	int killNormalEnemy, killSpeedEnemy, killRapidEnemy, killBossEnemy;
	int scoreNormalEnemy, scoreSpeedEnemy, scoreRapidEnemy, scoreBossEnemy;
	bool bScoreNormalEnemy, bScoreSpeedEnemy, bScoreRapidEnemy, bScoreBossEnemy;

	//디버깅용 편하게
	int KNE, KSE, KRE, KBE;				//종류별 죽인 탱크수
	int CNE, CSE, CRE, CBE;				//죽인 탱크를 1씩 카운트하기위한 변수
	int SNE, SSE, SRE, SBE;				//스코어 점수
	int bSNE, bSSE, bSRE, bSBE;			//순서대로 점수계산을 위한 불타입

	int TK;								//토탈 킬수
	int bTotalScore;					// 점수계산 끝나고 토탈점수 
	int round;							// 현재 라운드
	bool gameOver;						// 게임오버 알림



public:
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	void ScoreCalculate();
};

