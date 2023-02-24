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

	void Edit();

public:
	void LoadResource();

public:
	GwMainMenu gw_main_menu_;
	GwPorperty gw_property_;
	GwLevelEditor gw_level_editor_;


private:
	entt::registry reg_scene;
	LevelEditor* level_editor_;

	KGCA41B::LightingSystem sys_light;
	KGCA41B::RenderSystem sys_render;
	KGCA41B::CameraSystem sys_camera;
	KGCA41B::InputSystem  sys_input;

	entt::entity ent;
	KGCA41B::C_Camera debug_camera_;
	C_InputMapping debug_input_;

	InstancedFoliage instanced_foliage_;
	ResourceSelector res_selector_;
};

