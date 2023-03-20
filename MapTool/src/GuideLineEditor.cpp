#include "GuideLineEditor.h"
#include "Widgets.h"

void GuideLineEditor::AddNewGuideLine(string name)
{
	GuideLine* new_guide_line = new GuideLine;
	InstancedObject* new_node_mark = new InstancedObject;

	new_node_mark->Init("SkySphere.stmesh", "InstancingVS.cso", "NodeMark_1.mat");
	new_guide_line->Init(GuideLine::GuideType::eNpcTrack);

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

	current_pin = current_mark->AddNewInstance(current_name + string("_pin"))->instance_id;
	current_mark->SetInstanceScale(current_pin, {50, 50, 50});
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

		static char buffer[128] = { 0, };
		ImGui::InputText("##string", buffer, 128);
		ImGui::SameLine();
		if (ImGui::Button("Add New Guide Line"))
		{
			AddNewGuideLine(string(buffer));
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
					current_pin = node.second->instance_id;
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

	DrawGuideLine();


	ImGui::End();
}
