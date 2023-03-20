#include "MapTool.h"
#include "ComponentSystem.h"
#include "TestCapsuleActor.h"
#include "PickingMgr.h"
using namespace reality;

void MapTool::OnInit()
{
	RESOURCE->Init("../../Contents/");  
	ComponentSystem::GetInst()->OnInit(SCENE_MGR->GetRegistry());

	//FbxImportOption option;
	//option.import_rotation = { 90, 0, 180, 0 };
	//option.import_scale = 10.0f;
	//FBX->ImportAndSaveFbx("../../Contents/FBX/DeadPoly_FullLevel.fbx", option);

	//GUI
	GUI->AddWidget(GWNAME(gw_main_menu_), &gw_main_menu_);
	GUI->AddWidget(GWNAME(gw_property_), &gw_property_);
	
	res_selector_.Init();

	sky_sphere.CreateSphere();
	light_mesh_level.Create("DeadPoly_FullLevel.ltmesh", "LevelVS.cso", "LevelGS.cso", "DeadPoly_Level_Collision.ltmesh");

	//player_ent = SCENE_MGR->AddPlayer<TestCapsuleActor>();
	//SCENE_MGR->GetActor<TestCapsuleActor>(player_ent)->Create();

	sys_movement.OnCreate(SCENE_MGR->GetRegistry());
	sys_light.OnCreate(SCENE_MGR->GetRegistry());
	sys_camera.TargetTag(SCENE_MGR->GetRegistry(), "Debug");
	sys_camera.OnCreate(SCENE_MGR->GetRegistry());
	sys_camera.SetSpeed(1000);
	sys_render.OnCreate(SCENE_MGR->GetRegistry());

	QUADTREE->Init(&light_mesh_level, 4);
	PICKING->Init(&sys_camera);
}

void MapTool::OnUpdate()
{
	sys_light.UpdateSun(sky_sphere);

	sys_movement.OnUpdate(SCENE_MGR->GetRegistry());
	sys_camera.OnUpdate(SCENE_MGR->GetRegistry());
	sys_light.OnUpdate(SCENE_MGR->GetRegistry());
	sys_render.OnUpdate(SCENE_MGR->GetRegistry());

	gw_property_.collide_pos = XMFLOAT4(PICKING->current_point.m128_f32);
	// Gui Msg Proc;

	switch (gw_main_menu_.msg_)  
	{
	case MsgType::NONE: break;

	case MsgType::OW_RES_VIEWER:
	{
		res_selector_.Active();
	} break;
	case MsgType::OW_GUIDE_LINE:
	{
		gw_guide_line_.Active();
		break;
	}
	case MsgType::OPT_WIREFRAME:
		NOT(wire_frame);
	}

	QUADTREE->Frame(&sys_camera);
	PICKING->Frame();
}

void MapTool::OnRender()
{   
	if (wire_frame)
		DX11APP->SetWireframes();

	//LevelEdittingProcess();

	sky_sphere.FrameRender(sys_camera.GetCamera());
	gw_property_.camera = XMFLOAT3(sys_camera.GetCamera()->camera_pos.m128_f32);
	light_mesh_level.Update();
	light_mesh_level.Render();

	//GUI
	GUI->RenderWidgets();
}

void MapTool::OnRelease()
{
	RESOURCE->Release();
	QUADTREE->Release();
}

