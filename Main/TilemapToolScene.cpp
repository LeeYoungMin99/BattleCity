#include "TilemapToolScene.h"
#include "Image.h"
#include "CommonFunction.h"

HRESULT TilemapToolScene::Init()
{
    SetWindowSize(20, 20, TILEMAPTOOL_SIZE_X, TILEMAPTOOL_SIZE_Y);

    sampleImage = ImageManager::GetSingleton()->AddImage("Image/Tile3.bmp",
        128, 32, 8, 2, true, RGB(255, 0, 255));

    if (sampleImage == nullptr)
    {
        cout << "Image/Tile3.bmp 로드 실패!!" << endl; 
        return E_FAIL;
    }

    // 왼쪽 상단 메인 영역 초기화
    for (int i = 0; i < TILE_COUNT_Y; i++)    // y축
    {
        for (int j = 0; j < TILE_COUNT_X; j++)    // x축
        {
            SetRect(&(tileInfo[i * TILE_COUNT_X + j].rc),
                j * TILE_SIZE , i * TILE_SIZE,
                j * TILE_SIZE + TILE_SIZE , i * TILE_SIZE + TILE_SIZE);

            tileInfo[i * TILE_COUNT_X + j].frameX = 7;
            tileInfo[i * TILE_COUNT_X + j].frameY = 0;

            tileInfo[i].collider.left = 0;
            tileInfo[i].collider.top = 0;
            tileInfo[i].collider.right = 0;
            tileInfo[i].collider.bottom = 0;

            tileInfo[i].bodyCollider.left = 0;
            tileInfo[i].bodyCollider.top = 0;
            tileInfo[i].bodyCollider.right = 0;
            tileInfo[i].bodyCollider.bottom = 0;

            tileInfo[i * TILE_COUNT_X + j].tileType = TileType::Ground;
            tileInfo[i * TILE_COUNT_X + j].leftHit = 0;
            tileInfo[i * TILE_COUNT_X + j].rightHit = 0;
            tileInfo[i * TILE_COUNT_X + j].bottomHit = 0;
            tileInfo[i * TILE_COUNT_X + j].topHit = 0;
        }
    }

    // 오른쪽 상단 샘플 영역 초기화
    for (int i = 0; i < SAMPLE_TILE_COUNT_Y; i++)    // y축
    {
        for (int j = 0; j < SAMPLE_TILE_COUNT_X; j++)    // x축
        {
            SetRect(&(sampleTileInfo[i * SAMPLE_TILE_COUNT_X + j].rc),
                TILEMAPTOOL_SIZE_X - sampleImage->GetWidth() + j * TILE_SIZE,
                i * TILE_SIZE,
                TILEMAPTOOL_SIZE_X - sampleImage->GetWidth() + j * TILE_SIZE + TILE_SIZE,
                i * TILE_SIZE + TILE_SIZE);


            sampleTileInfo[i * SAMPLE_TILE_COUNT_X + j].frameX = j;
            sampleTileInfo[i * SAMPLE_TILE_COUNT_X + j].frameY = i;
        }
    }

    bShowNoneWalkable = false;
    bShowBodyCollider = false;

    // 선택된 타일
    selectedSampleTile.frameX = 0;
    selectedSampleTile.frameY = 0;

    return S_OK;
}

