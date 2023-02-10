#pragma once
#include "Widgets.h"

class MapTool : public KGCA41B::Scene
{
public:
	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnRelease();

public:
	void LoadResource();

public:
	GwMainMenu gw_main_menu_;

private:
	entt::registry reg_scene;
	KGCA41B::Level level;

	KGCA41B::RenderSystem sys_render;
	KGCA41B::CameraSystem sys_camera;
	KGCA41B::InputSystem  sys_input;

	entt::entity ent;
	Camera debug_camera_;
	InputMapping debug_input_;
};

