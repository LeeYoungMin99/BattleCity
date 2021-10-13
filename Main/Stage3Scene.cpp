#include "Stage3Scene.h"
#include "Stage3Scene.h"
#include "ImageManager.h"
#include "Image.h"
#include "CommonFunction.h"


HRESULT Stage3Scene::Init()
{
    SetWindowSize(20, 20, WIN_SIZE_X, WIN_SIZE_Y);
    sampleImage = ImageManager::GetSingleton()->AddImage("Image/Tile3.bmp",
        128, 16, 8, 1, true, RGB(255, 0, 255));
    if (sampleImage == nullptr)
    {
        cout << "Image/Tile1.bmp �ε� ����!!" << endl;
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

    Load(3);

    return S_OK;
}

void Stage3Scene::Update()
{

}

void Stage3Scene::Render(HDC hdc)
{
    // ���� ����
    for (int i = 0; i < TILE_COUNT_Y; i++)
    {
        for (int j = 0; j < TILE_COUNT_X; j++)
        {
            sampleImage->Render(hdc,
                tileInfo[i * TILE_COUNT_X + j].rc.left + TILE_SIZE / 2 + WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16,
                tileInfo[i * TILE_COUNT_X + j].rc.top + TILE_SIZE / 2 + WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y,
                tileInfo[i * TILE_COUNT_X + j].frameX,
                tileInfo[i * TILE_COUNT_X + j].frameY);
        }
    }

    for (int i = 0; i < remainSpawnMonster; i++)
    {
        if (i % 2 == 0)
            spawnMonsterImage->Render(hdc, 472, 35 + 16 * (i / 2));
        else
            spawnMonsterImage->Render(hdc, 472 + 16, 35 + 16 * (i / 2));
    }

    lifeImage->Render(hdc, 480, 260);
    stageImage->Render(hdc, 480, 370);
    stageLevel->Render(hdc, 490, 390, 3, 0);
}

void Stage3Scene::Release()
{
}

void Stage3Scene::Load(int index)
{

    string filePath = "Save/saveMapData" + to_string(index) + ".map";

    HANDLE hFile = CreateFile(filePath.c_str(),
        GENERIC_READ,                  //�б�, ���� Ÿ��
        0, NULL,                        //����, ���� ���
        OPEN_EXISTING,                  //������ ����ų� ���� �� �ɼ�
        FILE_ATTRIBUTE_NORMAL,          //���� �Ӽ�(�б� ����, ���� ���)
        NULL);                          //

    //�б�

    DWORD readByte;
    if (ReadFile(hFile, tileInfo, sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y, &readByte, NULL) == false)
    {
        MessageBox(g_hWnd, "�� ������ �ε忡 �����߽��ϴ�.", "����", MB_OK);
    }

    CloseHandle(hFile);


}