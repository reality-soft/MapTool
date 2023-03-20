#pragma once
#include "Engine_include.h"
#include "GuideLine.h"
#include "InstancedObject.h"

using namespace reality;

class GuideLineEditor : public reality::GuiWidget
{
public:
	map<string, GuideLine*> guide_lines;
	map<string, InstancedObject*> node_marks;

	GuideLine* current_guide = nullptr;
	InstancedObject* current_mark = nullptr;
	string current_name;
	string current_pin;

	SingleMesh<Vertex> line_mesh;

	
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;

	void AddNewGuideLine(string name);
	void SelectGuideLine(string name);
	void AddNewNode(XMVECTOR position);
	void DrawGuideLine();
	
	void Active();
	virtual void Update() override;
	virtual void Render() override;
};

