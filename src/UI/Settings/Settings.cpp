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
    /// buff types and map types
    ///----------------------------------------------------------------------------------------------------

    ImGui::Text("Pick the instance types and buffs you want reminders for");

    if (ImGui::BeginTable("BuffReminderTable", 3, ImGuiTableFlags_SizingFixedFit))
    {
        ImGui::TableSetupColumn("Left");
        ImGui::TableSetupColumn("Spacer", ImGuiTableColumnFlags_WidthFixed, 50.0f);
        ImGui::TableSetupColumn("Right");

        ImGui::TableNextRow();

        // DUNGEONS (left column)
        ImGui::TableSetColumnIndex(0);
        Reminders reminders = SettingsManager::GetReminders();

        if (ImGui::Checkbox("Dungeons", &reminders.dungeons.enabled))
            SettingsManager::SetReminders(reminders);

        if (reminders.dungeons.enabled)
        {
            ImGui::Indent();
            if (ImGui::Checkbox("Enhancement##dungeons", &reminders.dungeons.utility))
                SettingsManager::SetReminders(reminders);
            if (ImGui::Checkbox("Night Sigil##dungeons", &reminders.dungeons.sigil))
                SettingsManager::SetReminders(reminders);
            ImGui::HoverTooltip("Also reminds when you should NOT use it");
            if (ImGui::Checkbox("Slaying Sigil##dungeons", &reminders.dungeons.sigilSlaying))
                SettingsManager::SetReminders(reminders);
            ImGui::Spacing();
            if (ImGui::Checkbox("Default buffs##dungeons", &reminders.dungeons.defaultBuffs))
                SettingsManager::SetReminders(reminders);
            ImGui::HoverTooltip("Shown when you should use your build’s default buff");
            ImGui::Unindent();
        }
        else
        {
            ImGui::SameLine();
            ImGui::TextDisabled(":(");
        }

        // FRACTALS (right column)
        ImGui::TableSetColumnIndex(2); // skip spacer

        if (ImGui::Checkbox("Fractals", &reminders.fractals.enabled))
            SettingsManager::SetReminders(reminders);
        ImGui::SameLine();
        ImGui::TextDisabled("(CMs only)");

        if (reminders.fractals.enabled)
        {
            ImGui::Indent();
            if (ImGui::Checkbox("Enhancement##fractals", &reminders.fractals.utility))
                SettingsManager::SetReminders(reminders);
            if (ImGui::Checkbox("Night Sigil##fractals", &reminders.fractals.sigil))
                SettingsManager::SetReminders(reminders);
            ImGui::HoverTooltip("Also reminds when you should NOT use it");
            if (ImGui::Checkbox("Slaying Sigil##fractals", &reminders.fractals.sigilSlaying))
                SettingsManager::SetReminders(reminders);
            ImGui::Spacing();
            if (ImGui::Checkbox("Default buffs##fractals", &reminders.fractals.defaultBuffs))
                SettingsManager::SetReminders(reminders);
            ImGui::HoverTooltip("Shown when you should use your build’s default buff");
            ImGui::Unindent();
        }

        ImGui::EndTable();
    }

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
        ImGui::HoverTooltip("Mouse events aren't passed through to the game if enabled");

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

    ImGui::Text("Buff animation duration");
    int flashingDuration = SettingsManager::GetFlashingDuration();
    if (ImGui::SliderInt("Seconds", &flashingDuration, 0, 60))
    {
        SettingsManager::SetFlashingDuration(flashingDuration);
    }

    ImGui::Spacing();

    ///----------------------------------------------------------------------------------------------------
    /// overlay position
    ///----------------------------------------------------------------------------------------------------

    ImGui::Text("Reminder position");
    ImGui::SameLine();
    ImGui::TextDisabled("(can also be dragged)");

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
