#include "MapTool.h"
#include "ComponentSystem.h"
#include "PickingMgr.h"
#include "FbxMgr.h"
using namespace reality;

void MapTool::OnInit()
{
	RESOURCE->Init("../../Contents/");
	ComponentSystem::GetInst()->OnInit(SCENE_MGR->GetRegistry());

	//GUI
	GUI->AddWidget(GWNAME(gw_main_menu_), &gw_main_menu_);
	GUI->AddWidget(GWNAME(gw_property_), &gw_property_);
	
	res_selector_.Init();
	

	light_mesh_level.Create("DNDLevel_WithCollision_01.stmesh", "LevelVS.cso");
	

	sys_light.OnCreate(SCENE_MGR->GetRegistry());
	sys_camera.TargetTag(SCENE_MGR->GetRegistry(), "Debug");
	sys_camera.OnCreate(SCENE_MGR->GetRegistry());
	sys_camera.SetSpeed(1000);

	QUADTREE->Init(&light_mesh_level,SCENE_MGR->GetRegistry());
	QUADTREE->ImportQuadTreeData("../../Contents/BinaryPackage/QuadTreeData_01.matdat");
	QUADTREE->CreatePhysicsCS();
	QUADTREE->InitCollisionMeshes();

	//QUADTREE->CreateQuadTreeData(4);
	//QUADTREE->ExportQuadTreeData("../../Contents/BinaryPackage/QuadTreeData_01.matdat");
	//QUADTREE->ImportQuadTreeData("../../Contents/BinaryPackage/QuadTreeData_01.matdat");

	PICKING->Init(&sys_camera);

	environment_.CreateEnvironment();
	environment_.SetWorldTime(60, 60, true);
	environment_.SetSkyColorByTime(RGB_TO_FLOAT(201, 205, 204), RGB_TO_FLOAT(11, 11, 19));
	environment_.SetFogDistanceByTime(10000, 10000);
	environment_.SetLightProperty(0.2f, 0.2f);
}

void MapTool::OnUpdate()
{
	QUADTREE->Frame(&sys_camera);
	QUADTREE->UpdatePhysics("PhysicsCS.cso");
	sys_camera.OnUpdate(SCENE_MGR->GetRegistry());
	sys_light.OnUpdate(SCENE_MGR->GetRegistry());
	environment_.Update(&sys_camera, &sys_light);

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

	gw_property_.camera = XMFLOAT3(sys_camera.GetCamera()->camera_pos.m128_f32);

	environment_.Render();
	light_mesh_level.Update();
	light_mesh_level.Render();
	QUADTREE->RenderCollisionMeshes();

	//GUI
	GUI->RenderWidgets();
}

void MapTool::OnRelease()
{
	RESOURCE->Release();
	QUADTREE->Release();
}

