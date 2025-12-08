#include "Settings.h"

#include <Common/Globals.h>
#include <Common/Utils.h>
#include <Data/SettingsManager.h>
#include <imgui/imgui.h>

namespace
{
    constexpr float SLIDER_WIDTH = 250.0f;
}

void OnOptionsRender()
{
    UIState::IsOptionsPaneOpen = true;

    ///----------------------------------------------------------------------------------------------------
    /// REMINDER TYPE SETTINGS
    ///----------------------------------------------------------------------------------------------------

    ImGui::Text("Pick the instance types and buffs you want reminders for");

    if (ImGui::BeginTable("BuffReminderTable", 3, ImGuiTableFlags_SizingFixedFit))
    {
        ImGui::TableSetupColumn("Left", ImGuiTableColumnFlags_WidthFixed, 200.0f);
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
            if (ImGui::Checkbox("Night Sigil##dungeons", &reminders.dungeons.nightSigil))
                SettingsManager::SetReminders(reminders);
            ImGuiUtil::InlineHelp("Also reminds when NOT to use it");
            if (ImGui::Checkbox("Slaying Sigil##dungeons", &reminders.dungeons.slayingSigil))
                SettingsManager::SetReminders(reminders);
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
            if (ImGui::Checkbox("Night Sigil##fractals", &reminders.fractals.nightSigil))
                SettingsManager::SetReminders(reminders);
            ImGuiUtil::InlineHelp("Also reminds when NOT to use it");
            if (ImGui::Checkbox("Slaying Sigil##fractals", &reminders.fractals.slayingSigil))
                SettingsManager::SetReminders(reminders);
            ImGui::Unindent();
        }

        ImGui::EndTable();
    }

    if (!WorldState::IsOnSupportedMap)
    {
        const char *msg =
            "You are on an unsupported map. All reminders are shown regardless of configuration.\n"
            "Enter a dungeon or fractal to see only your selected reminders.";

        ImGuiUtil::TextInBox(msg, ImVec4(0.8f, 0.1f, 0.1f, 0.15f), ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
    }

    /// ----------------------------------------------------------------------------------------------------
    /// REMINDER DISPLAY SETTINGS
    /// ----------------------------------------------------------------------------------------------------

    ImGui::Spacing();
    if (ImGui::CollapsingHeader("Reminder Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        bool compactMode = SettingsManager::IsCompactMode();
        if (ImGui::Checkbox("Hide buff names", &compactMode))
        {
            SettingsManager::SetCompactMode(compactMode);
        }

        if (compactMode)
        {
            ImGui::Indent();

            // TOOLTIPS
            bool isTooltipsEnabled = SettingsManager::IsTooltipsEnabled();
            if (ImGui::Checkbox("Show name in tooltip on mouse over", &isTooltipsEnabled))
            {
                SettingsManager::SetTooltipsEnabled(isTooltipsEnabled);
            }
            ImGuiUtil::InlineHelp("Mouse events aren't passed through to the game if enabled");

            // HORIZONTAL ALIGNMENT
            bool horizontalMode = SettingsManager::IsHorizontalMode();
            if (ImGui::Checkbox("Align icons horizontally", &horizontalMode))
            {
                SettingsManager::SetHorizontalMode(horizontalMode);
            }

            ImGui::Unindent();
        }

        // BUFF ICON / NAME RENDERING ORDER
        if (compactMode)
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.33f);

        bool isIconFirst = SettingsManager::IsIconFirst();
        if (ImGui::Checkbox("Put buff icon before name", &isIconFirst) && !compactMode)
            SettingsManager::SetIconFirst(isIconFirst);

        if (compactMode)
            ImGui::PopStyleVar();

        ImGuiUtil::InlineHelp("Controls the order of icon and name in the reminder overlay\nNote: No effect when names are hidden");

        // BUFF ICON SIZE
        int imageSize = SettingsManager::GetImageSize();
        ImGui::SetNextItemWidth(SLIDER_WIDTH);
        if (ImGui::SliderInt("##BuffIconSize", &imageSize, 20, 80, "%d px"))
        {
            SettingsManager::SetImageSize(imageSize);
        }
        ImGui::SameLine();
        ImGui::Text("Buff icon size");

        // BUFF FLASHING EFFECT DURATION
        int flashingDuration = SettingsManager::GetFlashingDuration();
        ImGui::SetNextItemWidth(SLIDER_WIDTH);
        if (ImGui::SliderInt("##BuffAnimationDuration", &flashingDuration, 0, 60, "%d seconds"))
        {
            SettingsManager::SetFlashingDuration(flashingDuration);
        }
        ImGui::SameLine();
        ImGui::Text("Flash effect duration");
    }

    /// ----------------------------------------------------------------------------------------------------
    /// OVERLAY POSITION SETTINGS
    /// ----------------------------------------------------------------------------------------------------

    ImGui::Spacing();
    if (ImGui::CollapsingHeader("Overlay Position", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static const char *pivotLabels[] = {"Top-Left", "Top-Right", "Bottom-Left", "Bottom-Right"};

        ImVec2 pos                       = SettingsManager::GetOverlayPosition();
        int current_window_anchor_index  = static_cast<int>(SettingsManager::GetWindowAnchor());
        int current_overlay_origin_index = static_cast<int>(SettingsManager::GetOverlayOrigin());
        bool anchorOriginSync            = SettingsManager::IsAnchorOriginSync();

        // SYNC CHECKBOX
        if (ImGui::Checkbox("Sync overlay origin with anchor", &anchorOriginSync))
        {
            current_overlay_origin_index = current_window_anchor_index;
            SettingsManager::SetOverlayOrigin(static_cast<Pivot>(current_overlay_origin_index));

            SettingsManager::SetAnchorOriginSync(anchorOriginSync);
        }
        ImGuiUtil::InlineHelp("Overlay anchor and origin change together for easy positioning\nUncheck to set them independently");

        if (current_overlay_origin_index != current_window_anchor_index)
            ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Warning: Overlay might go off-screen.");

        // OVERLAY ANCHOR POSITION
        ImGui::SetNextItemWidth(SLIDER_WIDTH);
        if (ImGui::Combo("Overlay anchor", &current_window_anchor_index, pivotLabels, IM_ARRAYSIZE(pivotLabels)))
        {
            if (anchorOriginSync)
            {
                current_overlay_origin_index = current_window_anchor_index;
                SettingsManager::SetOverlayOrigin(static_cast<Pivot>(current_overlay_origin_index));
            }
            SettingsManager::SetWindowAnchor(static_cast<Pivot>(current_window_anchor_index));
        }
        ImGuiUtil::InlineHelp("Defines the screen corner the overlay is anchored to");

        if (!anchorOriginSync)
        {
            // OVERLAY CONTENT ORIGIN POSITION
            ImGui::SetNextItemWidth(SLIDER_WIDTH);
            if (ImGui::Combo("Overlay content origin", &current_overlay_origin_index, pivotLabels, IM_ARRAYSIZE(pivotLabels)))
                SettingsManager::SetOverlayOrigin(static_cast<Pivot>(current_overlay_origin_index));

            ImGuiUtil::InlineHelp("Sets the point from which reminders expand inside the overlay");
        }

        ImGui::Spacing();

        // X POSITION / OFFSET
        ImGui::SetNextItemWidth(SLIDER_WIDTH);
        if (ImGui::DragFloat("##OverlayPosX", &pos.x, 1.0f, 0.0f, 0.0f, "X: %.1f px"))
        {
            SettingsManager::SetOverlayPosition(pos);
        }
        ImGuiUtil::HoverTooltip("Click and Drag or Ctrl + Click to enter a value");

        ImGui::SameLine();
        if (ImGui::Button("-##OverlayPosXBtnMinus", ImVec2(30, 0)))
            SettingsManager::SetOverlayPosition({pos.x - 1.0f, pos.y});

        ImGui::SameLine();
        if (ImGui::Button("+##OverlayPosXBtnPlus", ImVec2(30, 0)))
            SettingsManager::SetOverlayPosition({pos.x + 1.0f, pos.y});

        // Y POSITION / OFFSET
        ImGui::SetNextItemWidth(SLIDER_WIDTH);
        if (ImGui::DragFloat("##OverlayPosY", &pos.y, 1.0f, 0.0f, 0.0f, "Y: %.1f px"))
        {
            SettingsManager::SetOverlayPosition(pos);
        }
        ImGuiUtil::HoverTooltip("Click and Drag or Ctrl + Click to enter a value");

        ImGui::SameLine();
        if (ImGui::Button("-##OverlayPosYBtnMinus", ImVec2(30, 0)))
            SettingsManager::SetOverlayPosition({pos.x, pos.y - 1.0f});

        ImGui::SameLine();
        if (ImGui::Button("+##OverlayPosYBtnPlus", ImVec2(30, 0)))
            SettingsManager::SetOverlayPosition({pos.x, pos.y + 1.0f});

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // RESET POSITION BUTTON
        if (ImGui::SmallButton("Reset position"))
            SettingsManager::ResetPosition();
    }

    ///----------------------------------------------------------------------------------------------------
    /// MISC SETTINGS
    ///----------------------------------------------------------------------------------------------------

    ImGui::Spacing();
    if (ImGui::CollapsingHeader("Debug & Reset"))
    {
        bool debugWindowEnabled = SettingsManager::IsDebugWindowEnabled();
        if (ImGui::Checkbox("Debug window", &debugWindowEnabled))
        {
            SettingsManager::SetDebugWindowEnabled(debugWindowEnabled);
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("Reset settings to default"))
        {
            SettingsManager::ResetSettings();
        }
    }
}
