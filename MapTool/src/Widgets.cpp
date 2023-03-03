#include "Widgets.h"
#include "PickingMgr.h"

using namespace KGCA41B;

void GwMainMenu::Update()
{
    msg_ = MsgType::NONE;
	ImGui::SetCurrentContext(GUI->GetContext());
}

void GwMainMenu::Render()
{
    ImGui::BeginMainMenuBar();
    {
        if (ImGui::MenuItem("Create Map"))
        {
            msg_ = MsgType::OW_LEVEL_EDITOR;
        }

        if (ImGui::MenuItem("Resource Veiwer"))
        {
            msg_ = MsgType::OW_RES_VIEWER;
        }
        if (ImGui::BeginMenu("Render Options"))
        {
            if (ImGui::MenuItem("WireFrame / Solid"))
            {
                msg_ = MsgType::OPT_WIREFRAME;
            }
            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

}

void GwPorperty::Update()
{
    POINT cursor_pos;
    GetCursorPos(&cursor_pos);
    ScreenToClient(ENGINE->GetWindowHandle(), &cursor_pos);

    mouse_pos_text =  "[FPS] : " + to_string(TM_FPS);
}

void GwPorperty::Render()
{
    ImGui::Begin("Porperty");
    {
        ImGui::SetWindowPos(ImVec2(300, 300), ImGuiCond_FirstUseEver);

        ImGui::Text(mouse_pos_text.c_str());
        ImGui::Text("Picking Point");
        ImGui::Text((to_string(PICKING->current_point.m128_f32[0]).c_str()));
        ImGui::Text((to_string(PICKING->current_point.m128_f32[1]).c_str()));
        ImGui::Text((to_string(PICKING->current_point.m128_f32[2]).c_str()));
    }
    ImGui::End();
}

void GwLevelEditor::Update()
{
    if (editing_level == nullptr)
        return;

    editing_level->LevelEdit();
}

void GwLevelEditor::Render()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, RGB_TO_FLOAT(0, 42, 38, 1));
    CwEditTerrain();
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, RGB_TO_FLOAT(32, 23, 36, 1));
    CwObjectControl();
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, RGB_TO_FLOAT(35, 15, 21, 1));
    CwSaveLoad();
    ImGui::PopStyleColor();
}

void GwLevelEditor::CwSaveLoad()
{
    ImGui::Begin("Save Load");  
    {
        ImGui::InputText("Directory", dir_buffer, 128);

        ImGui::InputText("##save", save_buffer, 128);
        ImGui::SameLine();
        if (ImGui::Button("Save"))
        {
            editing_level->ExportToFile(string(dir_buffer) + string(save_buffer));
        }

        ImGui::InputText("##load", load_buffer, 128);
        ImGui::SameLine();
        if (ImGui::Button("Load"))
        {
            loaded_level = new Level;
            loaded_level->ImportFromFile(string(dir_buffer) + string(load_buffer));
            QUADTREE->Init(loaded_level);  
        }

    }
    ImGui::End();
}

