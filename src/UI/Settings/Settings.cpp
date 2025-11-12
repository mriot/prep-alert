#include "Settings.h"
#include <Common/Globals.h>
#include <Common/Utils.h>
#include <Data/MapData.h>
#include <Data/SettingsManager.h>
#include <algorithm>
#include <imgui/imgui.h>
#include <string>
#include <vector>

void OnOptionsRender()
{
    ///----------------------------------------------------------------------------------------------------
    /// buff types to show
    ///----------------------------------------------------------------------------------------------------

    ImGui::Text("Buff types to show");
    ShownBuffTypes shownBuffs = SettingsManager::GetShownBuffTypes();

    if (ImGui::Checkbox("Enhancement", &shownBuffs.utility))
    {
        SettingsManager::SetShownBuffTypes(shownBuffs);
    }

    if (ImGui::Checkbox("Sigil", &shownBuffs.sigil))
    {
        SettingsManager::SetShownBuffTypes(shownBuffs);
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

        bool affe = SettingsManager::IsHorizontalMode();
        if (ImGui::Checkbox("Enable tooltips on hover", &affe))
        {
            SettingsManager::SetHorizontalMode(affe);
        }
        ImGui::SameLine();
        ImGui::TextDisabled("(i)");
        ImGui::HoverTooltip("Note: Mouse events aren't passed through to the game if enabled");

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
    /// default buff reminder timeout
    ///----------------------------------------------------------------------------------------------------

    ImGui::Text("Default buff reminder timeout");
    ImGui::SameLine();
    ImGui::TextDisabled("(i)");
    ImGui::HoverTooltip("Default buff reminders are shown on a timer.\nYou can configure their timeout here.");

    int defaultBuffTimeout = SettingsManager::GetDefaultBuffReminderTimeout();
    if (ImGui::SliderInt("Timeout seconds", &defaultBuffTimeout, 0, 60))
    {
        SettingsManager::SetDefaultBuffReminderTimeout(defaultBuffTimeout);
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

    G::IsOptionsPaneOpen = true;
    SettingsManager::SetOverlayDragEnabled(true);

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
    /// supported maps table
    ///----------------------------------------------------------------------------------------------------

    std::string headerLabel = "Supported maps: " + std::to_string(G::SupportedMaps.size());

    if (ImGui::CollapsingHeader(headerLabel.c_str()))
    {
        std::vector<std::pair<int, MapData>> entries;
        entries.reserve(G::MapDataMap.size());

        for (const auto &kv : G::MapDataMap)
        {
            entries.emplace_back(kv.first, kv.second);
        }

        std::sort(entries.begin(), entries.end(), [](const auto &a, const auto &b) {
            return a.first < b.first;
        });

        ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit;
        if (ImGui::BeginTable("SupportedMapsTable", 2, tableFlags))
        {
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 60.0f);
            ImGui::TableSetupColumn("Map name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();

            for (const auto &entry : entries)
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", entry.first);

                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(entry.second.name.c_str());
            }

            ImGui::EndTable();
        }
    }
}
