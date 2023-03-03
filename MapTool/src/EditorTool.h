#pragma once
#include "Engine_include.h"
#include "LevelEditor.h"


class EditorTool : public GuiWidget
{
public:
	//LevelInstanced* current_foliage = nullptr;
	//vector<LevelInstanced> folliage_list;
public:
	void Active();

public:
	void Init(LevelEditor* editing_level, entt::registry& reg);
	void Release();
public:
	//virtual void Update() override;
	//virtual void Render() override;

private:
	char mesh_id[128] = { 0, };
	char tex_id[128] = { 0, };
	char ps_id[128] = { 0, };
	
private:
	LevelEditor* editing_level = nullptr;
	entt::registry* reg = nullptr;
};