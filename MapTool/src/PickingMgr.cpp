#include "PickingMgr.h"

void PickingMgr::Init(KGCA41B::CameraSystem* _camera)
{
	camera = _camera;
}

void PickingMgr::Frame()
{
	KGCA41B::MouseRay current_ray = camera->CreateMouseRay();	
	KGCA41B::WorldRayCallback callback = PHYSICS->WorldPicking(current_ray);

	RPtoXM(callback.hitpoint, current_point);
	current_body = callback.body;
}

void PickingMgr::Release()
{
	current_body = nullptr;
	camera = nullptr;
}
