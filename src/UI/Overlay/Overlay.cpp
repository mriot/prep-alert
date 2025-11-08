#include "Overlay.h"
#include <Common/Globals.h>
#include <Common/Types.h>
#include <Common/Utils.h>
#include <Data/SettingsManager.h>
#include <Data/TextureLoader.h>
#include <format>
#include <imgui/imgui.h>
#include <nexus/Nexus.h>
#include <string>

namespace
{
    bool showOverlay = false;

    void HandleOverlayDrag()
    {
        static ImVec2 s_prevPos = SettingsManager::GetOverlayPosition();
        static bool s_wasMoving = false;

        ImVec2 currentPos = ImGui::GetWindowPos();

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
} // namespace

namespace Overlay
{
    void ToggleOverlay(bool state)
    {
        showOverlay = !state;
    }

    void RenderOverlay(const Buff &buff)
    {
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

        if (!isDragEnabled)
        {
            if (buff.id <= 0 || !showOverlay)
                return;

            // in "prod mode" make the overlay non-interactive
            flags |= ImGuiWindowFlags_NoInputs;
        }

        ImGui::SetNextWindowPos(SettingsManager::GetOverlayPosition(), isDragEnabled && !SettingsManager::IsOverlayPositionDirty() ? ImGuiCond_FirstUseEver : ImGuiCond_Always);

        if (ImGui::Begin(G::ADDON_NAME, nullptr, flags))
        {
            if (Texture_t *texture = LoadTexture(buff.id))
            {
                ImGui::Image((void *)texture->Resource, ImVec2(32, 32));
            }

            ImGui::SameLine();
            float lineheight = ImGui::GetFontSize();
            ImGui::SetCursorPos(ImVec2(50, lineheight * 0.5f + 2.5));
            ImGui::Text(buff.name.c_str());

            if (isDragEnabled)
                HandleOverlayDrag();
        }
        ImGui::End();
    }
} // namespace Overlay

