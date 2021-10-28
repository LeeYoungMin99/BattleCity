#include "KeyManager.h"

void KeyManager::Init()
{
	for (int i = 0; i < KEY_MAX_COUNT; i++)
	{
		keyUp[i] = true;
		keyDown[i] = false;

		// 안 쓰는 주석
	}
}

void KeyManager::Release()
{
}

bool KeyManager::IsOnceKeyDown(int key)
{
	// 안 쓰는 주석
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (keyDown[key] == false)
		{
			keyDown[key] = true;
			return true;
		}
	}
	else
	{
		keyDown[key] = false;
	}
	return false;
}

bool KeyManager::IsOnceKeyUp(int key)
{
	// 실습1. 이 함수를 완성해보자.
	if (GetAsyncKeyState(key) & 0x8000)
	{
		keyUp[key] = false;
	}
	else
	{
		if (keyUp[key] == false)
		{
			keyUp[key] = true;
			return true;
		}
	}

	return false;
}

bool KeyManager::IsStayKeyDown(int key)
{
//	bool b = 0x8000 & 0x8001;

	static time_t currTime;
	// 실습2. 얘도 해보자.
	if (GetAsyncKeyState(key) & 0x8001)
	{
		return true;
	}
	// 안 쓰는 코드

	return false;
}
