#include "Settings.h"
#include <Common/Globals.h>
#include <Common/Utils.h>
#include <Data/SettingsManager.h>
#include <imgui/imgui.h>

void OnOptionsRender()
{
    G::IsOptionsPaneOpen = true;
    SettingsManager::SetOverlayDragEnabled(true);

    // Overlay positioning
    ImGui::Text("Change overlay position");

    ImVec2 pos = SettingsManager::GetOverlayPosition();
    if (ImGui::InputFloat("Position X", &pos.x, 1.0f, 10.0f, "%.1f"))
    {
        SettingsManager::SetPreciseOverlayPosition(pos);
    }

    if (ImGui::InputFloat("Position Y", &pos.y, 1.0f, 10.0f, "%.1f"))
    {
        SettingsManager::SetPreciseOverlayPosition(pos);
    }

    ImGui::Spacing();

    // Reset overlay position button
    if (ImGui::Button("Reset overlay position"))
    {
        SettingsManager::ResetOverlayPosition();
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Restore the overlay to its default position");
    }

    /*
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::NextColumn();

    // Overlay timeout slider
    ImGui::Text("Overlay visibility timeout (seconds)");
    int timeout = SettingsManager::GetOverlayTimeoutSeconds();
    if (ImGui::SliderInt("", &timeout, 1, 60))
    {
        SettingsManager::SetOverlayTimeoutSeconds(timeout);
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Ctrl + Click to manually enter a value");
    }
    */
}