void TilemapToolScene::Update()
{
    // 샘플영역에서 샘플을 선택
    RECT sampleArea;
    sampleArea.left = TILEMAPTOOL_SIZE_X - sampleImage->GetWidth();
    sampleArea.right = TILEMAPTOOL_SIZE_X;
    sampleArea.top = 0;
    sampleArea.bottom = sampleImage->GetHeight();


    if (PtInRect(&(sampleArea), g_ptMouse))
    {
        if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
        {
            int posX = g_ptMouse.x - sampleArea.left;
            selectedIdX = posX / TILE_SIZE;

            int posY = g_ptMouse.y - sampleArea.top;
            selectedIdY = posY / TILE_SIZE;

            selectedSampleTile.frameX =
                sampleTileInfo[selectedIdY * SAMPLE_TILE_COUNT_X + selectedIdX].frameX;
            selectedSampleTile.frameY =
                sampleTileInfo[selectedIdY * SAMPLE_TILE_COUNT_X + selectedIdX].frameY;

            cout << selectedIdX << "  " << selectedIdY << endl;
        
            switch(selectedIdX)
            {
                case 0:
                    state = State::NoneWalkable;
                    tileType = TileType::Brick;
                    cout << "현재 타입 Brick" << endl;
                    break;
                case 1:
                    state = State::NoneWalkable;
                    tileType = TileType::Wall;
                    cout << "현재 타입 Wall" << endl;
                    break;
                case 2:
                    state = State::Walkable;
                    tileType = TileType::Grass;
                    cout << "현재 타입 Grass" << endl;
                    break;
                case 3:
                    state = State::Walkable;
                    tileType = TileType::Iced;
                    cout << "현재 타입 Iced" << endl;
                    break;
                case 4:
                    state = State::Walkable;
                    tileType = TileType::Water;
                    cout << "현재 타입 Water" << endl;
                    break;
                case 5:
                    state = State::NoneWalkable;
                    tileType = TileType::Water;
                    cout << "현재 타입 Water" << endl;
                    break;
                case 6:
                    state = State::NoneWalkable;
                    tileType = TileType::Water;
                    cout << "현재 타입 Water" << endl;
                    break;
                case 7:
                    state = State::Walkable;
                    tileType = TileType::Ground;
                    cout << "현재 타입 Ground" << endl;
                    break;
                default:
                    break;
            }
            if (selectedIdY == 1)
            {
                state = State::NoneWalkable;
                tileType = TileType::Nexus;
                cout << "현재 타입 Nexus" << endl;
            }
        }
    }

    // 메인영역에서 선택된 샘플 정보로 수정
    for (int i = 0; i < TILE_COUNT_X * TILE_COUNT_Y; i++)
    {
        if (PtInRect(&(tileInfo[i].rc), g_ptMouse))
        {
            if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
            {
                tileInfo[i].frameX = selectedSampleTile.frameX;
                tileInfo[i].frameY = selectedSampleTile.frameY;
                if (state == State::NoneWalkable)
                {
                    tileInfo[i].collider = tileInfo[i].rc;
                    tileInfo[i].tileType = tileType;

                    tileInfo[i].bodyCollider = tileInfo[i].rc;
                    if (tileType == TileType::Water)
                    {
                        tileInfo[i].collider.left = 0;
                        tileInfo[i].collider.top = 0;
                        tileInfo[i].collider.right = 0;
                        tileInfo[i].collider.bottom = 0;
                    }
                }
                else if (state == State::Walkable)
                {
                    tileInfo[i].collider.left = 0;
                    tileInfo[i].collider.top = 0;
                    tileInfo[i].collider.right = 0;
                    tileInfo[i].collider.bottom= 0;

                    tileInfo[i].bodyCollider.left = 0;
                    tileInfo[i].bodyCollider.top = 0;
                    tileInfo[i].bodyCollider.right = 0;
                    tileInfo[i].bodyCollider.bottom = 0;
                    tileInfo[i].tileType = tileType;
                }
                break;
            }
        }
    }

    if (KeyManager::GetSingleton()->IsOnceKeyUp('S'))
    {
        Save();
    }
    if (KeyManager::GetSingleton()->IsOnceKeyUp('L'))
    {
        Load();
    }
    if (KeyManager::GetSingleton()->IsOnceKeyUp('W'))
    {
        if (bShowNoneWalkable)
        {
            bShowNoneWalkable = false;
        }
        else
        {
            bShowNoneWalkable = true;
        }
    }
    if (KeyManager::GetSingleton()->IsOnceKeyUp('E'))
    {
        if (bShowBodyCollider)
        {
            bShowBodyCollider = false;
        }
        else
        {
            bShowBodyCollider = true;
        }
    }
}

