#include "MapTool.h"
#include "ComponentSystem.h"
#include "PickingMgr.h"

using namespace KGCA41B;

void MapTool::OnInit()
{
	DINPUT->Init(ENGINE->GetWindowHandle(), ENGINE->GetInstanceHandle());

	LoadResource();
	
	ComponentSystem::GetInst()->OnInit(reg_scene);

	level_editor_ = new LevelEditor;
	bool created = level_editor_->CreateLevel(65, 65, 10, 1);
	level_editor_->CreateHeightField(-128, 128);  
	level_editor_->vs_id_ = "LevelEditorVS.cso";
	level_editor_->ps_id_ = "LevelEditorPS.cso";
	level_editor_->gs_id_ = "LevelEditorGS.cso";
	level_editor_->texture_id = { "WhiteTile.png" };
	level_editor_->CreateEditSOStage();


	debug_camera_.position = { 0, 100, -200, 0 };
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
	reg_scene.emplace<C_Camera>(ent, debug_camera_);

	debug_input_.tag = "Player";
	reg_scene.emplace<C_InputMapping>(ent, debug_input_);

	sys_light.OnCreate(reg_scene);
	sys_render.OnCreate(reg_scene);
	sys_camera.TargetTag(reg_scene, "Player");
	sys_camera.OnCreate(reg_scene);
	sys_input.OnCreate(reg_scene);

	//GUI
	GUI->AddWidget(GWNAME(gw_main_menu_), &gw_main_menu_);
	GUI->AddWidget(GWNAME(gw_property_), &gw_property_);
	
	PICKING->Init(&sys_camera);
	instanced_foliage_.Init(level_editor_, reg_scene);
	res_selector_.Init();
	//QUADTREE->Init(level_editor_, 4);

	FbxMgr::GetInst()->ImportAndSaveFbx("../../Contents/STM/LeeEnfieldMKIII.fbx");

}

void MapTool::OnUpdate()
{
	sys_input.OnUpdate(reg_scene);
	sys_camera.OnUpdate(reg_scene);
	sys_light.OnUpdate(reg_scene);

	//QUADTREE->Frame(&sys_camera);

	PICKING->Frame();

	// Gui Msg Proc;

	switch (gw_main_menu_.msg_)
	{
	case MsgType::NONE: break;

	case MsgType::OW_RES_VIEWER:
	{
		res_selector_.Active();
	} break;
	case MsgType::OW_LEVEL_EDITOR:
	{
		if (GUI->FindWidget(GWNAME(gw_level_editor_)) == nullptr)
		{
			gw_level_editor_.editing_level = this->level_editor_;
			GUI->AddWidget(GWNAME(gw_level_editor_), &gw_level_editor_);
		}
		else
		{
			NOT(GUI->FindWidget(GWNAME(gw_level_editor_))->open_);
		}
	} break;
	case MsgType::OW_INSTANCED_FOLIAGE:
	{
		instanced_foliage_.Active();
	}
	}
}

void MapTool::OnRender()
{   
	//sys_render.OnUpdate(reg_scene);
	level_editor_->Update();
	level_editor_->SetEditSOStage();
	level_editor_->Render(false);




	//QUADTREE->Render();

	//GUI
	GUI->RenderWidgets();
}

void MapTool::OnRelease()
{
	PICKING->Release();
	RESOURCE->Release();
}

void MapTool::Edit()
{
	level_editor_->LevelEdit();
}

void MapTool::LoadResource()
{
	RESOURCE->Init("../../Contents/");
}