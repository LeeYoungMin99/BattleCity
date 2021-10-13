#pragma once
#include "Config.h"
#include "GameEntity.h"

class Image;
class Tank;
class PixelCollisionScene : public GameEntity
{
private:
	Image* ball;
	Image* mountain;

	POINTFLOAT  posBall;

	float moveSpeed;
	float maxMoveSpeed;
	float accel;

	Tank* tank;

	HPEN hpen;
	HPEN hpenOld;
	HBRUSH hbr;

public:
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

};

