#include "Debug.h"

#include "Common/BuffData.h"

#include <Common/Globals.h>
#include <Common/Types.h>
#include <Common/Utils.h>
#include <Data/SettingsManager.h>
#include <imgui/imgui.h>
#include <string>
#include <format>
#include <utility>
#include <vector>

namespace Debug
{
    namespace
    {
        std::string GetMapName()
        {
            return WorldState::IsOnSupportedMap ? WorldState::CurrentMapData.name : "n/a";
        }

        std::string GetSectorName()
        {
            if (!WorldState::IsOnSupportedMap)
                return "n/a";

            auto const it = std::ranges::find_if(WorldState::CurrentMapData.sectors, [](const Sector &sector) {
                return sector.id == WorldState::CurrentSectorID;
            });
            return it != WorldState::CurrentMapData.sectors.end() ? it->name : "n/a";
        }

        std::string GetContinentName()
        {
            switch (WorldState::CurrentContinent)
            {
            case Continent::Tyria:
                return "Tyria";
            case Continent::Mists:
                return "Mists";
            default:
                return "Unknown";
            }
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

            ImGui::Text("Supported Map: %s", WorldState::IsOnSupportedMap ? "Yes" : "No");
            ImGui::Text("Continent: %s", GetContinentName().c_str());
            ImGui::Text("Map ID: %d", WorldState::CurrentMapData.id);
            ImGui::SameLine();
            ImGui::TextDisabled("(%s)", GetMapName().c_str());
            ImGui::Text("Sector ID: %d", WorldState::CurrentSectorID);
            ImGui::SameLine();
            ImGui::TextDisabled("(%s)", GetSectorName().c_str());
            ImGui::Text("Floor Level: %d", Info.actualFloorLevel);
            if (Info.actualFloorLevel == 0)
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
                case BuffType::SlayingSigil:
                    buffTypeStr = "Sigil";
                    break;
                default:
                    buffTypeStr = "Unknown";
                    break;
                }
                ImGui::BulletText("ID: %d, Name: %s, Type: %s", buff.id, BuffData::GetBuffName(buff.id).c_str(), buffTypeStr.c_str());
            }
            ImGui::End();
        }
        if (!isVisible)
            SettingsManager::SetDebugWindowEnabled(isVisible);
    }
}
