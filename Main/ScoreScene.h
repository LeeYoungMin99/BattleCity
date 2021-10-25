#pragma once
#include "Config.h"
#include "GameEntity.h"

#define elapsedTest 20		//���� �ݿ� �ӵ� (������ ����)

class Image;
class ScoreScene : public GameEntity
{
private:
	//Image* hiScoreText;	//���̽��ھ� ���ŵɽ� ���
	Image* noneHiScore = nullptr;
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
	int hightScore = 0;
	int player1Score = 0;

	int killNormalTank = 0, killSpeedTank = 0, killRapidTank = 0, killDefensiveTank = 0;				//������ ���� ��ũ��
	int normalTankCount = 0, speedTankCount = 0, rapidTankCount = 0, defensiveTankCount = 0;				//���� ��ũ�� 1�� ī��Ʈ�ϱ����� ����
	int normalTankScore = 0, speedTankScore = 0, rapidTankScore = 0, defensiveTankScore = 0;				//���ھ� ����

	bool bSNE = false, bSSE = false, bSRE = false, bSBE = false;			//������� ��������� ���� ��Ÿ��

	int TK = 0;								//��Ż ų��
	int bTotalScore = 0;					// ������� ������ ��Ż���� 
	int round = 0;							// ���� ����
	bool gameOver = 0;						// ���ӿ��� �˸�



public:
	virtual ~ScoreScene() = default;

	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	void ScoreCalculate();
};

