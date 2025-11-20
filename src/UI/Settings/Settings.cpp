#include "Settings.h"
#include <Common/Globals.h>
#include <Common/Utils.h>
#include <Data/MapData.h>
#include <Data/SettingsManager.h>
#include <imgui/imgui.h>

void OnOptionsRender()
{
    UIState::IsOptionsPaneOpen = true;

    ///----------------------------------------------------------------------------------------------------
    /// buff types to show
    ///----------------------------------------------------------------------------------------------------

    ImGui::Text("Buff reminders");
    ShownBuffTypes shownBuffs = SettingsManager::GetShownBuffTypes();

    if (ImGui::Checkbox("Enhancement", &shownBuffs.utility))
    {
        SettingsManager::SetShownBuffTypes(shownBuffs);
    }

    if (ImGui::Checkbox("Sigil", &shownBuffs.sigil))
    {
        SettingsManager::SetShownBuffTypes(shownBuffs);
    }

    if (ImGui::Checkbox("Default buffs", &shownBuffs.defaultBuffs))
    {
        SettingsManager::SetShownBuffTypes(shownBuffs);
    }

    ImGui::SameLine();
    ImGui::TextDisabled("(i)");
    ImGui::HoverTooltip("Default buffs are shown when no special buffs are available");

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ///----------------------------------------------------------------------------------------------------
    /// compact mode
    ///----------------------------------------------------------------------------------------------------

    bool compactMode = SettingsManager::IsCompactMode();
    if (ImGui::Checkbox("Hide buff names", &compactMode))
    {
        SettingsManager::SetCompactMode(compactMode);
    }

    if (compactMode)
    {
        ImGui::Indent();

        // tooltips
        bool isTooltipsEnabled = SettingsManager::IsTooltipsEnabled();
        if (ImGui::Checkbox("Show name in tooltip on mouse over", &isTooltipsEnabled))
        {
            SettingsManager::SetTooltipsEnabled(isTooltipsEnabled);
        }

        ImGui::SameLine();
        ImGui::TextDisabled("(i)");
        ImGui::HoverTooltip("Note: Mouse events aren't passed through to the game if enabled");

        // horizontal mode
        bool horizontalMode = SettingsManager::IsHorizontalMode();
        if (ImGui::Checkbox("Align icons horizontally", &horizontalMode))
        {
            SettingsManager::SetHorizontalMode(horizontalMode);
        }

        ImGui::Unindent();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ///----------------------------------------------------------------------------------------------------
    /// buff icon size
    ///----------------------------------------------------------------------------------------------------

    ImGui::Text("Buff icon size");
    int imageSize = SettingsManager::GetImageSize();
    if (ImGui::SliderInt("Pixel", &imageSize, 24, 64))
    {
        SettingsManager::SetImageSize(imageSize);
    }

    ///----------------------------------------------------------------------------------------------------
    /// buff reminder flash animation duration
    ///----------------------------------------------------------------------------------------------------

    ImGui::Text("Buff reminder flash animation duration");
    int flashingDuration = SettingsManager::GetFlashingDuration();
    if (ImGui::SliderInt("Animation seconds", &flashingDuration, 0, 60))
    {
        SettingsManager::SetFlashingDuration(flashingDuration);
    }

    ImGui::Spacing();

    ///----------------------------------------------------------------------------------------------------
    /// overlay position
    ///----------------------------------------------------------------------------------------------------

    ImGui::Text("Reminder position");
    ImGui::SameLine();
    ImGui::TextDisabled("(dragging is also supported)");

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

    // Reset settings
    if (ImGui::Button("Reset settings to default"))
    {
        SettingsManager::ResetSettings();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ///----------------------------------------------------------------------------------------------------
    /// debug
    ///----------------------------------------------------------------------------------------------------

    bool debugWindowEnabled = SettingsManager::IsDebugWindowEnabled();
    if (ImGui::Checkbox("Enable debug window", &debugWindowEnabled))
    {
        SettingsManager::SetDebugWindowEnabled(debugWindowEnabled);
    }
}
