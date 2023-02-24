#pragma once
#include "Engine_include.h"
#include "CameraSystem.h"

class PickingMgr
{
	SINGLETON(PickingMgr);
#define PICKING PickingMgr::GetInst()
public:
	void Init(KGCA41B::CameraSystem* _camera);
	void Frame();
	void Render();
	void Release();
	XMVECTOR current_point;
	CollisionBody* current_body = nullptr;
private:
	shared_ptr<KGCA41B::CameraSystem> camera;
};
