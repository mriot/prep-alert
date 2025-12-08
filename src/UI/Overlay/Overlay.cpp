#include "Overlay.h"

#include "Common/BuffData.h"

#include <Common/Globals.h>
#include <Common/Types.h>
#include <Common/Utils.h>
#include <Data/SettingsManager.h>
#include <Data/Texture.h>
#include <chrono>
#include <imgui/imgui.h>
#include <nexus/Nexus.h>
#include <string>

namespace Overlay
{
    namespace
    {
        ImVec2 GetPivotRelativePosition(const ImVec2 &offset, const Pivot &pivot)
        {
            const ImGuiIO &io     = ImGui::GetIO();
            const ImVec2 viewport = io.DisplaySize;

            switch (pivot)
            {
            case Pivot::TopLeft:
                return {offset.x, offset.y};
            case Pivot::TopRight:
                return {viewport.x - offset.x, offset.y};
            case Pivot::BottomLeft:
                return {offset.x, viewport.y - offset.y};
            case Pivot::BottomRight:
                return {viewport.x - offset.x, viewport.y - offset.y};
            default:
                return {offset.x, offset.y};
            }
        }

        void RenderBuffIcon(const Buff &buff, const ImVec2 &imageSize, const float &windowAlpha)
        {
            if (const Texture_t *texture = Texture::LoadBuffTex(buff.id))
                ImGui::Image(texture->Resource, imageSize);
            else
                ImGui::Dummy(imageSize);

            // draw red X over icon if buff should be removed
            if (buff.remove)
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

        void RenderBuffName(const Buff &buff, const ImVec2 &imageSize, const float &textHeight)
        {
            // center text vertically in cell
            const ImVec2 cellMin          = ImGui::GetCursorScreenPos();
            const ImVec2 cellMax          = ImVec2(cellMin.x + ImGui::GetColumnWidth(), cellMin.y + imageSize.y);
            const float yOffset           = (imageSize.y - textHeight) * 0.5f;
            const std::string displayName = (buff.remove ? "Replace " : "") + BuffData::GetBuffName(buff.id);

            ImGui::SetCursorScreenPos(ImVec2(cellMin.x, cellMin.y + yOffset));
            ImGuiUtil::TextOutlined("%s", displayName.c_str());
            ImGui::SetCursorScreenPos(ImVec2(cellMin.x, cellMax.y));
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

        ImGuiWindowFlags winFlags = ImGuiWindowFlags_NoFocusOnAppearing |
                                    ImGuiWindowFlags_NoBackground |
                                    ImGuiWindowFlags_NoCollapse |
                                    ImGuiWindowFlags_AlwaysAutoResize |
                                    ImGuiWindowFlags_NoNav |
                                    ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoTitleBar;

        if (!UIState::IsOptionsPaneOpen)
        {
            const auto currentFrameTime = std::chrono::steady_clock::now();
            const auto elapsed          = std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - lastFrameTime).count();

            // disable all inputs only if tooltips option is disabled (can only be active in compact mode)
            if (!SettingsManager::IsCompactMode() || !SettingsManager::IsTooltipsEnabled())
                winFlags |= ImGuiWindowFlags_NoInputs;

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

        const ImVec2 offset       = SettingsManager::GetOverlayPosition();
        const Pivot windowAnchor  = SettingsManager::GetWindowAnchor();
        const Pivot overlayOrigin = SettingsManager::GetOverlayOrigin();

        ImGui::SetNextWindowPos(GetPivotRelativePosition(offset, windowAnchor), ImGuiCond_Always, Utils::PivotToImVec2(overlayOrigin));

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, UIState::IsOptionsPaneOpen ? 1.0f : windowAlpha); // always full alpha in options pane (no flashing effect)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));                            // no padding around the content
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);                                 // no border
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));                        // no spacing between icons/names
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 0.0f));                       // no vertical padding in table cells

        if (ImGui::Begin(G::ADDON_NAME, nullptr, winFlags))
        {
            const bool iconFirst = SettingsManager::IsIconFirst();
            const float size     = SettingsManager::GetImageSize();
            const ImVec2 imageSize(size, size);

            if (SettingsManager::IsCompactMode())
            {
                // compact mode (icons only)
                for (const Buff &buff : buffReminders)
                {
                    RenderBuffIcon(buff, imageSize, windowAlpha);

                    if (SettingsManager::IsTooltipsEnabled())
                        ImGuiUtil::HoverTooltip(BuffData::GetBuffName(buff.id));

                    if (SettingsManager::IsHorizontalMode())
                        ImGui::SameLine();
                }
            }
            // normal mode (icons + names)
            else if (ImGui::BeginTable("##RemindersTable", 2, ImGuiTableFlags_SizingFixedFit))
            {
                const float textHeight = ImGui::GetTextLineHeight();

                ImGui::TableSetupColumn("##RemindersTableCol1");
                ImGui::TableSetupColumn("##RemindersTableCol2");

                for (const Buff &buff : buffReminders)
                {
                    ImGui::TableNextColumn();

                    if (iconFirst)
                    {
                        RenderBuffIcon(buff, imageSize, windowAlpha);
                        ImGui::TableNextColumn();
                        RenderBuffName(buff, imageSize, textHeight);
                    }
                    else
                    {
                        RenderBuffName(buff, imageSize, textHeight);
                        ImGui::TableNextColumn();
                        RenderBuffIcon(buff, imageSize, windowAlpha);
                    }
                }
                ImGui::EndTable();
            }
        }
        ImGui::End();
        ImGui::PopStyleVar(5); // window alpha + window padding + window border size + item spacing + cell padding
    }
}