void TilemapToolScene::Render(HDC hdc)
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


            if(bShowNoneWalkable)
                Rectangle(hdc, tileInfo[i * TILE_COUNT_X + j].collider.left, tileInfo[i * TILE_COUNT_X + j].collider.top, tileInfo[i * TILE_COUNT_X + j].collider.right, tileInfo[i * TILE_COUNT_X + j].collider.bottom);
            if (bShowBodyCollider)
                Rectangle(hdc, tileInfo[i * TILE_COUNT_X + j].bodyCollider.left, tileInfo[i * TILE_COUNT_X + j].bodyCollider.top, tileInfo[i * TILE_COUNT_X + j].bodyCollider.right, tileInfo[i * TILE_COUNT_X + j].bodyCollider.bottom);


            //Rectangle(hdc, tileInfo[i * TILE_COUNT_X + j].rc.left,
            //    tileInfo[i * TILE_COUNT_X + j].rc.top,
            //    tileInfo[i * TILE_COUNT_X + j].rc.right,
            //    tileInfo[i * TILE_COUNT_X + j].rc.bottom);
        }
    }


    //for (int i = 0; i < SAMPLE_TILE_COUNT_X * SAMPLE_TILE_COUNT_Y; i++)
    //{
    //    Rectangle(hdc, sampleTileInfo[i].rc.left,
    //    sampleTileInfo[i].rc.top,
    //    sampleTileInfo[i].rc.right,
    //    sampleTileInfo[i].rc.bottom);
    //}

    // 샘플 영역
    sampleImage->Render(hdc, 
        TILEMAPTOOL_SIZE_X - sampleImage->GetWidth() + sampleImage->GetWidth() / 2,
        sampleImage->GetHeight() / 2);



    PatBlt(hdc, TILEMAPTOOL_SIZE_X - sampleImage->GetWidth() + sampleImage->GetFrameWidth() / 2 - 32, sampleImage->GetHeight() + sampleImage->GetFrameHeight() / 2 + 148, 112,112, WHITENESS);
    // 선택된 타일
    sampleImage->Render(hdc, TILEMAPTOOL_SIZE_X - sampleImage->GetWidth() + sampleImage->GetFrameWidth() / 2,
        sampleImage->GetHeight() + sampleImage->GetFrameHeight() / 2 + 180,
        selectedSampleTile.frameX, selectedSampleTile.frameY, 2.5f);
    
    

}

void TilemapToolScene::Release()
{

}

void TilemapToolScene::Save(int saveIndex)
{
    string filePath = "Save/saveMapData" + to_string(saveIndex) + ".map";


    HANDLE hFile = CreateFile(filePath.c_str(),
        GENERIC_WRITE,                  //읽기, 쓰기 타입
        0, NULL,                        //공유, 보안 모드
        CREATE_ALWAYS,                  //파일을 만들거나 읽을 때 옵션
        FILE_ATTRIBUTE_NORMAL,          //파일 속성(읽기 전용, 숨김 등등)
        NULL);                          //

    //쓰기
    DWORD writtenByte;
    if (WriteFile(hFile,                                    //파일 핸들
        tileInfo,                                       // 메모리 시작 주소
        sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y,  // 메모리 크기
        &writtenByte,                                   // 실제 쓰여진 파일 용량
        NULL) == false)
    {
        MessageBox(g_hWnd, "맵 데이터 저장에 실패했습니다.", "에러", MB_OK);
    }

    CloseHandle(hFile);
}

void TilemapToolScene::Load(int loadIndex)
{
    string filePath = "Save/saveMapData" + to_string(loadIndex) + ".map";

    HANDLE hFile = CreateFile(filePath.c_str(),
        GENERIC_READ,                  //읽기, 쓰기 타입
        0, NULL,                        //공유, 보안 모드
        OPEN_EXISTING,                  //파일을 만들거나 읽을 때 옵션
        FILE_ATTRIBUTE_NORMAL,          //파일 속성(읽기 전용, 숨김 등등)
        NULL);                          //

    //읽기

    DWORD readByte;
    if ( ReadFile(hFile, tileInfo, sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y, &readByte, NULL) == false )
    {
        MessageBox(g_hWnd, "맵 데이터 로드에 실패했습니다.", "에러", MB_OK);
    }

    CloseHandle(hFile);
}
