#include "Widgets.h"

using namespace KGCA41B;

#define RGB_TO_FLOAT(x, y, z) ImVec4(x / 255.f, y / 255.f, z / 255.f, 1.f)
#define STM_COLOR RGB_TO_FLOAT(93, 155, 155)
#define SKM_COLOR RGB_TO_FLOAT(73, 126, 118)
#define SKT_COLOR RGB_TO_FLOAT(179, 40, 33)
#define ANM_COLOR RGB_TO_FLOAT(244, 70, 017)
#define VS_COLOR  RGB_TO_FLOAT(201, 60, 32)
#define PS_COLOR  RGB_TO_FLOAT(222, 76, 138)
#define TEX_COLOR RGB_TO_FLOAT(63, 136, 143)
#define SOUND_COLOR RGB_TO_FLOAT(151, 79, 102)

void GwMainMenu::Update()
{
    msg_ = MsgType::NONE;
	ImGui::SetCurrentContext(GUI->GetContext());
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}

void GwMainMenu::Render()
{
    ImGui::BeginMainMenuBar();
    {
        if (ImGui::MenuItem("ResourceVeiwer"))
        {
            msg_ = MsgType::OW_RES_VIEWER;
        }
    }
    ImGui::EndMainMenuBar();
}

void GwResViewer::Update()
{
    if (res_id_map.empty())
    {
        res_id_map = RESOURCE->GetTotalResID();
    }
}

void GwResViewer::Render()
{
    ImGui::Begin("ResourceViewer", &open_);
    {
        ImGui::SetWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
    }

    int i = 0;
    for (auto res_id : res_id_map)
    {
        ImGui::SameLine();

        if (res_id.second == "STM")
        {
            ImGui::PushStyleColor(ImGuiCol_Button, STM_COLOR);
        }
        if (res_id.second == "SKM")
        {
            ImGui::PushStyleColor(ImGuiCol_Button, SKM_COLOR);
        }
        if (res_id.second == "SKT")
        {
            ImGui::PushStyleColor(ImGuiCol_Button, SKT_COLOR);
        }
        if (res_id.second == "ANM")
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ANM_COLOR);
        }
        if (res_id.second == "VS")
        {
            ImGui::PushStyleColor(ImGuiCol_Button, VS_COLOR);
        }
        if (res_id.second == "PS")
        {
            ImGui::PushStyleColor(ImGuiCol_Button, PS_COLOR);
        }
        if (res_id.second == "TEX")
        {
            ImGui::PushStyleColor(ImGuiCol_Button, TEX_COLOR);
        }
        if (res_id.second == "SOUND")
        {
            ImGui::PushStyleColor(ImGuiCol_Button, SOUND_COLOR);
        }

        ImGui::SameLine();
        ImGui::BeginGroup();
        {
            ImGui::ButtonEx(res_id.second.c_str(), ImVec2(100, 100));
            ImGui::PopStyleColor();
            ImGui::Text(res_id.first.c_str());
        }
        ImGui::EndGroup();
        if ((int)ImGui::GetWindowWidth() > 100)
        {
            if ((i + 1) % ((int)ImGui::GetWindowWidth() / 100) == 0)
            {
                ImGui::NewLine();
            }
        }

        ++i;
    }
    ImGui::End();
}

void GwPorperty::Update()
{
    POINT cursor_pos;
    GetCursorPos(&cursor_pos);
    ScreenToClient(ENGINE->GetWindowHandle(), &cursor_pos);

    mouse_pos_text =  "\n[Mouse Screen Pos]\n";
    mouse_pos_text += "\nX : " + to_string(cursor_pos.x);
    mouse_pos_text += "\nY : " + to_string(cursor_pos.y);
    mouse_pos_text += "\nNDC X : " + to_string(ndc_pos.x);
    mouse_pos_text += "\nNDC Y : " + to_string(ndc_pos.y);


    camera_pos_text = "\n[Camera Pos]\n";
    camera_pos_text += "\nX : " + to_string(camera_pos.m128_f32[0]);
    camera_pos_text += "\nY : " + to_string(camera_pos.m128_f32[1]);
    camera_pos_text += "\nZ : " + to_string(camera_pos.m128_f32[2]);

    ray_hitpoint_text_  = "\n[Ray Hit Point\n]";
    ray_hitpoint_text_ += "\nX : " + to_string(ray_hitpoint_.m128_f32[0]);
    ray_hitpoint_text_ += "\nY : " + to_string(ray_hitpoint_.m128_f32[1]);
    ray_hitpoint_text_ += "\nZ : " + to_string(ray_hitpoint_.m128_f32[2]);

}

void GwPorperty::Render()
{
    ImGui::Begin("Porperty");
    {
        ImGui::SetWindowPos(ImVec2(300, 300), ImGuiCond_FirstUseEver);

        ImGui::Text(camera_pos_text.c_str());
        ImGui::Text(mouse_pos_text.c_str());
        ImGui::Text(ray_hitpoint_text_.c_str());
    }
    ImGui::End();
}
