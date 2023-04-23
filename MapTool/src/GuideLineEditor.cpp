#include "EffectMgr.h"
#include "FX/FX_Flame.h"
#include "GuideLineEditor.h"
#include "Widgets.h"
#include "PickingMgr.h"

void GuideLineEditor::AddNewGuideLine(string name, int _guide_type)
{
	GuideType guide_type = (GuideType)_guide_type;
	string guide_line_mat;
	switch (guide_type)
	{
	case GuideType::eBlocking:
		guide_line_mat = "BlockingGuide.mat";
		break;
	case GuideType::eNpcTrack:
		guide_line_mat = "NpcGuide.mat";
		break;
	case GuideType::eSpawnPoint:
		guide_line_mat = "SpawnPoint.mat";
		break;
	}

	GuideLine* new_guide_line = new GuideLine;
	InstancedObject* new_node_mark = new InstancedObject;

	new_node_mark->Init("SkySphere.stmesh", "InstancingVS.cso", guide_line_mat);
	new_guide_line->Init(guide_type);

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

	current_pin = current_mark->AddNewInstance(current_name + string("_pin"));
	current_pin->S = { 25, 25, 25 };
	current_guide->AddNode(position);
}

void GuideLineEditor::DrawGuideLine()
{
	if (current_guide == nullptr || current_mark == nullptr)
		return;

	current_mark->Frame();
	current_mark->Render();
}

void GuideLineEditor::FloydWarshall()
{
	if (current_guide == nullptr)
		return;

	UINT rout_index = 0;
	for (const auto& start : current_guide->line_nodes)
	{
		for (const auto& dest : current_guide->line_nodes)
		{
			if (XMVector3Equal(start.second, dest.second))
				continue;

			FloydRout new_rout;
			new_rout.start_index = start.first;
			new_rout.dest_index = dest.first;
			new_rout.start = start.second;
			new_rout.dest = dest.second;

			RayShape rout_ray(_XMFLOAT3(start.second), _XMFLOAT3(dest.second));
			rout_ray.start.y += 50;
			rout_ray.end.y += 50;


			auto callback = QUADTREE->Raycast(rout_ray);
			if (callback.success == false)
			{
				new_rout.distance = Distance(start.second, dest.second);
			}
			else
			{
				map<float, UINT> trans_nodes_;
				for (const auto& trans : current_guide->line_nodes)
				{
					if (trans.first == start.first || trans.first == dest.first)
						continue;

					RayShape start_trans_ray(_XMFLOAT3(start.second), _XMFLOAT3(trans.second));
					start_trans_ray.start.y += 50;
					start_trans_ray.end.y += 50;

					RayShape trans_dest_ray(_XMFLOAT3(trans.second), _XMFLOAT3(dest.second));
					trans_dest_ray.start.y += 50;
					trans_dest_ray.end.y += 50;

					auto start_trans_callback = QUADTREE->Raycast(start_trans_ray);
					auto trans_dest_callback = QUADTREE->Raycast(trans_dest_ray);

					if (start_trans_callback.success == false && trans_dest_callback.success == false)
					{
						float total_distance = 0;
						total_distance += Distance(start.second, trans.second);
						total_distance += Distance(trans.second, dest.second);

						trans_nodes_.insert(make_pair(total_distance, trans.first));
					}
				}

				if (trans_nodes_.empty() == false)
				{
					new_rout.distance = trans_nodes_.begin()->first;
					new_rout.trans_index = trans_nodes_.begin()->second;
					new_rout.trans = current_guide->line_nodes.at(new_rout.trans_index);
				}
			}

			floyd_routs_.insert(make_pair(rout_index++, new_rout));
		}
	}
}
void GuideLineEditor::SaveMapdat_FloydRout()
{
	FloydWarshall();

	FileTransfer out_mapdata("../../Contents/BinaryPackage/DND_FloydRout.mapdat", WRITE);

	UINT num_routs = floyd_routs_.size();
	out_mapdata.WriteBinary<UINT>(&num_routs, 1);

	for (const auto& rout : floyd_routs_)
	{
		float distance = rout.second.distance;
		UINT start_index = rout.second.start_index;
		UINT dest_index = rout.second.dest_index;
		UINT trans_index = rout.second.trans_index;
		XMVECTOR start = rout.second.start;
		XMVECTOR dest = rout.second.dest;
		XMVECTOR trans = rout.second.trans;

		out_mapdata.WriteBinary<float>(&distance, 1);
		out_mapdata.WriteBinary<UINT>(&start_index, 1);
		out_mapdata.WriteBinary<UINT>(&dest_index, 1);
		out_mapdata.WriteBinary<UINT>(&trans_index, 1);
		out_mapdata.WriteBinary<XMVECTOR>(&start, 1);
		out_mapdata.WriteBinary<XMVECTOR>(&dest, 1);
		out_mapdata.WriteBinary<XMVECTOR>(&trans, 1);
	}
}
void GuideLineEditor::Active()
{
	if (GUI->FindWidget<GuideLineEditor>("guideline") == nullptr)
	{
		GUI->AddWidget<GuideLineEditor>("guideline");
	}
	else
	{
		NOT(GUI->FindWidget<GuideLineEditor>("guideline")->open_);
	}
}

