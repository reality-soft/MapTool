#include "ResourceSelector.h"
#include "Widgets.h"
#include "ResourceMgr.h"


void ResourceSelector::Active()
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

void ResourceSelector::Init()
{
    res_id_map = KGCA41B::RESOURCE->GetTotalResID();
}

void ResourceSelector::Release()
{
}

void ResourceSelector::Update()
{

}

void ResourceSelector::Render()
{
    ImGui::Begin("ResourceViewer", &open_);
    {
        ImGui::SetWindowSize(ImVec2(500, 500));
    }
    ImGui::PushStyleColor(ImGuiCol_FrameBg, LISTBOX_COLOR);
    ImGui::BeginListBox("##", ImVec2(300, 600));
    for (auto res_id : res_id_map)
    {
        //ImGui::SameLine();

        if (res_id.second == "STM")
        {
            ImGui::PushStyleColor(ImGuiCol_Text, STM_COLOR);
        }
        if (res_id.second == "SKM")
        {
            ImGui::PushStyleColor(ImGuiCol_Text, SKM_COLOR);
        }
        if (res_id.second == "SKT")
        {
            ImGui::PushStyleColor(ImGuiCol_Text, SKT_COLOR);
        }
        if (res_id.second == "ANM")
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ANM_COLOR);
        }
        if (res_id.second == "VS")
        {
            ImGui::PushStyleColor(ImGuiCol_Text, VS_COLOR);
        }
        if (res_id.second == "PS")
        {
            ImGui::PushStyleColor(ImGuiCol_Text, PS_COLOR);
        }
        if (res_id.second == "TEX")
        {
            ImGui::PushStyleColor(ImGuiCol_Text, TEX_COLOR);
        }
        if (res_id.second == "SOUND")
        {
            ImGui::PushStyleColor(ImGuiCol_Text, SOUND_COLOR);
        }

        bool selected = false;
        ImGui::Selectable(("[" + res_id.second + "] " + res_id.first).c_str(), &selected, 0, ImVec2(300, 30));
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("RESOURCE", res_id.first.c_str(), 128, ImGuiCond_Once);
            ImGui::Text(res_id.first.c_str());
            ImGui::EndDragDropSource();
        }
        ImGui::PopStyleColor();
    }
    ImGui::EndListBox();
    ImGui::PopStyleColor();

    ImGui::End();
}
