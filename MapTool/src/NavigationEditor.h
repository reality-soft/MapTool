#pragma once
#include "Engine_Include.h"
#include "GuideLine.h"
#include "InstancedObject.h"

class NavigationEditor : public reality::GuiWidget
{
public:
	void Active();
	virtual void Update() override;
	virtual void Render() override;
	void FloydWarshall();
public:
	InstancedObject node_pins_;

	XMVECTOR A, BA, AB, B;
	map<UINT, FloydRout> floyd_routs_;
	map<UINT, XMVECTOR> total_nodes_;

	int row_node_filling_ = 0;
	int col_node_filling_ = 0;

private:
	int selected_A_or_B = 0;
	bool initialized = false;
};

