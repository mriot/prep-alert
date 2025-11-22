#include "Debug.h"
#include <Common/Globals.h>
#include <Common/Types.h>
#include <Common/Utils.h>
#include <Data/SettingsManager.h>
#include <chrono>
#include <imgui/imgui.h>
#include <string>

namespace DebugOverlay
{
    void RenderDebugOverlay(const std::vector<Buff> &buffReminders)
    {
        auto getMapName = []() -> std::string {
            if (!G::IsOnSupportedMap)
                return "n/a";
            try
            {
                return G::MapDataMap.at(G::CurrentMapID).name;
            }
            catch (const std::exception &)
            {
                return "n/a";
            }
        };

        auto getSectorName = []() -> std::string {
            if (!G::IsOnSupportedMap)
                return "n/a";
            try
            {
                for (const Sector &sector : G::MapDataMap.at(G::CurrentMapID).sectors)
                {
                    if (sector.id == G::CurrentSectorID)
                    {
                        return sector.name;
                    }
                }
                return "n/a";
            }
            catch (const std::exception &e)
            {
                return e.what();
            }
        };

        if (ImGui::Begin(std::format("{} DEBUG", G::ADDON_NAME).c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (UIState::IsOptionsPaneOpen)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 100, 255));
                ImGui::Text("Some data does not update with options open");
                ImGui::PopStyleColor();
                ImGui::Separator();
            }

            ImGui::Text("Supported map: %s", G::IsOnSupportedMap ? "Yes" : "No");
            ImGui::Text("Map ID: %d", G::CurrentMapID);
            ImGui::SameLine();
            ImGui::TextDisabled("(%s)", getMapName().c_str());
            ImGui::Text("Sector ID: %d", G::CurrentSectorID);
            ImGui::SameLine();
            ImGui::TextDisabled("(%s)", getSectorName().c_str());
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
                ImGui::BulletText("ID: %d, Name: %s", buff.id, buff.name.c_str());
            }
        }
    }
}


