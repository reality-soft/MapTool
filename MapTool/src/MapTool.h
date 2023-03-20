#pragma once
#include "Widgets.h"
#include "LevelEditor.h"
#include "ResourceSelector.h"
#include "LightMeshLevel.h"
#include "GuideLineEditor.h"

class MapTool : public reality::Scene
{
public:
	MapTool() {}
	~MapTool() {}

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnRelease();

public:
	//bool LevelEdittingProcess();
	//bool SavedLevelRenderProcess();

	bool edit_mode = true;
	bool wire_frame = false;

	//string current_saved_file;

	SkySphere sky_sphere;
	LightMeshLevel light_mesh_level;
	//LevelEditor* editting_level_ = nullptr;
	//Level* saved_level_ = nullptr;

public:
	GwMainMenu gw_main_menu_;
	GwPorperty gw_property_;
	//GwLevelEditor gw_level_editor_;
	GuideLineEditor gw_guide_line_;

private:
	entt::entity player_ent;
	reality::LightingSystem sys_light;
	reality::CameraSystem sys_camera;
	reality::MovementSystem sys_movement;
	reality::RenderSystem sys_render;

	ResourceSelector res_selector_;
	MovementEvent* move_event = nullptr;
};

