#pragma once
#include "Config.h"
#include "Singleton.h"


class GameManager : public Singleton<GameManager>
{
public:
	int hightScore;	 // �ְ� ����
	int player1Score;


public:
	void Init();
	void ScoreSave();
	void ScoreLoad();

	inline void SetScore(int player1Score) { this->player1Score = player1Score; }
	inline int GetScore() { return this->player1Score; }
	inline int GetHightScore() { return this->hightScore; }
};