void GwLevelEditor::CwEditTerrain()
{
    ImGui::Begin("Edit Terrain");
    {
        ImGui::Text("Select Brush");
        {
            if (ImGui::Checkbox("Brush On", &brush_on_)) {}

            static int brush = 0;
            ImGui::RadioButton("Sculpting", &brush, 0);
            ImGui::RadioButton("Texturing", &brush, 1);
            ImGui::RadioButton("Selecting", &brush, 2);
            ImGui::SliderFloat("Brush Size", &editing_level->brush_scale, 1, 300);

            editing_level->brush_type = (BrushType)brush;

        } ImGui::Separator(); ImGui::Spacing();

        ImGui::Text("Resize Map");
        {
            ImGui::InputInt("Max LOD", &max_lod);
            ImGui::InputInt("Cell Scale", &cell_scale);
            ImGui::InputInt("UV Scale", &uv_scale_);
            ImGui::InputInt("Blocks", &row_col_blocks);

        } ImGui::Separator(); ImGui::Spacing();

        ImGui::Text("Texture Layer");
        {
            if (ImGui::BeginListBox("##"))
            {
                int index = 0;
                for (auto texid : editing_level->texture_id)
                {
                    if (ImGui::Selectable(texid.c_str()))
                    {
                        strcpy(tex_id_buffer, texid.c_str());
                        editing_level->current_layer = index;
                    }
                    index++;
                }

                ImGui::EndListBox();
            }

            ImTextureID texid = nullptr;
            auto texture = RESOURCE->UseResource<Texture>(tex_id_buffer);
            if (texture)
            {
                texid = (ImTextureID)texture->srv.Get();
            }
            else
            {
                strcpy(tex_id_buffer, editing_level->texture_id[0].c_str());
            }

            ImGui::Image(texid, ImVec2(100, 100));            
            ImGui::InputText("Texture ID", tex_id_buffer, 255);
            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE");
                if (payload != nullptr)
                {
                    char* resid = (char*)payload->Data;
                    strcpy(tex_id_buffer, resid);
                }
                ImGui::EndDragDropTarget();
            }
            if (ImGui::Button("Add Texture Layer"))
            {
                editing_level->texture_id.push_back(tex_id_buffer);
                editing_level->SetTexturesToLayer();
            }

        } ImGui::Separator(); ImGui::Spacing();
    }
    ImGui::End();
}

void GwLevelEditor::CwObjectControl()
{
    ImGui::Begin("Object Control");
    {
        ImGui::Text("Create Instance Object");
        {
            if (ImGui::BeginListBox("##"))
            {
                for (auto& inst_obj : editing_level->inst_objects)  
                {
                    if (ImGui::Selectable(inst_obj.object_name.c_str()))
                    {
                        inst_obj.AddNewInstance();
                        selected_instance = inst_obj.selected_instance;
                    }
                }

                ImGui::EndListBox();
            }

            ImGui::InputText("Mesh", mesh_id_buffer, sizeof(mesh_id_buffer));
            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE");
                if (payload != nullptr)
                {
                    char* resid = (char*)payload->Data;  
                    strcpy(mesh_id_buffer, resid);
                }
                ImGui::EndDragDropTarget();
            }

            if (ImGui::Button("Create Object Instanced"))  
            {
                InstancedObject new_obj;
                new_obj.Init(mesh_id_buffer, "InstancingVS.cso");
                editing_level->inst_objects.push_back(new_obj);
            }


        } ImGui::Separator(); ImGui::Spacing();

        ImGui::Text("Transform Selected Instance");
        {
            if (selected_instance)  
            {
                ImGui::Text((selected_instance->GetName() + " : Scale").c_str());
 
                ImGui::DragFloat("Scale XYZ", &selected_instance->S.x, 0.1f);
                selected_instance->S.y = selected_instance->S.x;
                selected_instance->S.z = selected_instance->S.x; 
                ImGui::Spacing();

                ImGui::Text((selected_instance->GetName() + " : Rotation").c_str());
                ImGui::DragFloat("Rotation X", &selected_instance->R.x, 0.1f);
                ImGui::DragFloat("Rotation Y", &selected_instance->R.y, 0.1f);
                ImGui::DragFloat("Rotation Z", &selected_instance->R.z, 0.1f);
                ImGui::Spacing();

                ImGui::Text((selected_instance->GetName() + " : Transform").c_str());
                ImGui::DragFloat("Transform X", &selected_instance->T.x, 0.1f);
                ImGui::DragFloat("Transform Y", &selected_instance->T.y, 0.1f);
                ImGui::DragFloat("Transform Z", &selected_instance->T.z, 0.1f);
                ImGui::Spacing();
            }

        } ImGui::Separator(); ImGui::Spacing();

        ImVec2 win_min = ImGui::GetWindowPos();
        ImVec2 win_max = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, 
                                ImGui::GetWindowPos().y + ImGui::GetWindowSize().y);
        
        if (ImGui::IsMouseHoveringRect(win_min, win_max))
        {
            DINPUT->Active(false);
        }
        else
        {
            DINPUT->Active(true);
        }
    }
    ImGui::End();
}
