#include "Stage1Scene.h"
#include "ImageManager.h"
#include "Image.h"
#include "CommonFunction.h"

#include "Tank.h"
#include "TankFactorial.h"

HRESULT Stage1Scene::Init()
{
    SetWindowSize(20, 20, WIN_SIZE_X, WIN_SIZE_Y);
    sampleImage = ImageManager::GetSingleton()->AddImage("Image/Tile3.bmp",
        128, 16, 8, 1, true, RGB(255, 0, 255));
    if (sampleImage == nullptr)
    {
        cout << "Image/Tile1.bmp 로드 실패!!" << endl;
        return E_FAIL;
    }

    remainMonster = 18;
    remainSpawnMonster = 18;

    spawnMonsterImage = ImageManager::GetSingleton()->AddImage("Image/Icon/Icon_Enemy.bmp",
        16, 16, 1, 1, true, RGB(255, 0, 255));
    lifeImage = ImageManager::GetSingleton()->AddImage("Image/Icon/player1Life.bmp",
        35, 40, 1, 1, true, RGB(255, 0, 255));
    stageImage = ImageManager::GetSingleton()->AddImage("Image/Icon/StageFlag.bmp",
        35, 40, 1, 1, true, RGB(255, 0, 255));
    stageLevel = ImageManager::GetSingleton()->AddImage("Image/Text/Number.bmp", 60 /*40*/, 28 /*14*/, 5, 2, true, RGB(255, 0, 255));

    Load(1);



    vecTankFactorial.resize(5);
    vecTankFactorial[0] = new PlayerTankFactorial;
    vecTankFactorial[1] = new NormalEnemyTankFactorial;
    vecTankFactorial[2] = new SpeedEnemyTankFactorial;
    vecTankFactorial[3] = new RapidEnemyTankFactorial;
    vecTankFactorial[4] = new DefensiveEnemyTankFactorial;

    tank = vecTankFactorial[0]->CreateTank();
    tank->Init(tileInfo);

    cout << tileInfo[0].frameX << endl;
    return S_OK;
}

void Stage1Scene::Update()
{

    tank->Update();
}

void Stage1Scene::Render(HDC hdc)
{
    // 메인 영역
    for (int i = 0; i < TILE_COUNT_Y; i++)
    {
        for (int j = 0; j < TILE_COUNT_X; j++)
        {
            sampleImage->Render(hdc,
                tileInfo[i * TILE_COUNT_X + j].rc.left + TILE_SIZE / 2 + WIN_SIZE_X/2 - 8 * TILE_COUNT_X - 16,
                tileInfo[i * TILE_COUNT_X + j].rc.top + TILE_SIZE / 2 + WIN_SIZE_Y/2 - 8 * TILE_COUNT_Y,
                tileInfo[i * TILE_COUNT_X + j].frameX,
                tileInfo[i * TILE_COUNT_X + j].frameY);
        }
    }

    for (int i = 0; i < remainSpawnMonster; i++)
    {
        if (i % 2 == 0)
            spawnMonsterImage->Render(hdc, 472, 35 + 16*(i / 2));
        else
            spawnMonsterImage->Render(hdc, 472+16, 35 + 16*(i / 2));
    }

    lifeImage->Render(hdc, 480, 260);
    stageImage->Render(hdc, 480, 370);
    stageLevel->Render(hdc, 490, 390, 1, 0);
    tank->Render(hdc);
}

void Stage1Scene::Release()
{
}

void Stage1Scene::Load(int index)
{
    string filePath = "Save/saveMapData" + to_string(index) + ".map";

    HANDLE hFile = CreateFile(filePath.c_str(),
        GENERIC_READ,                  //읽기, 쓰기 타입
        0, NULL,                        //공유, 보안 모드
        OPEN_EXISTING,                  //파일을 만들거나 읽을 때 옵션
        FILE_ATTRIBUTE_NORMAL,          //파일 속성(읽기 전용, 숨김 등등)
        NULL);                          //

    //읽기

    DWORD readByte;
    if (ReadFile(hFile, tileInfo, sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y, &readByte, NULL) == false)
    {
        MessageBox(g_hWnd, "맵 데이터 로드에 실패했습니다.", "에러", MB_OK);
    }

    CloseHandle(hFile);
}