#pragma once
#include "Widgets.h"
#include "LevelEditor.h"
#include "ResourceSelector.h"
#include "StaticMeshLevel.h"
#include "GuideLineEditor.h"
#include "StaticShadows.h"
#include "SequenceEditor.h"

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
	reality::EffectSystem sys_effect;
	reality::RenderSystem sys_render;
	//SingleShadow single_shadow_;
	//CubemapShadow cube_map_shadow_;

public:
	GwMainMenu* gw_main_menu_ = nullptr;
	GwPorperty* gw_property_ = nullptr;
	GuideLineEditor gw_guide_line_;
	ResourceSelector res_selector_;
	SequenceEditor gw_sequence_editor_;
};

