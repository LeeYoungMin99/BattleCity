#pragma once
#include <Windows.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <map>
#include <iostream>
#include <string>

using namespace std;

#pragma comment(lib, "winmm.lib")

#define WIN_START_POS_X	400
#define WIN_START_POS_Y	100
#define WIN_SIZE_X	512
#define WIN_SIZE_Y	448
#define TILEMAPTOOL_SIZE_X	800
#define TILEMAPTOOL_SIZE_Y	600
#define PIXELMAP_SIZE_X	1000
#define PIXELMAP_SIZE_Y	800

#define PI 3.14159265357989
#define PI2 (3.14159265357989 * 2)

#define DEGREE_TO_RADIAN(x)		(x * PI / 180.0f)

#define SAFE_RELEASE(p)	{ if (p) { p->Release(); delete p; p = nullptr; } }
#define SAFE_DELETE(p)	{ if (p) { delete p; p = nullptr; } }

enum MoveDir { Left = 2, Right = 6, Up = 0, Down = 4 };
enum class TankType { Player, Normal = 0 , Speed, Rapid, Defensive };


#include "TimerManager.h"
#include "ImageManager.h"
#include "KeyManager.h"
#include "SceneManager.h"

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;



//타일 관련
#define TILE_SIZE	16
#define TILE_COUNT_X	26
#define TILE_COUNT_Y	26
#define SAMPLE_TILE_COUNT_X 8
#define SAMPLE_TILE_COUNT_Y	1
#define STAGE_SIZE_X (WIN_SIZE_X / 2 - 8 * TILE_COUNT_X - 16)
#define STAGE_SIZE_Y (WIN_SIZE_Y / 2 - 8 * TILE_COUNT_Y)

#define RANDOM(min, max) (rand() % ((max) - (min) + 1) + (min))
#define CORRECTION_POS_MIN 6
#define CORRECTION_POS_MAX 10

enum class Terrain { Earth, Wall, Water, Grass, BreakableWall, End };

enum class TileType {Ground, Wall, Water, Grass, Brick};
typedef struct tagSampleTile
{
	RECT rc;
	int frameX, frameY;
} SAMPLE_TILE_INFO;

typedef struct tagTile
{
	Terrain terrain;
	RECT rc;				// bullet Hit Collider
	//image
	// sample tile의 아이디 (프레임 X, 프레임 Y)
	int frameX, frameY;

	RECT collider;		// walkable, noneWalkable 일때 

} TILE_INFO;

typedef struct ArgumentFuncPtr
{
	string sceneName;
	string loadingSceneName;
	TILE_INFO* tileInfo;
	int saveIndex;
	int loadIndex;
} ARGUMENT_PTR, * LPARGUMENT_PTR;