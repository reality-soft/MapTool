#include "MapTool.h"
#include "ComponentSystem.h"
#include "PickingMgr.h"
#include "FbxMgr.h"
using namespace reality;

void MapTool::OnInit()
{
	RESOURCE->Init("../../Contents/");
	ComponentSystem::GetInst()->OnInit(SCENE_MGR->GetRegistry());

	FbxImportOption import_option;
	import_option.import_scale = 0.1f;
	//import_option.recalculate_normal = true;
	//FBX->ImportAndSaveFbx("../../Contents/FBX/Vaccine.fbx", import_option, FbxVertexOption::BY_POLYGON_VERTEX);
	
	GUI->AddWidget<GwMainMenu>("mainmenu");
	GUI->AddWidget<GwPorperty>("property");
	gw_main_menu_ = GUI->FindWidget<GwMainMenu>("mainmenu");
	gw_property_ = GUI->FindWidget<GwPorperty>("property");

	res_selector_.Init();

	light_mesh_level.Create("DNDLevel_WithCollision_01.stmesh", "LevelVS.cso");
	
	sys_render.OnCreate(reg_scene_);
	sys_light.OnCreate(SCENE_MGR->GetRegistry());
	sys_light.SetGlobalLightPos({5000, 5000, -5000});
	sys_camera.TargetTag(SCENE_MGR->GetRegistry(), "Debug");
	sys_camera.OnCreate(SCENE_MGR->GetRegistry());
	sys_camera.SetSpeed(1000);
	sys_camera.SetNearZ(1.0f);
	sys_camera.SetFarZ(10000.0f);
	sys_effect.OnCreate(reg_scene_);

	QUADTREE->Init(&light_mesh_level,SCENE_MGR->GetRegistry());
	QUADTREE->ImportQuadTreeData("../../Contents/BinaryPackage/QuadTreeData_01.mapdat");
	QUADTREE->CreatePhysicsCS();
	QUADTREE->InitCollisionMeshes();
	QUADTREE->view_collisions_ = true;
	//QUADTREE->CreateQuadTreeData(4);
	//QUADTREE->ExportQuadTreeData("../../Contents/BinaryPackage/QuadTreeData_01.matdat");
	//QUADTREE->ImportQuadTreeData("../../Contents/BinaryPackage/QuadTreeData_01.matdat");

	PICKING->Init(&sys_camera);
	
	environment_.CreateEnvironment();
	environment_.SetWorldTime(60, 60);
	environment_.SetSkyColorByTime(RGB_TO_FLOAT(201, 205, 204), RGB_TO_FLOAT(11, 11, 19));
	environment_.SetFogDistanceByTime(10000, 10000);
	environment_.SetLightProperty(0.2f, 0.2f);

	//single_shadow_.Init({5000,15000}, {8192,8192}, {1024,1024}, "DepthMapVS.cso", "ShadowVS.cso", "ShadowPS.cso");
	//single_shadow_.static_mesh_level_ = &light_mesh_level;
	//gw_property_->single_shadow = &single_shadow_;

	//cube_map_shadow_.Init({ 10.f, 1000.f }, { 2048, 2048 }, "DepthMapVS.cso");
	//gw_property_->cube_shadow_ = &cube_map_shadow_;
}

void MapTool::OnUpdate()
{
	QUADTREE->Frame(&sys_camera);
	QUADTREE->UpdatePhysics();
	sys_camera.OnUpdate(SCENE_MGR->GetRegistry());
	sys_light.OnUpdate(SCENE_MGR->GetRegistry());
	environment_.Update(&sys_camera, &sys_light);
	sys_effect.OnUpdate(reg_scene_);

	gw_property_->collide_pos = XMFLOAT4(PICKING->current_point.m128_f32);
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

	case MsgType::OW_NAVI_EDITOR:
		gw_navi_editor_.Active();
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

	//single_shadow_.RenderShadowMap();
	//single_shadow_.SetShadowMapSRV();
	//cube_map_shadow_.CreateDepthMap(&light_mesh_level, sys_camera.GetCamera()->camera_pos);
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

