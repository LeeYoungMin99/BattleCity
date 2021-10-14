#pragma once
#include "Config.h"
#include "GameEntity.h"

enum selectedTitle{player_1, player_2, CONSTRUCTION};

class Image;
class TitleScene : public GameEntity
{
private:
	Image* title;
	Image* backGround;
	Image* tankUi;

	POINT pos;

	LPARGUMENT_PTR arg;
	selectedTitle selecTitle;
	int test; //�����̵�ȿ�� �׽�Ʈ
	bool titleStart; //������

public:
	virtual HRESULT Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;

};

