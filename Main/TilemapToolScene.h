#pragma once
#include "Config.h"
#include "GameEntity.h"


enum class State{ NoneWalkable, Walkable  };

class Image;
class Button;
class ButtonFunction;
class TilemapToolScene : public GameEntity
{
private:
	SAMPLE_TILE_INFO sampleTileInfo[SAMPLE_TILE_COUNT_Y * SAMPLE_TILE_COUNT_X];
	TILE_INFO tileInfo[TILE_COUNT_Y * TILE_COUNT_X];
	Image* sampleImage;

	SAMPLE_TILE_INFO	selectedSampleTile;

	vector<SAMPLE_TILE_INFO> selectedSampleTiles;

	int selectedIdX, selectedIdY;
	int posX, posY;
	State state;
	TileType tileType;

	bool bShowNoneWalkable;
	bool bShowBodyCollider;
public:
	virtual ~TilemapToolScene() = default;

	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

	void Save(int saveIndex=0);
	void Load(int loadIndex=0);
};