#include "AStarScene.h"
#include "CommonFunction.h"

HRESULT AstarTile::Init()
{
    return E_NOTIMPL;
}

HRESULT AstarTile::Init(int idX, int idY)
{
    this->idX = idX;
    this->idY = idY;

    center.x = idX;// *ASTAR_TILE_SIZE + ASTAR_TILE_SIZE / 2;
    center.y = idY;// *ASTAR_TILE_SIZE + ASTAR_TILE_SIZE / 2;

    rc.left = idX * ASTAR_TILE_SIZE;
    rc.top = idY * ASTAR_TILE_SIZE;
    rc.right = rc.left + ASTAR_TILE_SIZE;
    rc.bottom = rc.top + ASTAR_TILE_SIZE;

    F = 0.0f; G = 0.0f; H = 0.0f;

    type = AstarTileType::None;

    color = RGB(205,255,205);
    hBrush = CreateSolidBrush(color);
    
    hTypeBrush[AstarTileType::Start] = CreateSolidBrush(RGB(206, 9, 53));
    hTypeBrush[AstarTileType::Dest] = CreateSolidBrush(RGB(61, 209, 94));
    hTypeBrush[AstarTileType::Wall] = CreateSolidBrush(RGB(94, 94, 94));
    hTypeBrush[AstarTileType::Walkable] = CreateSolidBrush(RGB(194, 194, 194));
    hTypeBrush[AstarTileType::Curr] = CreateSolidBrush(RGB(255, 0, 255));
    hTypeBrush[AstarTileType::Closed] = CreateSolidBrush(RGB(0, 0, 0));
    hTypeBrush[AstarTileType::Path] = CreateSolidBrush(RGB(0, 255, 0));
    hTypeBrush[AstarTileType::None] = CreateSolidBrush(RGB(205, 255, 205));

    return S_OK;
}

void AstarTile::Update()
{
}

void AstarTile::Render(HDC hdc)
{
    hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    FillRect(hdc, &rc, hBrush);
    Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);


    SelectObject(hdc, hOldBrush);
}

void AstarTile::Release()
{
    for (int i = 0; i < AstarTileType::End; i++)
    {
        DeleteObject(hTypeBrush[i]);
    }

    DeleteObject(hBrush);
}

void AstarTile::SetType(AstarTileType type)
{
    this->type = type; 
   
    hBrush = hTypeBrush[type];
}

HRESULT AStarScene::Init()
{
    SetWindowSize(30, 30, TILEMAPTOOL_SIZE_X, TILEMAPTOOL_SIZE_Y);

    openList.clear();

    for (int i = 0; i < ASTAR_TILE_COUNT; i++)
    {
        for (int j = 0; j < ASTAR_TILE_COUNT; j++)
        {
            map[i][j].Init(j, i);
        }
    }

    currTile = nullptr;
    
    doneAStar = true;
    return S_OK;
}

void AStarScene::Update()
{
    RECT astarArea = { 0, 0, ASTAR_TILE_COUNT * ASTAR_TILE_SIZE,  ASTAR_TILE_COUNT * ASTAR_TILE_SIZE };

    if (PtInRect(&astarArea, g_ptMouse))
    {
        int idX = g_ptMouse.x / ASTAR_TILE_SIZE;
        int idY = g_ptMouse.y / ASTAR_TILE_SIZE;

        if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
        {
            if (KeyManager::GetSingleton()->IsStayKeyDown('S'))
            {
                map[startPoint.y][startPoint.x].SetType(AstarTileType::None);
                map[idY][idX].SetType(AstarTileType::Start);
                startPoint.x = idX;
                startPoint.y = idY;

                currTile = &(map[idY][idX]);
            }
            else if (KeyManager::GetSingleton()->IsStayKeyDown('D'))
            {
                map[destPoint.y][destPoint.x].SetType(AstarTileType::None);
                map[idY][idX].SetType(AstarTileType::Dest);
                destPoint.y = idY;
                destPoint.x = idX;
            }
            else
            {
                map[idY][idX].SetType(AstarTileType::Wall);
            }
        }
        else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RBUTTON))
        {
            map[idY][idX].SetType(AstarTileType::None);
        }
    }
    //엔터키 누르면 길찾기 1회 진행
    if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_RETURN))
    {
        FindPath();
    }

    if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_SPACE))
    {
        doneAStar = false;
    }

    if (KeyManager::GetSingleton()->IsOnceKeyDown('R'))
    {
        Init();
    }
    if (!doneAStar)
    {
        FindPath();
    }

    
}

