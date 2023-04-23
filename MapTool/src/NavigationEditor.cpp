#include "Widgets.h"
#include "NavigationEditor.h"
#include "PickingMgr.h"

void NavigationEditor::Active()
{
	if (GUI->FindWidget<NavigationEditor>("navigation") == nullptr)
	{
		GUI->AddWidget<NavigationEditor>("navigation");
	}
	else
	{
		NOT(GUI->FindWidget<NavigationEditor>("navigation")->open_);
	}
}

void NavigationEditor::Update()
{
	if (initialized == false)
	{
		A = XMVectorZero();
		BA = XMVectorZero();
		AB = XMVectorZero();
		B = XMVectorZero();

		node_pins_.Init("SkySphere.stmesh", "InstancingVS.cso", "NpcGuide.mat");

		node_pins_.AddNewInstance("A")->S = { 25, 25, 25 };
		node_pins_.AddNewInstance("BA")->S = { 25, 25, 25 };
		node_pins_.AddNewInstance("AB")->S = { 25, 25, 25 };
		node_pins_.AddNewInstance("B")->S = { 25, 25, 25 };

		initialized = true;
	}

	if (DINPUT->GetMouseState(L_BUTTON) == KEY_HOLD)
	{
		if (selected_A_or_B == 0)
		{
			A = PICKING->current_point;
		}
		if (selected_A_or_B == 1)
		{
			B = PICKING->current_point;
		}
	}
	AB.m128_f32[0] = A.m128_f32[0];
	AB.m128_f32[2] = B.m128_f32[2];
	BA.m128_f32[0] = B.m128_f32[0];
	BA.m128_f32[2] = A.m128_f32[2];

	for (auto& pin : node_pins_.instance_pool)
	{
		if (pin.second->instance_id == "A")
			pin.second->T = _XMFLOAT3(A);

		if (pin.second->instance_id == "BA")
			pin.second->T = _XMFLOAT3(BA);

		if (pin.second->instance_id == "AB")
			pin.second->T = _XMFLOAT3(AB);

		if (pin.second->instance_id == "B")
			pin.second->T = _XMFLOAT3(B);
	}

	float row_length = Vector3Length(A - BA);
	float col_length = Vector3Length(A - AB);
	float row_unit_length = row_length / (row_node_filling_ + 1);
	float col_unit_length = col_length / (col_node_filling_ + 1);

	UINT index = 0;
	for (int c = 0; c < col_node_filling_ + 2; ++c)
	{
		for (int r = 0; r < row_node_filling_ + 2; ++r)
		{
			InstanceData* pin = nullptr;
			for (auto& p : node_pins_.instance_pool)
			{
				if (p.second->instance_id == "filling_" + to_string(c) + to_string(r))
				{
					pin = p.second;
					break;
				}
			}

			if (pin == nullptr)
				continue;

			XMVECTOR row_coord = A + XMVectorSet(1, 0,  0, 0) * row_unit_length * r;
			XMVECTOR col_coord = A + XMVectorSet(0, 0, -1, 0) * col_unit_length * c;
			pin->T.x = row_coord.m128_f32[0];
			pin->T.z = col_coord.m128_f32[2];

			total_nodes_.insert(make_pair(index++, _XMVECTOR3(pin->T)));
		}
	}
}

void NavigationEditor::Render()
{
	ImGui::Begin("Navigation Editor", &open_);
	{
		ImGui::RadioButton("Select A Node", &selected_A_or_B, 0);
		ImGui::RadioButton("Select B Node", &selected_A_or_B, 1);

		ImGui::InputInt("Row Node Filling", &row_node_filling_);
		ImGui::InputInt("Col Node Filling", &col_node_filling_);

		if (ImGui::Button("Generate"))
		{
			for (int c = 0; c < col_node_filling_ + 2; ++c)
			{
				for (int r = 0; r < row_node_filling_ + 2; ++r)
				{
					node_pins_.AddNewInstance("filling_" + to_string(c) + to_string(r));
					node_pins_.selected_instance->S = { 25, 25, 25 };
				}
			}
		}

		if (ImGui::Button("Calculate Floyd Warshall"))
		{
			FloydWarshall();
		}
	}
	ImGui::End();

 	node_pins_.Frame();
	node_pins_.Render();
}

void NavigationEditor::FloydWarshall()
{
	UINT rout_index = 0;
	for (const auto& start : total_nodes_)
	{
		for (const auto& dest : total_nodes_)
		{
			XMVECTOR start_point = start.second;
			XMVECTOR dest_point = dest.second;

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
				for (const auto& trans : total_nodes_)
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
					new_rout.trans = total_nodes_.at(new_rout.trans_index);
				}
			}
			
			floyd_routs_.insert(make_pair(rout_index++, new_rout));
		}
	}
}