void GuideLineEditor::Update()
{
	if (current_mark == nullptr)
		return;


	if (DINPUT->GetMouseState(L_BUTTON) == KEY_HOLD)
	{
		current_pin->T = _XMFLOAT3(PICKING->current_point);
	}


	for (auto& pin : current_mark->instance_pool)
	{
		current_guide->line_nodes[pin.first] = XMLoadFloat3(&pin.second->T);
		//SetActorPos(custum_meshs[pin.first], pin.second->T);
		//SCENE_MGR->GetActor<Actor>(flame_effects[pin.first])->ApplyMovement(TransformT(pin.second->T));
	}

	current_guide->UpdateLines();
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
		ImGui::RadioButton("Spawn Point", &guide_type, 2);

		static char buffer[128] = { 0, };
		ImGui::InputText("##string", buffer, 128);
		ImGui::SameLine();

		if (ImGui::Button("Add New Guide Line"))
		{
			AddNewGuideLine(string(buffer), guide_type);
			ZeroMemory(buffer, sizeof(buffer));
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

	if (ImGui::Button("Svae Floyd Rout"))
	{
		SaveMapdat_FloydRout();
	}

	ImGui::Checkbox("View Pin Mesh", &view_pin_mesh);

	if (!current_name.empty())
	{
		if (current_pin)
		{
			ImGui::Text(string("Selected Pin : " + current_pin->index).c_str());
			ImGui::DragFloat("X : ", &current_pin->T.x, 1.f);
			ImGui::DragFloat("Y : ", &current_pin->T.y, 1.f);
			ImGui::DragFloat("Z : ", &current_pin->T.z, 1.f);
		}

		if (ImGui::TreeNode(current_name.c_str()))
		{
			for (auto node : node_marks.find(current_name)->second->instance_pool)
			{				
				if (ImGui::Selectable(node.second->instance_id.c_str()))
				{
					current_pin = current_mark->SelectInstance(node.second->index);
				}
			}

			ImGui::TreePop();
		}

		if (ImGui::Button("+"))
		{
			AddNewNode(XMVectorZero());
			//entt::entity ent = AddCustumMeshActor("Vaccine.stmesh", "StaticMeshVS.cso");
			//custum_meshs.push_back(ent);
			//entt::entity ent = EFFECT_MGR->SpawnEffect<reality::FX_Flame>(XMVectorZero(), XMQuaternionIdentity(), XMVectorSet(10.0f, 10.0f, 10.0f, 0.0f));
			//flame_effects.push_back(ent);
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

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImRect rect = window->Rect();

	if (ImGui::IsMouseHoveringRect(rect.Min, rect.Max))
	{
		DINPUT->Active(false);
	}
	else
	{
		DINPUT->Active(true);
	}

	if (view_pin_mesh)
		DrawGuideLine();

	ImGui::End();
}

entt::entity GuideLineEditor::AddCustumMeshActor(string mesh_id, string vs_id)
{
	entt::entity ent = SCENE_MGR->GetRegistry().create();

	reality::C_StaticMesh stm;
	stm.local = XMMatrixIdentity();
	stm.world = XMMatrixIdentity();
	stm.static_mesh_id = mesh_id;
	stm.vertex_shader_id = vs_id;

	SCENE_MGR->GetRegistry().emplace_or_replace<reality::C_StaticMesh>(ent, stm);
	return ent;
}

void GuideLineEditor::SetActorPos(entt::entity ent, XMFLOAT3 position)
{
	C_StaticMesh* c_stmesh = SCENE_MGR->GetRegistry().try_get<C_StaticMesh>(ent);
	if (c_stmesh == nullptr)
		return;

	c_stmesh->world = TransformT(position);
}
