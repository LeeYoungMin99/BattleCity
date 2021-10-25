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
        cout << "Image/Tile3.bmp �ε� ����!!" << endl; 
        return E_FAIL;
    }

    // ���� ��� ���� ���� �ʱ�ȭ
    for (int i = 0; i < TILE_COUNT_Y; i++)    // y��
    {
        for (int j = 0; j < TILE_COUNT_X; j++)    // x��
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

    // ������ ��� ���� ���� �ʱ�ȭ
    for (int i = 0; i < SAMPLE_TILE_COUNT_Y; i++)    // y��
    {
        for (int j = 0; j < SAMPLE_TILE_COUNT_X; j++)    // x��
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

    // ���õ� Ÿ��
    selectedSampleTile.frameX = 0;
    selectedSampleTile.frameY = 0;

    return S_OK;
}

void TilemapToolScene::Update()
{
    // ���ÿ������� ������ ����
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
                    cout << "���� Ÿ�� Brick" << endl;
                    break;
                case 1:
                    state = State::NoneWalkable;
                    tileType = TileType::Wall;
                    cout << "���� Ÿ�� Wall" << endl;
                    break;
                case 2:
                    state = State::Walkable;
                    tileType = TileType::Grass;
                    cout << "���� Ÿ�� Grass" << endl;
                    break;
                case 3:
                    state = State::Walkable;
                    tileType = TileType::Iced;
                    cout << "���� Ÿ�� Iced" << endl;
                    break;
                case 4:
                    state = State::Walkable;
                    tileType = TileType::Water;
                    cout << "���� Ÿ�� Water" << endl;
                    break;
                case 5:
                    state = State::NoneWalkable;
                    tileType = TileType::Water;
                    cout << "���� Ÿ�� Water" << endl;
                    break;
                case 6:
                    state = State::NoneWalkable;
                    tileType = TileType::Water;
                    cout << "���� Ÿ�� Water" << endl;
                    break;
                case 7:
                    state = State::Walkable;
                    tileType = TileType::Ground;
                    cout << "���� Ÿ�� Ground" << endl;
                    break;
                default:
                    break;
            }
            if (selectedIdY == 1)
            {
                state = State::NoneWalkable;
                tileType = TileType::Nexus;
                cout << "���� Ÿ�� Nexus" << endl;
            }
        }
    }

    // ���ο������� ���õ� ���� ������ ����
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


    // ���� ����
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

    // ���� ����
    sampleImage->Render(hdc, 
        TILEMAPTOOL_SIZE_X - sampleImage->GetWidth() + sampleImage->GetWidth() / 2,
        sampleImage->GetHeight() / 2);



    PatBlt(hdc, TILEMAPTOOL_SIZE_X - sampleImage->GetWidth() + sampleImage->GetFrameWidth() / 2 - 32, sampleImage->GetHeight() + sampleImage->GetFrameHeight() / 2 + 148, 112,112, WHITENESS);
    // ���õ� Ÿ��
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
        GENERIC_WRITE,                  //�б�, ���� Ÿ��
        0, NULL,                        //����, ���� ���
        CREATE_ALWAYS,                  //������ ����ų� ���� �� �ɼ�
        FILE_ATTRIBUTE_NORMAL,          //���� �Ӽ�(�б� ����, ���� ���)
        NULL);                          //

    //����
    DWORD writtenByte;
    if (WriteFile(hFile,                                    //���� �ڵ�
        tileInfo,                                       // �޸� ���� �ּ�
        sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y,  // �޸� ũ��
        &writtenByte,                                   // ���� ������ ���� �뷮
        NULL) == false)
    {
        MessageBox(g_hWnd, "�� ������ ���忡 �����߽��ϴ�.", "����", MB_OK);
    }

    CloseHandle(hFile);
}

void TilemapToolScene::Load(int loadIndex)
{
    string filePath = "Save/saveMapData" + to_string(loadIndex) + ".map";

    HANDLE hFile = CreateFile(filePath.c_str(),
        GENERIC_READ,                  //�б�, ���� Ÿ��
        0, NULL,                        //����, ���� ���
        OPEN_EXISTING,                  //������ ����ų� ���� �� �ɼ�
        FILE_ATTRIBUTE_NORMAL,          //���� �Ӽ�(�б� ����, ���� ���)
        NULL);                          //

    //�б�

    DWORD readByte;
    if ( ReadFile(hFile, tileInfo, sizeof(tagTile) * TILE_COUNT_X * TILE_COUNT_Y, &readByte, NULL) == false )
    {
        MessageBox(g_hWnd, "�� ������ �ε忡 �����߽��ϴ�.", "����", MB_OK);
    }

    CloseHandle(hFile);
}
