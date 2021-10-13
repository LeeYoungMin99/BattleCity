#pragma once
#include "Config.h"
#include "Singleton.h"

class GameManager : public Singleton<GameManager>
{
public:
	int hightScore;	 // 최고 점수
	int player1Score;


public:
	void Init();
};

