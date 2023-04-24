#include "Widgets.h"
#include "PickingMgr.h"

using namespace reality;

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
        if (ImGui::MenuItem("Guide Line"))
        {
            msg_ = MsgType::OW_GUIDE_LINE;
        }
        if (ImGui::MenuItem("Resource Veiwer"))
        {
            msg_ = MsgType::OW_RES_VIEWER;
        }
        if (ImGui::MenuItem("Navigation Editor"))
        {
            msg_ = MsgType::OW_NAVI_EDITOR;
        }
        if (ImGui::MenuItem("Sequence Editor"))
        {
            msg_ = MsgType::OW_SEQUENCE_EDITOR;
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
    fps = "[FPS] : " + to_string(TM_FPS);

    visible_leaf =
        "[visible nodes]\n" +
        to_string(visible_leaves) + "\n";

    collision =
        "[collision]\n" +
        string("x : ") + to_string(collide_pos.x) + "\n" +
        string("y : ") + to_string(collide_pos.y) + "\n" +
        string("z : ") + to_string(collide_pos.z) + "\n";

    camera_pos = 
        "[camera pos]\n" +
        string("X : ") + to_string(camera.x) + "\n" +
        string("Y : ") + to_string(camera.y) + "\n" +
        string("Z : ") + to_string(camera.z) + "\n";
    
}

void GwPorperty::Render()
{
    ImGui::Begin("Porperty");
    {
        ImGui::SetWindowPos(ImVec2(300, 300), ImGuiCond_FirstUseEver);

        ImGui::Text(fps.c_str());
        ImGui::Text(visible_leaf.c_str());
        ImGui::Text(collision.c_str());
        ImGui::Text(camera_pos.c_str());

        ImGui::Text("[current tri index]");
        ImGui::Text(to_string(PICKING->current_tri).c_str());


        //for (int i = 0; i < 6; ++i)
        //{
        //    ImTextureID im_tex;
        //    im_tex = (ImTextureID)(cube_shadow_->depth_srvs[i]);
        //    ImGui::Image(im_tex, ImVec2(200, 200));
        //}

        //ImGui::DragInt("DepthBias", &db, 1000);
        //ImGui::DragFloat("SlopeScaledDepthBias", &sdb, 0.1f);
        //ImGui::DragFloat("DepthBiasClamp", &clp, 1000);

        //CreateDepthBiasRS(db, sdb, clp, &cube_shadow_->depth_bias_rs_);
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
    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_RGB_TO_FLOAT(0, 42, 38, 1));
    CwEditTerrain();
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_RGB_TO_FLOAT(32, 23, 36, 1));
    CwObjectControl();
    ImGui::PopStyleColor();
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
            // Base Texture Selection

            ImTextureID texid = nullptr;
            auto texture = RESOURCE->UseResource<Texture>(editing_level->texture_id[0]);
            if (texture)
            {
                texid = (ImTextureID)texture->srv.Get();
                ImGui::Image(texid, ImVec2(100, 100));
            }
            ImGui::InputText("Base Texture", editing_level->texture_id[0].data(), editing_level->texture_id[0].size());
            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE");
                if (payload != nullptr)
                {
                    char* resid = (char*)payload->Data;
                    strcpy(tex_id_buffer, resid);

                    auto texture = RESOURCE->UseResource<Texture>(string(tex_id_buffer));
                    if (texture)
                    {
                        editing_level->texture_id[0] = string(tex_id_buffer);
                    }
                }
                ImGui::EndDragDropTarget();
            }

            // Tex Layers Selection
            if (ImGui::BeginTabBar("Layer Texture 0"))
            {
                if (ImGui::BeginTabItem("Channel R")) {
                    static string tex_id;
                    Texture* texture = DragDropImage(tex_id);
                    if (texture)
                    {
                        editing_level->texture_id[1] = tex_id;
                    }

                    editing_level->current_layer = 1;
                    ImGui::EndTabItem();  
                }
                if (ImGui::BeginTabItem("Channel G")) {
                    static string tex_id;
                    Texture* texture = DragDropImage(tex_id);
                    if (texture)
                    {
                        editing_level->texture_id[2] = tex_id;
                    }

                    editing_level->current_layer = 2;
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Channel B")) {
                    static string tex_id;
                    Texture* texture = DragDropImage(tex_id);
                    if (texture)
                    {
                        editing_level->texture_id[3] = tex_id;
                    }

                    editing_level->current_layer = 3;
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Channel A")) {
                    static string tex_id;
                    Texture* texture = DragDropImage(tex_id);
                    if (texture)
                    {
                        editing_level->texture_id[4] = tex_id;
                    }

                    editing_level->current_layer = 4;
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();  
            }

            if (ImGui::RadioButton("Paint", editing_level->paint_on))
            {
                editing_level->paint_on = true;
            }
            if (ImGui::RadioButton("Erase", !editing_level->paint_on))
            {
                editing_level->paint_on = false;
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
                    bool selected = ImGui::Selectable(inst_obj.object_name.c_str());
                    if (selected)
                    {
                        inst_obj.AddNewInstance(inst_obj.object_name);
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
                new_obj.Init(mesh_id_buffer, "InstancingVS.cso", "MeshSurfacePS.cso");
                editing_level->inst_objects.push_back(new_obj);
            }


        } ImGui::Separator(); ImGui::Spacing();

        ImGui::Text("Transform Selected Instance");
        {
            if (selected_instance)  
            {
                ImGui::Text((selected_instance->instance_id + " : Scale").c_str());
 
                ImGui::DragFloat("Scale XYZ", &selected_instance->S.x, 0.1f);
                selected_instance->S.y = selected_instance->S.x;
                selected_instance->S.z = selected_instance->S.x; 
                ImGui::Spacing();

                ImGui::Text((selected_instance->instance_id + " : Rotation").c_str());
                ImGui::DragFloat("Rotation X", &selected_instance->R.x, 0.1f);
                ImGui::DragFloat("Rotation Y", &selected_instance->R.y, 0.1f);
                ImGui::DragFloat("Rotation Z", &selected_instance->R.z, 0.1f);
                ImGui::Spacing();

                ImGui::Text((selected_instance->instance_id + " : Transform").c_str());
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

Texture* GwLevelEditor::DragDropImage(string& name)
{
    Texture* texture = nullptr;
    ImTextureID im_texid = nullptr;

    texture = RESOURCE->UseResource<Texture>(name);
    if (texture)
        im_texid = (ImTextureID)texture->srv.Get();

    ImGui::Image(im_texid, ImVec2(100, 100));
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE");
        if (payload != nullptr)
        {
            char* resid = (char*)payload->Data;
            name = string(resid);
            texture = RESOURCE->UseResource<Texture>(name);
            if (texture)
            {
                im_texid = (ImTextureID)texture->srv.Get();
            }
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::Text(name.c_str());

    return texture;
}
