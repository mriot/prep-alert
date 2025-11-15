#include "Overlay.h"
#include <Common/Globals.h>
#include <Common/Types.h>
#include <Common/Utils.h>
#include <Data/SettingsManager.h>
#include <Data/TextureLoader.h>
#include <chrono>
#include <imgui/imgui.h>
#include <nexus/Nexus.h>
#include <string>

namespace
{
    void HandleOverlayDrag()
    {
        static ImVec2 prevPos = SettingsManager::GetOverlayPosition();
        static bool wasMoving = false;

        const ImVec2 currentPos = ImGui::GetWindowPos();

        if (currentPos.x != prevPos.x || currentPos.y != prevPos.y)
        {
            wasMoving = true;
        }

        if (wasMoving && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            SettingsManager::SetOverlayPosition(currentPos);
            wasMoving = false;
            prevPos   = currentPos;
        }
    }

    float RotateAlpha()
    {
        const float t = ImGui::GetTime() * 10.0f; // time since imgui initialized
        return 0.5f + 0.5f * sinf(t); // oscillates between 0 and 1 (sinf -1 to 1)
    }
}

namespace Overlay
{
    void RenderOverlay(const std::vector<Buff> &buffs)
    {
        static float alpha                  = 1.0f;
        static std::vector<int> prevBuffIDs = {};

        if (buffs.empty())
        {
            prevBuffIDs.clear(); // cleanup when no buffs to show anymore
            return;
        }

        static auto last = std::chrono::steady_clock::now();

        if (!G::APIDefs->ImguiContext || !ImGui::GetCurrentContext())
        {
            Log::Critical("Failed to get ImGui context");
            return;
        }

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing |
                                 ImGuiWindowFlags_NoBackground |
                                 ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoTitleBar;

        if (!UIState::IsOptionsPaneOpen)
        {
            // when not in options pane make the overlay non-interactive
            flags |= ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoFocusOnAppearing;

            if (!SettingsManager::IsTooltipsEnabled())
                flags |= ImGuiWindowFlags_NoInputs; // cant set this if user wants tooltips

            // gather current buff IDs to check for changes
            std::vector<int> currentBuffIDs;
            for (const Buff &buff : buffs)
            {
                currentBuffIDs.push_back(buff.id);
            }

            // check if buff list changed for flashing effect
            if (prevBuffIDs != currentBuffIDs)
            {
                prevBuffIDs = currentBuffIDs;
                last        = std::chrono::steady_clock::now();
            }

            const auto now     = std::chrono::steady_clock::now();
            const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();

            alpha = elapsed <= SettingsManager::GetFlashingDuration() * 1000 ? RotateAlpha() : 1.0f; // flashing effect
        }

        // only set position on first use to allow dragging and precise position simultaneously
        ImGui::SetNextWindowPos(SettingsManager::GetOverlayPosition(), UIState::IsOptionsPaneOpen && !SettingsManager::IsOverlayPositionDirty() ? ImGuiCond_FirstUseEver : ImGuiCond_Always);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, UIState::IsOptionsPaneOpen ? 1.0f : alpha);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(ImGui::GetStyle().CellPadding.x, 0.0f));

        if (ImGui::Begin(G::ADDON_NAME, nullptr, flags))
        {
            if (UIState::IsOptionsPaneOpen)
                HandleOverlayDrag();

            for (const Buff &buff : buffs)
            {
                const ImVec2 imageSize(SettingsManager::GetImageSize(), SettingsManager::GetImageSize());

                // compact mode
                if (SettingsManager::IsCompactMode())
                {
                    if (const Texture_t *texture = LoadTexture(buff.id))
                        ImGui::Image((void *)texture->Resource, imageSize);
                    else
                        ImGui::Dummy(imageSize);

                    if (SettingsManager::IsTooltipsEnabled())
                        ImGui::HoverTooltip(buff.name);

                    if (SettingsManager::IsHorizontalMode())
                    {
                        ImGui::SameLine();
                    }
                }
                // normal mode
                else if (ImGui::BeginTable("buffs", 2, ImGuiTableFlags_SizingFixedFit))
                {
                    const float textHeight = ImGui::GetTextLineHeight();

                    ImGui::TableSetupColumn("Icon", ImGuiTableColumnFlags_WidthFixed, imageSize.x);
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);

                    ImGui::TableNextColumn();
                    if (Texture_t *texture = LoadTexture(buff.id))
                        ImGui::Image((void *)texture->Resource, imageSize);
                    else
                        ImGui::Dummy(imageSize);
                    ImGui::TableNextColumn();

                    const ImVec2 cellMin = ImGui::GetCursorScreenPos();
                    const ImVec2 cellMax = ImVec2(cellMin.x + ImGui::GetColumnWidth(), cellMin.y + imageSize.y);
                    const float yOffset  = (imageSize.y - textHeight) * 0.5f; // center text vertically

                    ImGui::SetCursorScreenPos(ImVec2(cellMin.x, cellMin.y + yOffset));

                    ImGui::TextOutlined("%s", buff.name.c_str());

                    ImGui::SetCursorScreenPos(ImVec2(cellMin.x, cellMax.y));

                    ImGui::EndTable();
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleVar(4); // window alpha + window padding + item spacing + cell padding
    }
} // namespace Overlay
