#include "PickingMgr.h"

void PickingMgr::Init(reality::CameraSystem* _camera)
{
	camera = _camera;  
}  

void PickingMgr::Frame()
{
	reality::MouseRay current_ray = camera->CreateMouseRay();
	reality::WorldRayCallback callback = PHYSICS->WorldPicking(current_ray);

	RPtoXM(callback.hitpoint, current_point);
	current_body = callback.body;
}

void PickingMgr::Release()
{
	current_body = nullptr;
	camera = nullptr;
}
