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
