#include "Settings.h"
#include <Data/SettingsManager.h>
#include <imgui/imgui.h>

void OnOptionsRender()
{
    // Overlay drag enable checkbox
    bool dragEnabled = SettingsManager::IsOverlayDragEnabled();
    if (ImGui::Checkbox("Change overlay position", &dragEnabled))
    {
        SettingsManager::SetOverlayDragEnabled(dragEnabled);
    }

    if (dragEnabled)
    {
        ImGui::TextDisabled("You can also drag and drop");

        ImVec2 pos = SettingsManager::GetOverlayPosition();
        if (ImGui::InputFloat("Overlay position X", &pos.x, 1.0f, 10.0f, "%.1f"))
        {
            SettingsManager::SetPreciseOverlayPosition(pos);
        }

        if (ImGui::InputFloat("Overlay position Y", &pos.y, 1.0f, 10.0f, "%.1f"))
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
