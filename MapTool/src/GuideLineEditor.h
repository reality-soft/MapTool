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

	string current_name;
	UINT current_pin = 0;
	
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;

	void AddNewGuideLine(string name, int guide_type);
	void SelectGuideLine(string name);
	void AddNewNode(XMVECTOR position);
	void DrawGuideLine();
	
	void Active();
	virtual void Update() override;
	virtual void Render() override;
};

