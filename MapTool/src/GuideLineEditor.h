#pragma once
#include "Engine_include.h"
#include "GuideLine.h"
#include "InstancedObject.h"

using namespace reality;

class GuideLineEditor : public reality::GuiWidget
{
public:
	map<string, GuideLine*> guide_lines;
	GuideLine* current_guide = nullptr;

	map<string, InstancedObject*> node_marks;
	InstancedObject* current_mark = nullptr;
	InstanceData* current_pin = nullptr;

	string current_name;
	
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;

	vector<entt::entity> flame_effects;
	vector<entt::entity> custum_meshs;

	bool view_pin_mesh = true;

	void AddNewGuideLine(string name, int guide_type);
	void SelectGuideLine(string name);
	void AddNewNode(XMVECTOR position);
	void DrawGuideLine();
	
	void Active();
	virtual void Update() override;
	virtual void Render() override;

	entt::entity AddCustumMeshActor(string mesh_id, string vs_id);
	void SetActorPos(entt::entity ent, XMFLOAT3 position);
};

