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
        GENERIC_WRITE,                  //읽기, 쓰기 타입
        0, NULL,                        //공유, 보안 모드
        CREATE_ALWAYS,                  //파일을 만들거나 읽을 때 옵션
        FILE_ATTRIBUTE_NORMAL,          //파일 속성(읽기 전용, 숨김 등등)
        NULL);                          //

    //쓰기
    DWORD writtenByte;
    if (WriteFile(hFile,                                    //파일 핸들
        (LPCVOID)&hightScore,                                       // 메모리 시작 주소
        sizeof(int)/* * TILE_COUNT_X * TILE_COUNT_Y*/,  // 메모리 크기
        &writtenByte,                                   // 실제 쓰여진 파일 용량
        NULL) == false)
    {
        MessageBox(g_hWnd, "SAVE_FAIL"/*맵 데이터 저장에 실패했습니다.*/, "ERROR"/*에러*/, MB_OK);
    }

    CloseHandle(hFile);
}

void GameManager::ScoreLoad()
{
    string filePath = "Save/ScoreData.txt"/* + to_string(loadIndex) + ".map"*/;

    HANDLE hFile = CreateFile(filePath.c_str(),
        GENERIC_READ,                  //읽기, 쓰기 타입
        0, NULL,                        //공유, 보안 모드
        OPEN_EXISTING,                  //파일을 만들거나 읽을 때 옵션
        FILE_ATTRIBUTE_NORMAL,          //파일 속성(읽기 전용, 숨김 등등)
        NULL);                          //

    //읽기

    DWORD readByte;
    if (ReadFile(hFile, (LPVOID)&hightScore, sizeof(int)/* * TILE_COUNT_X * TILE_COUNT_Y*/, &readByte, NULL) == false)
    {
        MessageBox(g_hWnd, "LOAD_FAIL"/*맵 데이터 로드에 실패했습니다.*/, "ERROR"/*에러*/, MB_OK);
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
