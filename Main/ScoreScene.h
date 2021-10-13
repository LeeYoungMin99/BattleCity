#pragma once
#include "Config.h"
#include "GameEntity.h"

#define elapsedTest 20

class Image;
class ScoreScene : public GameEntity
{
private:
	//Image* hiScoreText;	//하이스코어 갱신될시 사용
	Image* hiScore;
	Image* totalScore;
	Image* playerScore;
	Image* stage;
	Image* stageNum;
	Image* enemyTank;
	Image* arrow;
	Image* pts;
	Image* backGround;

	int elapsedcount;
	//변수명 
	int killNormalEnemy, killSpeedEnemy, killRapidEnemy, killBossEnemy;
	int scoreNormalEnemy, scoreSpeedEnemy, scoreRapidEnemy, scoreBossEnemy;
	bool bScoreNormalEnemy, bScoreSpeedEnemy, bScoreRapidEnemy, bScoreBossEnemy;

	//구현용 편하게
	int KNE, KSE, KRE, KBE;
	int SNE, SNE_1, SNE_10, SSE, SSE_1, SSE_10, SRE, SRE_1, SRE_10, SBE, SBE_1, SBE_10;
	int bSNE, bSSE, bSRE, bSBE;


	int score;

public:
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
};

