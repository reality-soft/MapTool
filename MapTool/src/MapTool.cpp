#include "MapTool.h"
#include "ComponentSystem.h"
#include "PickingMgr.h"

using namespace KGCA41B;

void MapTool::OnInit()
{
	RESOURCE->Init("../../Contents/");  

	ComponentSystem::GetInst()->OnInit(reg_scene);

	sys_light.OnCreate(reg_scene);  
	sys_camera.TargetTag(reg_scene, "Debug");  
	sys_camera.OnCreate(reg_scene);

	//GUI
	GUI->AddWidget(GWNAME(gw_main_menu_), &gw_main_menu_);
	GUI->AddWidget(GWNAME(gw_property_), &gw_property_);
	
	PICKING->Init(&sys_camera);
	//instanced_foliage_.Init(level_editor_, reg_scene);
	res_selector_.Init();

	sphere.CreateSphere();

}

void MapTool::OnUpdate()
{
	sys_camera.OnUpdate(reg_scene);
	sys_light.OnUpdate(reg_scene);

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
			gw_level_editor_.editing_level = this->editting_level_;
			GUI->AddWidget(GWNAME(gw_level_editor_), &gw_level_editor_);  
			edit_mode = true;
		}
		else
		{
			NOT(GUI->FindWidget(GWNAME(gw_level_editor_))->open_);
		}
	} break;
	case MsgType::OPT_WIREFRAME:
		NOT(wire_frame);
	}
}

void MapTool::OnRender()
{   
	if (wire_frame)
		DX11APP->SetWireframes();

	else
		DX11APP->SetSolid();

	if (gw_level_editor_.loaded_level)
	{
		this->saved_level_ = gw_level_editor_.loaded_level;
		edit_mode = false;
		SavedLevelRenderProcess();
	}

	else
		LevelEdittingProcess();



	//GUI
	GUI->RenderWidgets();
}

void MapTool::OnRelease()
{
	if (editting_level_)
	{
		delete editting_level_;
		editting_level_ = nullptr;
	}
	if (saved_level_)
	{
		delete saved_level_;
		saved_level_ = nullptr;
	}


	PICKING->Release();
	RESOURCE->Release();
	QUADTREE->Release();
}

bool MapTool::LevelEdittingProcess()
{
	if (edit_mode == false)
	{
		if (editting_level_)
		{
			delete editting_level_;
			editting_level_ = nullptr;
		}
		return false;
	}

	if (editting_level_ == nullptr)
	{
		editting_level_ = new LevelEditor;

		if (saved_level_)
		{
			// Copy From saved Level
			editting_level_->CopyFromSavedLevel(saved_level_);
			editting_level_->SetCamera(sys_camera.GetCamera());
			delete saved_level_;
			saved_level_ = nullptr;
		}
		else  
		{
			// create basic level  
			bool created = editting_level_->CreateLevel(3, 12, 12, { 16 , 16 });
			editting_level_->SetCamera(sys_camera.GetCamera());
			editting_level_->vs_id_ = "LevelEditorVS.cso";
			editting_level_->ps_id_ = "LevelEditorPS.cso";
			editting_level_->gs_id_ = "LevelEditorGS.cso";
			editting_level_->texture_id[0] = "WhiteTile.png";

			editting_level_->CreateEditSOStage();
		}
	}

	// Updates aobut edittings
	{
		PICKING->Frame();  
		editting_level_->Update();
		editting_level_->SetEditSOStage();
	}

	// Render editting level  
	{
		editting_level_->Render(false);
	}

	return true;
}

bool MapTool::SavedLevelRenderProcess()
{
	if (edit_mode)
	{
		if (saved_level_)
		{
			QUADTREE->Release();
		}
		return false;
	}

	saved_level_->SetCamera(sys_camera.GetCamera());

	// Updates
	{
		QUADTREE->Frame(&sys_camera);
	}

	// Rendering
	{
		QUADTREE->Render();
	}

	return true;
}

