#include "Debug.h"
#include <Common/Globals.h>
#include <Common/Types.h>
#include <Common/Utils.h>
#include <Data/SettingsManager.h>
#include <chrono>
#include <imgui/imgui.h>
#include <string>
#include <format>
#include <vector>

namespace DebugOverlay
{
    std::string GetMapName()
    {
        return G::IsOnSupportedMap ? G::CurrentMapData.name : "n/a";
    }

    std::string GetSectorName()
    {
        if (!G::IsOnSupportedMap)
            return "n/a";

        auto const it = std::ranges::find_if(G::CurrentMapData.sectors, [](const Sector &sector) {
            return sector.id == G::CurrentSectorID;
        });
        return it != G::CurrentMapData.sectors.end() ? it->name : "n/a";
    }

    std::string GetContinentName()
    {
        switch (G::CurrentContinent)
        {
        case Continent::Tyria:
            return "Tyria";
        case Continent::Mists:
            return "Mists";
        default:
            return "Unknown";
        }
    }

    void RenderDebugOverlay(const std::vector<Buff> &buffReminders)
    {
        bool isVisible = SettingsManager::IsDebugWindowEnabled();
        if (ImGui::Begin(std::format("{} DEBUG", G::ADDON_NAME).c_str(), &isVisible, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (UIState::IsOptionsPaneOpen)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 100, 255));
                ImGui::Text("Some data does not update with options open");
                ImGui::PopStyleColor();
                ImGui::Separator();
            }

            ImGui::Text("Supported Map: %s", G::IsOnSupportedMap ? "Yes" : "No");
            ImGui::Text("Continent: %s", GetContinentName().c_str());
            ImGui::Text("Map ID: %d", G::CurrentMapData.id);
            ImGui::SameLine();
            ImGui::TextDisabled("(%s)", GetMapName().c_str());
            ImGui::Text("Sector ID: %d", G::CurrentSectorID);
            ImGui::SameLine();
            ImGui::TextDisabled("(%s)", GetSectorName().c_str());
            ImGui::Text("Floor Level: %d", G::CurrentMapFloor);
            if (G::CurrentMapFloor == 0)
            {
                ImGui::SameLine();
                ImGui::TextDisabled("(not yet detected)");
            }
            ImGui::Separator();
            ImGui::Text("Buff Reminders: %zu", buffReminders.size());
            for (const Buff &buff : buffReminders)
            {
                std::string buffTypeStr;
                switch (buff.type)
                {
                case BuffType::Utility:
                    buffTypeStr = "Utility";
                    break;
                case BuffType::Sigil:
                    buffTypeStr = "Sigil";
                    break;
                case BuffType::Food:
                    buffTypeStr = "Food";
                    break;
                case BuffType::Reset:
                    buffTypeStr = "Reset";
                    break;
                default:
                    buffTypeStr = "Unknown";
                    break;
                }
                ImGui::BulletText("ID: %d, Name: %s, Type: %s", buff.id, buff.name.c_str(), buffTypeStr.c_str());
            }
            ImGui::End();
        }
        if (!isVisible)
            SettingsManager::SetDebugWindowEnabled(isVisible);
    }
}
