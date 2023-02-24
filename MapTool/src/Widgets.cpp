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
        if (ImGui::MenuItem("Instanced Foliage"))
        {
            msg_ = MsgType::OW_INSTANCED_FOLIAGE;
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
    editing_level->LevelEdit();
}

void GwLevelEditor::Render()
{
    ImGui::Begin("Level Editor", &open_);
    {
        ImGui::SetWindowPos(ImVec2(200, 400), ImGuiCond_FirstUseEver);

        ImGui::Text("Resize Map");
        {
            ImGui::SliderInt("Cell Count", &cell_count_, 4, 1024, (to_string(cell_count_) + " X " + to_string(cell_count_)).c_str());
            ImGui::InputInt("Cell Distance", &cell_distance_);
            ImGui::InputInt("UV Scale", &uv_scale_);

            if (ImGui::Button("Generate"))
            {
                editing_level->Regenerate((UINT)cell_count_ - 1, (UINT)cell_count_ - 1, cell_distance_, uv_scale_);

            }

        } ImGui::Separator(); ImGui::Spacing();

        ImGui::Text("Change Texture");
        {
            ImTextureID texid = (ImTextureID)RESOURCE->UseResource<Texture>(editing_level->texture_id[0])->srv.Get();
            ImGui::Image(texid, ImVec2(100, 100));

            static char buffer[255] = { 0, };
            ImGui::InputText("##", buffer, 255);
            if (ImGui::Button("Set Resource ID"))
            {
                editing_level->texture_id[0] = buffer;
            }

        } ImGui::Separator(); ImGui::Spacing();


        ImGui::Text("Map Sculpting");
        {
            if (ImGui::Checkbox("Brush On", &brush_on_)) {}

            if (ImGui::SliderFloat("Brush Size", &brush_size_, 1, 300))
                editing_level->sculpting_brush_ = brush_size_;

            if (ImGui::Button("Save Heightmap Data"))
            {
                editing_level->ResetHeightField();
            }
        
        } ImGui::Separator(); ImGui::Spacing();

        ImGui::Text("Export Level");
        {
            static char buffer1[255] = { 0, };
            ImGui::InputText("##", buffer1, 255);

            if (ImGui::Button("Export"))
            {
                editing_level->ExportToFile(buffer1);
            }
            if (ImGui::Button("Import"))
            {
                Level level;
                level.ImportFromFile(editing_level->export_dir + "TestWorld.jlevel");
            }

        } ImGui::Separator(); ImGui::Spacing();
    }
    ImGui::End();
}

void GwLevelEditor::Gernate(Level* new_level)
{

}
