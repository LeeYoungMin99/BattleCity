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

	int KNE = 0, KSE = 0, KRE = 0, KBE = 0;				//������ ���� ��ũ��
	int CNE = 0, CSE = 0, CRE = 0, CBE = 0;				//���� ��ũ�� 1�� ī��Ʈ�ϱ����� ����
	int SNE = 0, SSE = 0, SRE = 0, SBE = 0;				//���ھ� ����
	int bSNE = 0, bSSE = 0, bSRE = 0, bSBE = 0;			//������� ��������� ���� ��Ÿ��

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

