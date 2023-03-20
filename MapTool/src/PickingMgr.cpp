#include "PickingMgr.h"

using namespace reality;

void PickingMgr::Init(reality::CameraSystem* _camera)
{
	camera = _camera;  
	current_point = XMVectorZero();
}  

void PickingMgr::Frame()
{
	RayShape mouse_ray;
	if (DINPUT->GetMouseState(L_BUTTON) == KEY_HOLD)
	{
		mouse_ray = camera->CreateMouseRay();
		auto result = QUADTREE->RaycastAdjustLevel(mouse_ray, 10000);
		if (result.success)
			current_point = result.point;
	}
}

void PickingMgr::Release()
{
	camera = nullptr;
}
