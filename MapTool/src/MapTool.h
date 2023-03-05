#pragma once
#include "Widgets.h"
#include "LevelEditor.h"
#include "EditorTool.h"
#include "ResourceSelector.h"

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
	bool LevelEdittingProcess();
	bool SavedLevelRenderProcess();

	bool edit_mode = true;
	bool wire_frame = false;

	string current_saved_file;

	LevelEditor* editting_level_ = nullptr;
	Level* saved_level_ = nullptr;
	SkySphere sphere;

public:
	GwMainMenu gw_main_menu_;
	GwPorperty gw_property_;
	GwLevelEditor gw_level_editor_;

private:
	entt::registry reg_scene;

	reality::LightingSystem sys_light;
	reality::CameraSystem sys_camera;
	ResourceSelector res_selector_;
};

