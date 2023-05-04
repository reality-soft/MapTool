#include "MapTool.h"
#include "ComponentSystem.h"
#include "PickingMgr.h"
#include "FbxMgr.h"
using namespace reality;

void MapTool::OnInit()
{
	RESOURCE->LoadDir("../../Contents/Shader/", &ResourceMgr::ImportShaders);
	RESOURCE->Init("../../Contents/");
	ComponentSystem::GetInst()->OnInit(SCENE_MGR->GetRegistry());

	//FbxImportOption import_option;
	//import_option.import_scale = 1.f;
	//import_option.recalculate_normal = true;
	//FBX->ImportAndSaveFbx("../../Contents/FBX/DND_BossZombie/DND_BossZombie_Kick.fbx", import_option, FbxVertexOption::BY_POLYGON_VERTEX);
	//FBX->ImportAndSaveFbx("../../Contents/FBX/DND_BossZombie/DND_BossZombie_Punch_L.fbx", import_option, FbxVertexOption::BY_POLYGON_VERTEX);
	//FBX->ImportAndSaveFbx("../../Contents/FBX/DND_BossZombie/DND_BossZombie_Punch_R.fbx", import_option, FbxVertexOption::BY_POLYGON_VERTEX);
	//FBX->ImportAndSaveFbx("../../Contents/FBX/DND_BossZombie/DND_BossZombie_Run.fbx", import_option, FbxVertexOption::BY_POLYGON_VERTEX);
	//FBX->ImportAndSaveFbx("../../Contents/FBX/DND_BossZombie/DND_BossZombie_StrongPunch.fbx", import_option, FbxVertexOption::BY_POLYGON_VERTEX);
	//FBX->ImportAndSaveFbx("../../Contents/FBX/DND_BossZombie/DND_BossZombie_Walk.fbx", import_option, FbxVertexOption::BY_POLYGON_VERTEX);
	//
	GUI->AddWidget<GwMainMenu>("mainmenu");
	GUI->AddWidget<GwPorperty>("property");
	gw_main_menu_ = GUI->FindWidget<GwMainMenu>("mainmenu");
	gw_property_ = GUI->FindWidget<GwPorperty>("property");

	res_selector_.Init();

	light_mesh_level.Create("DNDLevel_WithCollision_01.stmesh", "LevelVS.cso");
	
	sys_render.OnCreate(reg_scene_);
	sys_light.OnCreate(SCENE_MGR->GetRegistry());
	sys_light.SetGlobalLightPos({5000, 5000, -5000, 0 });
	sys_camera.TargetTag(SCENE_MGR->GetRegistry(), "Debug");
	sys_camera.OnCreate(SCENE_MGR->GetRegistry());
	sys_camera.SetSpeed(500);
	sys_camera.SetNearZ(0.1f);
	sys_camera.SetFarZ(10000.0f);
	sys_camera.SetFov(90);
	sys_effect.OnCreate(reg_scene_);

	QUADTREE->Init(&light_mesh_level,SCENE_MGR->GetRegistry());
	QUADTREE->ImportQuadTreeData("../../Contents/BinaryPackage/QuadTreeData_01.mapdat");
	QUADTREE->CreatePhysicsCS();
	QUADTREE->InitCollisionMeshes();
	QUADTREE->view_collisions_ = true;
	PICKING->Init(&sys_camera);
	
	environment_.CreateEnvironment();
	environment_.SetWorldTime(60, 60);
	environment_.SetSkyColorByTime(RGB_TO_FLOAT(201, 205, 204), RGB_TO_FLOAT(11, 11, 19));
	environment_.SetFogDistanceByTime(10000, 10000);
	environment_.SetLightProperty(XMFLOAT4(1.0, 0.7, 0.5, 1), XMFLOAT4(0.05, 0.05, 0.1, 1), 0.1f, 0.25f);
}

void MapTool::OnUpdate()
{
	QUADTREE->Frame(&sys_camera);
	QUADTREE->UpdatePhysics();
	sys_camera.OnUpdate(SCENE_MGR->GetRegistry());
	sys_light.OnUpdate(SCENE_MGR->GetRegistry());
	environment_.Update(sys_camera.GetCamera()->camera_pos, &sys_light);
	sys_effect.OnUpdate(reg_scene_);

	gw_property_->collide_pos = XMFLOAT4(PICKING->current_point.m128_f32);
	gw_sequence_editor_.sys_camera_ = &sys_camera;
	// Gui Msg Proc;

	switch (gw_main_menu_->msg_)  
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
		break;

	case MsgType::OW_SEQUENCE_EDITOR:
		gw_sequence_editor_.Active();
		GUI->FindWidget<SequenceEditor>("sequence")->sys_camera_ = &sys_camera;
		break;
	}

	QUADTREE->Frame(&sys_camera);
	PICKING->Frame();
}

void MapTool::OnRender()
{
	if (wire_frame)
		DX11APP->SetWireframes();

	gw_property_->camera = XMFLOAT3(sys_camera.GetCamera()->camera_pos.m128_f32);

	environment_.Render();

	light_mesh_level.Update();
	light_mesh_level.Render();


	QUADTREE->RenderCollisionMeshes();

	sys_render.OnUpdate(reg_scene_);

	//GUI
	GUI->RenderWidgets();
}

void MapTool::OnRelease()
{
	RESOURCE->Release();
	QUADTREE->Release();
}

