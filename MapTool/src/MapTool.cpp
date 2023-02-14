#include "MapTool.h"
#include "ComponentSystem.h"

using namespace KGCA41B;

void MapTool::OnInit()
{
	DINPUT->Init(ENGINE->GetWindowHandle(), ENGINE->GetInstanceHandle());

	LoadResource();
	
	level.CreateLevel(128, 128, 10, 10);
	level.CreateHeightField(-1, 1);  
	level.vs_id_ = "LevelVS.cso";
	level.ps_id_ = "LevelPS.cso";
	level.texture_id = { "Ground.png" };

	ComponentSystem::GetInst()->OnInit(reg_scene);

	debug_camera_.position = { 0, 300, 0, 0 };
	debug_camera_.look = { 0, -1, 0, 0 };
	debug_camera_.up = { 0, 1, 0, 0 };
	debug_camera_.near_z = 1.f;
	debug_camera_.far_z = 10000.f;
	debug_camera_.fov = XMConvertToRadians(45);
	debug_camera_.yaw = 0;
	debug_camera_.pitch = 0;
	debug_camera_.roll = 0;
	debug_camera_.speed = 100;
	debug_camera_.tag = "Player";
	reg_scene.emplace<Camera>(ent, debug_camera_);

	debug_input_.tag = "Player";
	reg_scene.emplace<InputMapping>(ent, debug_input_);

	sys_render.OnCreate(reg_scene);
	sys_camera.TargetTag(reg_scene, "Player");
	sys_camera.OnCreate(reg_scene);
	sys_input.OnCreate(reg_scene);

	//GUI
	GUI->AddWidget(GWNAME(gw_main_menu_), &gw_main_menu_);
	GUI->AddWidget(GWNAME(gw_property_), &gw_property_);
}

void MapTool::OnUpdate()
{
	sys_input.OnUpdate(reg_scene);
	sys_camera.OnUpdate(reg_scene);

	gw_property_.camera_pos = sys_camera.GetCamera()->position;

	MouseRay ray = sys_camera.CreateMouseRay();   
	gw_property_.ndc_pos = sys_camera.ndc;

	XMVECTOR hitpoint = level.LevelPicking(ray, 100.f, XMFLOAT4(0.5, 0, 0, 1.f));
	gw_property_.ray_hitpoint_ = hitpoint;

	level.Update();


}

void MapTool::OnRender()
{   
	level.Render();
	sys_render.OnUpdate(reg_scene);

	//GUI
	GUI->RenderWidgets();  

	switch (gw_main_menu_.msg_)
	{
	case MsgType::NONE: break;

	case MsgType::OW_RES_VIEWER:
	{
		GwResViewer* gw_res_viewer = new GwResViewer;
		if (GUI->FindWidget(GWNAME(gw_res_viewer)) == nullptr)
		{
			GUI->AddWidget(GWNAME(gw_res_viewer), gw_res_viewer);
		}
	}
	}
}

void MapTool::OnRelease()
{
	RESOURCE->Release();
}

void MapTool::LoadResource()
{
	RESOURCE->Init("../../Contents/");
}