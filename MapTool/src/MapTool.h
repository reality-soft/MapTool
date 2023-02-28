#pragma once
#include "Widgets.h"
#include "LevelEditor.h"
#include "InstancedFoliage.h"
#include "ResourceSelector.h"
#include "Object/LevelInstanced.h"

class MapTool : public KGCA41B::Scene
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
	string current_saved_file;

	LevelEditor* editting_level_ = nullptr;
	Level* saved_level_ = nullptr;

public:
	GwMainMenu gw_main_menu_;
	GwPorperty gw_property_;
	GwLevelEditor gw_level_editor_;

private:
	entt::registry reg_scene;

	KGCA41B::LightingSystem sys_light;
	KGCA41B::CameraSystem sys_camera;


	InstancedFoliage instanced_foliage_;
	ResourceSelector res_selector_;
};

