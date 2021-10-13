#include "GameEntity.h"
#include "Config.h"

#pragma once
class GameBoard : public GameEntity
{
	

public:
	virtual HRESULT Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
};

