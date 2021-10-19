#include "GameManager.h"

void GameManager::Init()
{
    player1Score = 0;
    hightScore = 0;

    remainSpawnMonster=0;
    remainMonster=0;
    spawnCount = 0;
    defeatNormalTank = 0;
    defeatSpeedTank = 0;
    defeatRapidTank = 0;
    defeatDefensiveTank = 0;
    player1Life = 2;

    state = GameState::Playing;

    stageLevel = 1;
}

void GameManager::ScoreSave()
{
    string filePath = "Save/ScoreData.txt"/* + to_string(saveIndex) + ".map"*/;

    hightScore = player1Score;

    HANDLE hFile = CreateFile(filePath.c_str(),
        GENERIC_WRITE,                  //�б�, ���� Ÿ��
        0, NULL,                        //����, ���� ���
        CREATE_ALWAYS,                  //������ ����ų� ���� �� �ɼ�
        FILE_ATTRIBUTE_NORMAL,          //���� �Ӽ�(�б� ����, ���� ���)
        NULL);                          //

    //����
    DWORD writtenByte;
    if (WriteFile(hFile,                                    //���� �ڵ�
        (LPCVOID)&hightScore,                                       // �޸� ���� �ּ�
        sizeof(int)/* * TILE_COUNT_X * TILE_COUNT_Y*/,  // �޸� ũ��
        &writtenByte,                                   // ���� ������ ���� �뷮
        NULL) == false)
    {
        MessageBox(g_hWnd, "SAVE_FAIL"/*�� ������ ���忡 �����߽��ϴ�.*/, "ERROR"/*����*/, MB_OK);
    }

    CloseHandle(hFile);
}

void GameManager::ScoreLoad()
{
    string filePath = "Save/ScoreData.txt"/* + to_string(loadIndex) + ".map"*/;

    HANDLE hFile = CreateFile(filePath.c_str(),
        GENERIC_READ,                  //�б�, ���� Ÿ��
        0, NULL,                        //����, ���� ���
        OPEN_EXISTING,                  //������ ����ų� ���� �� �ɼ�
        FILE_ATTRIBUTE_NORMAL,          //���� �Ӽ�(�б� ����, ���� ���)
        NULL);                          //

    //�б�

    DWORD readByte;
    if (ReadFile(hFile, (LPVOID)&hightScore, sizeof(int)/* * TILE_COUNT_X * TILE_COUNT_Y*/, &readByte, NULL) == false)
    {
        MessageBox(g_hWnd, "LOAD_FAIL"/*�� ������ �ε忡 �����߽��ϴ�.*/, "ERROR"/*����*/, MB_OK);
    }

    //return hightScore;
    CloseHandle(hFile);
}

void GameManager::GameInit()
{
    player1Score = 0;
    hightScore = 0;

    remainSpawnMonster = 0;
    remainMonster = 0;
    spawnCount = 0;
    defeatNormalTank = 0;
    defeatSpeedTank = 0;
    defeatRapidTank = 0;
    defeatDefensiveTank = 0;
    player1Life = 2;

    state = GameState::Playing;

    stageLevel = 1;
}
