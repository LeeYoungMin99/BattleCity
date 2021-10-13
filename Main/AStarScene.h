#pragma once
#include "Config.h"
#include "GameEntity.h"

#define ASTAR_TILE_SIZE 64
#define ASTAR_TILE_COUNT 12

enum AstarTileType { Start, Dest, Wall, None, Walkable, Curr, Closed, Path, End };

class AstarTile : public GameEntity
{
private:
	int idX, idY;
	RECT rc;
	POINTFLOAT center;

	float F;		// G+H
	float G;		// 출발 노드에서 현재 노드까지 이동 비용
	float H;		// 현재 노드에서 목적지까지 예상 비용

	AstarTile* parentTile;

	//타일의 종류에 따른 다른 색상 출력
	AstarTileType type;

	COLORREF color;
	HBRUSH hBrush;
	HBRUSH hTypeBrush[AstarTileType::End];
	HBRUSH hOldBrush;
	

public:
	virtual HRESULT Init();
	HRESULT Init(int idX, int idY);
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	int GetIdX() { return this->idX; };
	int GetIdY() { return this->idY; };


	void SetType(AstarTileType type);
	AstarTileType GetType() { return type; }

	void SetParentTile(AstarTile* parent) { this->parentTile = parent; }
	AstarTile* GetParentTile() { return this->parentTile; }
	
	void SetF(float f) { this->F = f; }
	float GetF() { return this->F; }
	void SetG(float g) { this->G = g; }
	float GetG() { return this->G; }
	void SetH(float h) { this->H = h; }
	float GetH() { return this->H; }

	POINTFLOAT GetCenter() { return this->center; }
};


class AStarScene : public GameEntity
{
private:
	AstarTile map[ASTAR_TILE_COUNT][ASTAR_TILE_COUNT];
	POINT startPoint;
	POINT destPoint;

	vector<AstarTile*> openList;

	AstarTile* currTile;		//현재 기준 타일

	bool doneAStar;
	int i = 0;

public:
	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	void FindPath();
	int AddOpenList(AstarTile* currTile);
	AstarTile* GetMinFTile();

	void PrintPath();
};

