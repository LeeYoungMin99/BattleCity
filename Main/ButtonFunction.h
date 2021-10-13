#pragma once
#include "Config.h"
#include "Button.h"

class ButtonFunction
{
public:
	void ChangeScene(ArgumentFuncPtr* arg = nullptr);
	void QuitProgram(ArgumentFuncPtr* arg = nullptr);
	void Save(ArgumentFuncPtr* arg =nullptr);
	void Load(ArgumentFuncPtr* arg = nullptr);
};