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
};

