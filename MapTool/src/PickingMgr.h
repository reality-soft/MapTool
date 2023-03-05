#pragma once
#include "Engine_include.h"
#include "CameraSystem.h"

class PickingMgr
{
	SINGLETON(PickingMgr);
#define PICKING PickingMgr::GetInst()  
public:
	void Init(reality::CameraSystem* _camera);
	void Frame();
	void Release();  
	XMVECTOR current_point;
	CollisionBody* current_body = nullptr;

private:
	reality::CameraSystem* camera = nullptr;
};

