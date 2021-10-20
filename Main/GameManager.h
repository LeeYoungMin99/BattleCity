#pragma once
#include "Config.h"
#include "Singleton.h"

static enum class GameState{Playing, Done, DestoryNexus, GameOver};

class GameManager : public Singleton<GameManager>
{
public:
	int defeatNormalTank;
	int defeatSpeedTank;
	int defeatRapidTank;
	int defeatDefensiveTank;

	int hightScore;	 // 최고 점수
	int player1Score;
	int player1Life;
	int playerEnforceCount = 0;


	int remainSpawnMonster;
	int remainMonster;
	int spawnCount;

	int stageLevel;

	GameState state;

public:
	void Init();
	void ScoreSave();
	void ScoreLoad();

	inline void SetScore(int player1Score) { this->player1Score = player1Score; }
	inline int GetScore() { return this->player1Score; }
	inline int GetHightScore() { return this->hightScore; }
	void GameInit();
};

