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

namespace Overlay
{
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

        void RenderBuffIcon(const Buff &buff, const ImVec2 &imageSize, const float &windowAlpha)
        {
            if (const Texture_t *texture = LoadTexture(buff.id))
                ImGui::Image(texture->Resource, imageSize);
            else
                ImGui::Dummy(imageSize);

            // draw red X over reset-type buffs
            if (buff.type == BuffType::Reset)
            {
                {
                    ImDrawList *drawList      = ImGui::GetWindowDrawList();
                    const ImVec2 iconMin      = ImGui::GetItemRectMin();
                    const ImVec2 iconMax      = ImGui::GetItemRectMax();
                    const ImU32 red           = IM_COL32(255, 0, 0, windowAlpha * 255.0f);
                    constexpr float thickness = 5.0f;
                    constexpr float padding   = 3.0f;

                    const ImVec2 paddedMin = ImVec2(iconMin.x + padding, iconMin.y + padding);
                    const ImVec2 paddedMax = ImVec2(iconMax.x - padding, iconMax.y - padding);

                    drawList->AddLine(paddedMin, paddedMax, red, thickness);
                    drawList->AddLine(ImVec2(paddedMax.x, paddedMin.y), ImVec2(paddedMin.x, paddedMax.y), red, thickness);
                }
            }
        }

        float RotateAlpha()
        {
            const float t = ImGui::GetTime() * 10.0f;
            return 0.5f + 0.5f * sinf(t); // oscillates between 0 and 1 (sinf -1 to 1)
        }
    }

    void RenderOverlay(const std::vector<Buff> &buffReminders)
    {
        static std::vector<int> prevBuffIDs = {};
        static float windowAlpha            = 1.0f;
        static auto lastFrameTime           = std::chrono::steady_clock::now();

        if (buffReminders.empty())
        {
            prevBuffIDs.clear(); // cleanup when no buffs to show anymore
            return;
        }

        if (!G::APIDefs->ImguiContext || !ImGui::GetCurrentContext())
        {
            Log::Critical("Failed to get ImGui context");
            return;
        }

        // default window flags
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing |
                                 ImGuiWindowFlags_NoBackground |
                                 ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoTitleBar;

        if (!UIState::IsOptionsPaneOpen)
        {
            const auto currentFrameTime = std::chrono::steady_clock::now();
            const auto elapsed          = std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - lastFrameTime).count();

            // when not in options pane make the overlay non-interactive
            flags |= ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoFocusOnAppearing;

            // disable all inputs only if tooltips option is disabled (can only be active in compact mode)
            if (!SettingsManager::IsCompactMode() || !SettingsManager::IsTooltipsEnabled())
                flags |= ImGuiWindowFlags_NoInputs;

            // gather current buff IDs to check for changes (used for flashing effect)
            std::vector<int> currentBuffIDs;
            for (const Buff &buff : buffReminders)
                currentBuffIDs.push_back(buff.id);

            // check if buff list changed for flashing effect
            if (prevBuffIDs != currentBuffIDs)
            {
                prevBuffIDs   = currentBuffIDs;
                lastFrameTime = std::chrono::steady_clock::now();
            }

            // calculate alpha for flashing effect
            windowAlpha = elapsed <= SettingsManager::GetFlashingDuration() * 1000 ? RotateAlpha() : 1.0f;
        }

        // only set position on first use to allow dragging and precise position simultaneously
        ImGui::SetNextWindowPos(SettingsManager::GetOverlayPosition(), UIState::IsOptionsPaneOpen && !SettingsManager::IsOverlayPositionDirty() ? ImGuiCond_FirstUseEver : ImGuiCond_Always);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, UIState::IsOptionsPaneOpen ? 1.0f : windowAlpha); // full alpha in options pane (no flashing effect)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(ImGui::GetStyle().CellPadding.x, 0.0f)); // no vertical padding in table cells

        if (ImGui::Begin(G::ADDON_NAME, nullptr, flags))
        {
            const ImVec2 imageSize(SettingsManager::GetImageSize(), SettingsManager::GetImageSize());

            if (UIState::IsOptionsPaneOpen)
                HandleOverlayDrag();

            // render buff reminders
            for (const Buff &buff : buffReminders)
            {
                // compact mode (icons only)
                if (SettingsManager::IsCompactMode())
                {
                    RenderBuffIcon(buff, imageSize, windowAlpha);

                    if (SettingsManager::IsTooltipsEnabled())
                        ImGui::HoverTooltip(buff.name);

                    if (SettingsManager::IsHorizontalMode())
                        ImGui::SameLine();
                }
                // normal mode (with names)
                else if (ImGui::BeginTable(std::format("{} Reminders", G::ADDON_NAME).c_str(), 2, ImGuiTableFlags_SizingFixedFit))
                {
                    const float textHeight = ImGui::GetTextLineHeight();
                    ImGui::TableSetupColumn("Icon", ImGuiTableColumnFlags_WidthFixed, imageSize.x);
                    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);

                    ImGui::TableNextColumn();

                    RenderBuffIcon(buff, imageSize, windowAlpha);

                    ImGui::TableNextColumn();

                    // center text vertically in cell
                    const ImVec2 cellMin = ImGui::GetCursorScreenPos();
                    const ImVec2 cellMax = ImVec2(cellMin.x + ImGui::GetColumnWidth(), cellMin.y + imageSize.y);
                    const float yOffset  = (imageSize.y - textHeight) * 0.5f;

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
}
