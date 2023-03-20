#pragma once
#include "Engine_include.h"
class TestCapsuleActor : public reality::Character
{
public:
	void Create();
	void Update();

public:
	XMVECTOR translation;
};

