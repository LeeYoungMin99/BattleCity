#pragma once
#include "Config.h"
#include "GameEntity.h"

#define elapsedTest 5		//���� �ݿ� �ӵ� (������ ����)

class Image;
class ScoreScene : public GameEntity
{
private:
	//Image* hiScoreText;	//���̽��ھ� ���ŵɽ� ���
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
	//������ ���
	int killNormalEnemy, killSpeedEnemy, killRapidEnemy, killBossEnemy;
	int scoreNormalEnemy, scoreSpeedEnemy, scoreRapidEnemy, scoreBossEnemy;
	bool bScoreNormalEnemy, bScoreSpeedEnemy, bScoreRapidEnemy, bScoreBossEnemy;

	//������ ���ϰ�
	int KNE, KSE, KRE, KBE;				//������ ���� ���ͼ�
	int CNE, CSE, CRE, CBE;				//���� ���͸� 1�� ī��Ʈ�ϱ����� ����
	int SNE, SSE, SRE, SBE;				//���ھ� ����
	int bSNE, bSSE, bSRE, bSBE;			//������� ��������� ���� ��Ÿ��
	int TK_1, TK_10;					//��Ż ų�� 1���ڸ�, 10���ڸ�
	int bTotalScore;					// ������� ������ ��Ż���� 
	int round;



public:
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
};