void AStarScene::Render(HDC hdc)
{
    for (int i = 0; i < ASTAR_TILE_COUNT; i++)
    {
        for (int j = 0; j < ASTAR_TILE_COUNT; j++)
        {
            map[i][j].Render(hdc);
        }
    }
}

void AStarScene::Release()
{
}

void AStarScene::FindPath()
{

    // 1. 현재 노드의 상하좌우 노드 중 후보 노드를 찾는다.
    //Step1

    switch (AddOpenList(currTile))
    {
    case 2: 
        currTile = GetMinFTile();
        currTile->SetType(AstarTileType::Curr);
        break;
    case 1:
        break;
    case 0:
        PrintPath();
        break;
    }
    //if (AddOpenList(currTile)==2)
    //{
    //    // 오픈리스트에 있는 후보들 중에 F 값이 가장 작은 노드를 찾는다.
    //    //Step2
    //    currTile = GetMinFTile();
    //    currTile->SetType(AstarTileType::Curr);
    //}
    //else if(AddOpenList(currTile) == 1)//목적지 찾았을 때 
    //{
    //    // 경로를 출력한다.
    //    PrintPath();
    //}
}

int AStarScene::AddOpenList(AstarTile* currTile)
{
    int tmpIdX, tmpIdY;
#pragma region Step1
    for (int i = -1; i <= 1; i += 2) //상하
    {
        if (currTile->GetIdY() == 0 && i == -1) continue;
        if (currTile->GetIdY() == ASTAR_TILE_COUNT - 1 && i == 1) continue;

        tmpIdY = currTile->GetIdY() + i;
        tmpIdX = currTile->GetIdX();

        if (map[tmpIdY][tmpIdX].GetType() == AstarTileType::Dest)
        {
            map[destPoint.y][destPoint.x].SetParentTile(currTile);
            return 0;
        }


        // 상하좌우의 노드이 이동할 수 있는 타입이면 오픈리스트에 추가
        if (map[tmpIdY][tmpIdX].GetType() == AstarTileType::None)
        {
            map[tmpIdY][tmpIdX].SetType(AstarTileType::Walkable);
            map[tmpIdY][tmpIdX].SetParentTile(currTile);
            openList.push_back(&map[tmpIdY][tmpIdX]);
        }

    }

    for (int j = -1; j <= 1; j += 2) // 좌우
    {
        if (currTile->GetIdX() == 0 && j == -1) continue;
        if (currTile->GetIdX() == ASTAR_TILE_COUNT - 1 && j == 1) continue;

        tmpIdX = currTile->GetIdX() + j;
        tmpIdY = currTile->GetIdY();

        if (map[tmpIdY][tmpIdX].GetType() == AstarTileType::Dest)
        {
            map[destPoint.y][destPoint.x].SetParentTile(currTile);
            return 0;
        }

        // 상하좌우의 노드가 이동할 수 있는 타입이면 오픈리스트에 추가
        if (map[tmpIdY][tmpIdX].GetType() == AstarTileType::None)
        {
            map[tmpIdY][tmpIdX].SetType(AstarTileType::Walkable);
            map[tmpIdY][tmpIdX].SetParentTile(currTile);
            openList.push_back(&map[tmpIdY][tmpIdX]);
        }
    }

    if (openList.size() == 0)
        return 1;

    currTile->SetType(AstarTileType::Closed);
#pragma endregion

    return 2;
}

AstarTile* AStarScene::GetMinFTile()
{
#pragma region Step2
    float F, G, H;
    float minF = FLT_MAX;

    vector<AstarTile*>::iterator it = openList.begin();
    vector<AstarTile*>::iterator itmp;
    for (; it != openList.end(); it++)
    {
        G = (*it)->GetParentTile()->GetG() + 10;
        (*it)->SetG(G);
        //float h = /*현재 노드에서 목적지 노드까지의 예상 비용*/
        H = GetDistance((*it)->GetCenter(), map[destPoint.y][destPoint.x].GetCenter()) * 10;
        (*it)->SetH(H);
        F = G + H;
        (*it)->SetF(F);

        if (minF > F)
        {
            minF = F;
            itmp = it;
            currTile = (*it);
        }
    }

#pragma endregion
    currTile = (*itmp);
    openList.erase(itmp);


    return currTile;
}

void AStarScene::PrintPath()
{
    AstarTile* currPathTile = &map[destPoint.y][destPoint.x];

    while (currPathTile != &(map[startPoint.y][startPoint.x]))
    {
        currPathTile = currPathTile->GetParentTile();
        currPathTile->SetType(AstarTileType::Path);
    }

    doneAStar = true;
}
