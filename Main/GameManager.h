#pragma once
#include "Config.h"
#include "Singleton.h"

enum class GameState{Playing, Done, DestoryNexus, GameOver};

class GameManager : public Singleton<GameManager>
{
public:
	int defeatNormalTank = 0;
	int defeatSpeedTank = 0;
	int defeatRapidTank = 0;
	int defeatDefensiveTank = 0;

	int hightScore = 0;	 // 최고 점수
	int player1Score = 0;
	int player1Life = 0;
	int playerEnforceCount = 0;


	int remainSpawnMonster = 0;
	int remainMonster = 0;
	int spawnCount = 0;

	int stageLevel = 0;

	GameState state = {};

public:
	virtual ~GameManager() = default;

	void Init();
	void ScoreSave();
	void ScoreLoad();

	inline void SetScore(int player1Score) { this->player1Score = player1Score; }
	inline int GetScore() { return this->player1Score; }
	inline int GetHightScore() { return this->hightScore; }
	void GameInit();
};

