#include "Settings.h"
#include <Data/SettingsManager.h>
#include <imgui/imgui.h>

void OnOptionsRender()
{
    // Overlay drag enable checkbox
    bool dragEnabled = SettingsManager::IsOverlayDragEnabled();
    if (ImGui::Checkbox("Enable overlay drag", &dragEnabled))
    {
        SettingsManager::SetOverlayDragEnabled(dragEnabled);
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(toggle whether the overlay can be moved)");

    // currently buggy if dragging is enabled
    /*
    ImGui::NextColumn();

    ImVec2 pos = SettingsManager::GetOverlayPosition();
    if (ImGui::InputFloat("Overlay position X", &pos.x, 1.0f, 10.0f, "%.1f"))
    {
        SettingsManager::SetOverlayPosition(pos);
    }

    if (ImGui::InputFloat("Overlay position Y", &pos.y, 1.0f, 10.0f, "%.1f"))
    {
        SettingsManager::SetOverlayPosition(pos);
    }
    */

    ImGui::Spacing();

    // Overlay timeout slider
    ImGui::Text("Overlay visibility timeout in seconds");
    int timeout = SettingsManager::GetOverlayTimeoutSeconds();
    if (ImGui::SliderInt("", &timeout, 1, 60))
    {
        SettingsManager::SetOverlayTimeoutSeconds(timeout);
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Ctrl + Click to manually enter a value");
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Reset overlay position button
    if (ImGui::Button("Reset overlay position"))
    {
        SettingsManager::SetOverlayDragEnabled(false); // TODO remove once setting pos is fixed
        SettingsManager::ResetOverlayPosition();
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Restore the overlay to its default position");
    }
}
