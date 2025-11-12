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
#include <nexus/Nexus.h>

namespace
{
    void HandleOverlayDrag()
    {
        static ImVec2 s_prevPos = SettingsManager::GetOverlayPosition();
        static bool s_wasMoving = false;

        const ImVec2 currentPos = ImGui::GetWindowPos();

        if (currentPos.x != s_prevPos.x || currentPos.y != s_prevPos.y)
        {
            s_wasMoving = true;
        }

        if (s_wasMoving && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            SettingsManager::SetOverlayPosition(currentPos);

            s_wasMoving = false;
            s_prevPos   = currentPos;
        }
    }

    float RotateAlpha()
    {
        const float t = ImGui::GetTime() * 10.0f; // time since imgui initialized
        return 0.5f + 0.5f * sinf(t); // oscillates between 0 and 1 (sinf -1 to 1)
    }
} // namespace

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

        const bool isDragEnabled = SettingsManager::IsOverlayDragEnabled();

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing |
                                 ImGuiWindowFlags_NoBackground |
                                 ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoTitleBar;

        if (!isDragEnabled) // normal overlay mode
        {
            // when not in options panel make the overlay non-interactive
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

            alpha = elapsed <= SettingsManager::GetFlashingDuration() * 1000 ? RotateAlpha() : 1.0f;
        }

        ImGui::SetNextWindowPos(SettingsManager::GetOverlayPosition(), isDragEnabled && !SettingsManager::IsOverlayPositionDirty() ? ImGuiCond_FirstUseEver : ImGuiCond_Always);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, isDragEnabled ? 1.0f : alpha);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(ImGui::GetStyle().CellPadding.x, 0.0f));

        if (ImGui::Begin(G::ADDON_NAME, nullptr, flags))
        {
            if (isDragEnabled)
                HandleOverlayDrag();

            for (Buff buff : buffs)
            {
                const ImVec2 imageSize(SettingsManager::GetImageSize(), SettingsManager::GetImageSize());

                // compact mode
                if (SettingsManager::IsCompactMode())
                {
                    if (Texture_t *texture = LoadTexture(buff.id))
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

                    ImVec2 cellMin = ImGui::GetCursorScreenPos();
                    ImVec2 cellMax = ImVec2(cellMin.x + ImGui::GetColumnWidth(), cellMin.y + imageSize.y);
                    float yOffset  = (imageSize.y - textHeight) * 0.5f;

                    ImGui::SetCursorScreenPos(ImVec2(cellMin.x, cellMin.y + yOffset));

                    ImGui::TextOutlined("%s", buff.name.c_str());

                    ImGui::SetCursorScreenPos(ImVec2(cellMin.x, cellMax.y));

                    ImGui::EndTable();
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleVar(4); // alpha + window padding + item spacing + cell padding
    }
} // namespace Overlay
