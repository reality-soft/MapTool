#pragma once
#include "Widgets.h"
#include "LevelEditor.h"
#include "ResourceSelector.h"
#include "StaticMeshLevel.h"
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

	bool edit_mode = true;
	bool wire_frame = false;

	Environment environment_;
	StaticMeshLevel light_mesh_level;

	reality::LightingSystem sys_light;
	reality::CameraSystem sys_camera;

public:
	GwMainMenu gw_main_menu_;
	GwPorperty gw_property_;
	GuideLineEditor gw_guide_line_;
	ResourceSelector res_selector_;
};

