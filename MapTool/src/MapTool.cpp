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
			//gw_level_editor_.editing_level = this->level_editor_;
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
	case MsgType::OPT_WIREFRAME:
		NOT(QUADTREE->wire_frame);
	}
}

void MapTool::OnRender()
{   
	LevelEdittingProcess();

	SavedLevelRenderProcess();

	//GUI
	GUI->RenderWidgets();
}

void MapTool::OnRelease()
{
	if (editting_level_)
	{
		editting_level_->ExportToFile(current_saved_file);
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

			delete saved_level_;
			saved_level_ = nullptr;
		}
		else
		{
			// create basic level
			bool created = editting_level_->CreateLevel(3, 12, 12, { 16 , 16 });
			editting_level_->vs_id_ = "LevelEditorVS.cso";
			editting_level_->ps_id_ = "LevelEditorPS.cso";
			editting_level_->gs_id_ = "LevelEditorGS.cso";
			editting_level_->texture_id = { "WhiteTile.png" };
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

	// Edit process
	{
		editting_level_->LevelEdit();
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

	if (saved_level_ == nullptr)
	{
		// Import Saved Level
		saved_level_ = new Level;
		bool imported = saved_level_->ImportFromFile(current_saved_file);
		editting_level_->vs_id_ = "LevelVS.cso";
		editting_level_->ps_id_ = "LevelPS.cso";

		QUADTREE->Init(saved_level_);
	}

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

