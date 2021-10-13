#include "Stage1Scene.h"
#include "ImageManager.h"
#include "Image.h"
#include "CommonFunction.h"


HRESULT Stage1Scene::Init()
{
    SetWindowSize(20, 20, TILEMAPTOOL_SIZE_X, TILEMAPTOOL_SIZE_Y);
    sampleImage = ImageManager::GetSingleton()->AddImage("Image/Tile3.bmp",
        128, 16, 8, 1, true, RGB(255, 0, 255));

    if (sampleImage == nullptr)
    {
        cout << "Image/Tile1.bmp 로드 실패!!" << endl;
        return E_FAIL;
    }


    Load(0);

    cout << tileInfo[0].frameX << endl;
    return S_OK;
}

void Stage1Scene::Update()
{
}

void Stage1Scene::Render(HDC hdc)
{
    // 메인 영역
    for (int i = 0; i < TILE_COUNT_Y; i++)
    {
        for (int j = 0; j < TILE_COUNT_X; j++)
        {
            sampleImage->Render(hdc,
                tileInfo[i * TILE_COUNT_X + j].rc.left + TILE_SIZE / 2,
                tileInfo[i * TILE_COUNT_X + j].rc.top + TILE_SIZE / 2,
                tileInfo[i * TILE_COUNT_X + j].frameX,
                tileInfo[i * TILE_COUNT_X + j].frameY);
        }
    }

}

void Stage1Scene::Release()
{
}

void Stage1Scene::Load(int index)
{

    string filePath = "Save/saveMapData" + to_string(0) + ".map";

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