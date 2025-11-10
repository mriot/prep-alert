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
    bool compactMode = SettingsManager::IsCompactMode();
    if (ImGui::Checkbox("No Text", &compactMode))
    {
        SettingsManager::SetCompactMode(compactMode);
    }

    if (compactMode)
    {
        ImGui::SameLine();
        bool horizontalMode = SettingsManager::IsHorizontalMode();
        if (ImGui::Checkbox("Horizontal", &horizontalMode))
        {
            SettingsManager::SetHorizontalMode(horizontalMode);
        }
    }

    ImGui::Spacing();

    ImGui::Text("Buffs to alert you about");
    ShownBuffTypes shownBuffs = SettingsManager::GetShownBuffTypes();

    // ImGui::SameLine();
    if (ImGui::Checkbox("Utility", &shownBuffs.utility))
    {
        SettingsManager::SetShownBuffTypes(shownBuffs);
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("Sigil", &shownBuffs.sigil))
    {
        SettingsManager::SetShownBuffTypes(shownBuffs);
    }
    // ImGui::SameLine();
    // if (ImGui::Checkbox("Food", &shownBuffs.food))
    // {
    //     SettingsManager::SetShownBuffTypes(shownBuffs);
    // }

    ImGui::Spacing();

    ImGui::Text("Icon size");
    int imageSize = SettingsManager::GetImageSize();
    if (ImGui::SliderInt("Pixel", &imageSize, 24, 64))
    {
        SettingsManager::SetImageSize(imageSize);
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Ctrl + Click to enter a value (Default 32px)");
    }

    ImGui::Spacing();

    ImGui::Text("Buff reminder flash animation duration");
    int flashingDuration = SettingsManager::GetFlashingDuration();
    if (ImGui::SliderInt("Seconds", &flashingDuration, 0, 60))
    {
        SettingsManager::SetFlashingDuration(flashingDuration);
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Ctrl + Click to enter a value (Default 5s)");
    }

    ImGui::Spacing();

    G::IsOptionsPaneOpen = true;
    SettingsManager::SetOverlayDragEnabled(true);

    // Overlay positioning
    ImGui::Text("Change overlay position");
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

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // supported maps table
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
