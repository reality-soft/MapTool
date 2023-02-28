#include "InstancedFoliage.h"
#include "Widgets.h"
#include "PickingMgr.h"

void InstancedFoliage::Active()
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
	
void InstancedFoliage::Init(LevelEditor* editing_level, entt::registry& reg)
{
	this->editing_level = editing_level;
	this->reg = &reg;
}

void InstancedFoliage::Release()
{

}

void InstancedFoliage::Update()
{
	if (current_foliage != nullptr)
	{
		current_foliage->TransformationInstance(0, PICKING->current_point);
		if (DINPUT->GetMouseState(L_BUTTON) == KEY_PUSH)
		{
			current_foliage->AddNewInstance(XMMatrixTranslationFromVector(PICKING->current_point));
		}

		current_foliage->Frame();
		current_foliage->Render();
	}
}

void InstancedFoliage::Render()
{
	if (this->open_ == false)
		return;

	ImGui::Begin("New Foliage", &open_);
	{
		ImGui::Text("Mesh");
		ImGui::InputText("Mesh ResID", mesh_id, 128);
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE");
			if (payload != nullptr)
			{
				char* resid = (char*)payload->Data;
				strcpy(mesh_id, resid);
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::Separator();

		static string result_msg;
		static bool is_generated = true;
		if (ImGui::Button("Grnerate"))
		{
			if (RESOURCE->UseResource<StaticMesh>(mesh_id) == nullptr)
			{
				result_msg = "There's No Resource among Meshes : " + string(mesh_id);
				is_generated = false;
				ZeroMemory(mesh_id, ARRAYSIZE(mesh_id));
			}
			else
			{
				is_generated = true;
				result_msg = "Ready For Use!";
			}
			if (is_generated)
			{
				if (current_foliage != nullptr)
				{
					folliage_list.push_back(*current_foliage);
					delete current_foliage;
					current_foliage = nullptr;
				}
				current_foliage = new LevelInstanced;
				current_foliage->Init(mesh_id, "InstancingVS.cso");
				current_foliage->AddNewInstance(XMMatrixTranslationFromVector(PICKING->current_point));
			}
		}
		switch (is_generated)
		{
		case true:  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1)); break;
		case false: ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1)); break;
		}
		ImGui::Text(result_msg.c_str());
		ImGui::PopStyleColor();


	}
	ImGui::End();
}
