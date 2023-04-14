#include "GuideLineEditor.h"
#include "Widgets.h"
#include "PickingMgr.h"

void GuideLineEditor::AddNewGuideLine(string name, int _guide_type)
{
	GuideLine::GuideType guide_type = (GuideLine::GuideType)_guide_type;
	string guide_line_mat;
	switch (guide_type)
	{
	case GuideLine::GuideType::eBlocking:
		guide_line_mat = "BlockingGuide.mat";
		break;
	case GuideLine::GuideType::eNpcTrack:
		guide_line_mat = "NpcGuide.mat";
		break;
	}

	GuideLine* new_guide_line = new GuideLine;
	InstancedObject* new_node_mark = new InstancedObject;

	new_node_mark->Init("SkySphere.stmesh", "InstancingVS.cso", guide_line_mat);
	new_guide_line->Init(GuideLine::GuideType::eNpcTrack);

	new_guide_line->guide_type_ = guide_type;

	guide_lines.insert(make_pair(name, new_guide_line));
	node_marks.insert(make_pair(name, new_node_mark));

	current_name = name;
}

void GuideLineEditor::SelectGuideLine(string name)
{
	current_name = name;
	current_guide = guide_lines.find(name)->second;
	current_mark = node_marks.find(name)->second;
}

void GuideLineEditor::AddNewNode(XMVECTOR position)
{
	if (current_guide == nullptr || current_mark == nullptr)
		return;

	current_pin = current_mark->AddNewInstance(current_name + string("_pin"))->index;
	current_mark->SetInstanceScale(current_pin, {25, 25, 25});
	current_guide->AddNode(position);
}

void GuideLineEditor::DrawGuideLine()
{
	if (current_guide == nullptr || current_mark == nullptr)
		return;

	current_mark->Frame();
	current_mark->Render();
}

void GuideLineEditor::Active()
{
	if (GUI->FindWidget(GWNAME(*this)) == nullptr)
	{
		GUI->AddWidget(GWNAME(*this), this);
	}
	else
	{
		NOT(GUI->FindWidget(GWNAME(*this))->open_);
	}
}

void GuideLineEditor::Update()
{
	if (current_mark == nullptr)
		return;

	if (DINPUT->GetMouseState(L_BUTTON) == KEY_HOLD)
	{
		current_mark->SetInstanceTranslation(current_pin, XMFLOAT3(PICKING->current_point.m128_f32));
	
		UINT index = 0;
		for (auto& pin : current_mark->instance_pool)
		{
			current_guide->line_nodes[index++] = XMLoadFloat3(&pin.second->T);
		}

		current_guide->UpdateLines();

	}
}

void GuideLineEditor::Render()
{
	ImGui::Begin("GuideLines", &open_);
	{
		if (ImGui::BeginListBox("##LIST"))
		{
			for (auto& guide_line : guide_lines)
			{
				if (ImGui::Selectable(guide_line.first.c_str()))
				{
					current_name = guide_line.first;
					SelectGuideLine(guide_line.first);
				}
			}

			ImGui::EndListBox();
		}

		static int guide_type = 0;
		ImGui::RadioButton("BLocking Field", &guide_type, 0);
		ImGui::RadioButton("Npc Track", &guide_type, 1);

		static char buffer[128] = { 0, };
		ImGui::InputText("##string", buffer, 128);
		ImGui::SameLine();

		if (ImGui::Button("Add New Guide Line"))
		{
			AddNewGuideLine(string(buffer), guide_type);
			ZeroMemory(buffer, sizeof(buffer));
		}
	}

	if (!current_name.empty())
	{
		ImGui::Text(string("Selected Pin : " + current_pin).c_str());
		if (ImGui::TreeNode(current_name.c_str()))
		{
			for (auto node : node_marks.find(current_name)->second->instance_pool)
			{				
				if (ImGui::Selectable(node.second->instance_id.c_str()))
				{
					current_pin = node.second->index;
				}
			}

			ImGui::TreePop();
		}

		if (ImGui::Button("+"))
		{
			AddNewNode(XMVectorZero());
		}
		ImGui::SameLine(); 
		if (ImGui::Button("-"))
		{

		}
	}

	if (current_guide)
	{
		for (auto& node : current_guide->line_nodes)
		{
			ImGui::Text(string("[" + to_string(node.first) + "]").c_str());
			ImGui::Text(VectorToString(node.second).c_str());
		}
	}

	if (ImGui::Button("Save To Map Data"))
	{
		FileTransfer out_mapdata("../../Contents/BinaryPackage/DeadPoly_GuideLine.mapdat", WRITE);

		UINT num_guide_lines = guide_lines.size();
		out_mapdata.WriteBinary<UINT>(&num_guide_lines, 1);

		for (auto& guide_line : guide_lines)
		{
			UINT num_nodes = guide_line.second->line_nodes.size();
			out_mapdata.WriteBinary<UINT>(&num_nodes, 1);

			for (auto& node : guide_line.second->line_nodes)
			{
				UINT node_nunber = node.first;
				XMVECTOR node_pos = node.second;
				out_mapdata.WriteBinary<UINT>(&node_nunber, 1);
				out_mapdata.WriteBinary<XMVECTOR>(&node_pos, 1);
			}
		}

	}

	DrawGuideLine();


	ImGui::End();
}
