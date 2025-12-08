#include "imgui/imgui.h"
#include <filesystem>
#include <fstream>
#include <Common/Globals.h>
#include <nexus/Nexus.h>
#include <string>


/// ----------------------------------------------------------------------------------------------------
/// Logging utilities
/// ----------------------------------------------------------------------------------------------------

namespace Log
{
    void Trace(const std::string &message)
    {
        G::APIDefs->Log(LOGL_TRACE, G::ADDON_NAME, message.c_str());
    }

    void Debug(const std::string &message)
    {
        G::APIDefs->Log(LOGL_DEBUG, G::ADDON_NAME, message.c_str());
    }

    void Info(const std::string &message)
    {
        G::APIDefs->Log(LOGL_INFO, G::ADDON_NAME, message.c_str());
    }

    void Warning(const std::string &message)
    {
        G::APIDefs->Log(LOGL_WARNING, G::ADDON_NAME, message.c_str());
    }

    void Critical(const std::string &message)
    {
        G::APIDefs->Log(LOGL_CRITICAL, G::ADDON_NAME, message.c_str());
    }
}


/// ----------------------------------------------------------------------------------------------------
/// ImGui utilities
/// ----------------------------------------------------------------------------------------------------

namespace ImGuiUtil
{
    void HoverTooltip(const std::string &text)
    {
        if (ImGui::IsItemHovered())
        {
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 3.0f));
            ImGui::SetTooltip("%s", text.c_str());
            ImGui::PopStyleVar(2);
        }
    }

    void TextOutlined(const char *fmt, ...)
    {
        ImVec2 pos = ImGui::GetCursorPos();

        va_list args = nullptr;
        va_start(args, fmt);
        pos.x += 1;
        pos.y += 1;
        ImGui::SetCursorPos(pos);

        const float alpha = ImGui::GetStyle().Alpha;
        ImGui::TextColoredV(ImVec4(0, 0, 0, alpha), fmt, args);

        pos.x -= 1;
        pos.y -= 1;
        ImGui::SetCursorPos(pos);
        ImGui::TextV(fmt, args);
        va_end(args);
    }

    void InlineHelp(const char *desc)
    {
        ImGui::SameLine();
        ImGui::TextDisabled("(?)");
        HoverTooltip(desc);
    }

    void TextInBox(const char *msg, const ImVec4 &bgColor, const ImVec4 &borderColor)
    {
        ImGui::Spacing();

        constexpr float padX = 8.0f;
        constexpr float padY = 6.0f;

        const ImVec2 pos      = ImGui::GetCursorScreenPos();
        const ImVec2 textSize = ImGui::CalcTextSize(msg, nullptr, false, -1.0f);
        const ImVec2 boxSize(textSize.x + padX * 2.0f, textSize.y + padY * 2.0f);
        const ImVec2 posEnd = ImVec2(pos.x + boxSize.x, pos.y + boxSize.y);

        ImDrawList *draw = ImGui::GetWindowDrawList();

        draw->AddRectFilled(pos, posEnd, ImGui::GetColorU32(bgColor), 5.0f);
        draw->AddRect(pos, posEnd, ImGui::GetColorU32(borderColor), 5.0f, 0, 2.0f);

        ImGui::SetCursorScreenPos(ImVec2(pos.x + padX, pos.y + padY));
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", msg);

        ImGui::SetCursorScreenPos(posEnd);
        ImGui::Spacing();
    }
}


/// ----------------------------------------------------------------------------------------------------
/// Other utilities
/// ----------------------------------------------------------------------------------------------------

namespace Utils
{
    ImVec2 PivotToImVec2(const Pivot pivot)
    {
        switch (pivot)
        {
        case Pivot::TopLeft:
            return {0.0f, 0.0f};
        case Pivot::TopRight:
            return {1.0f, 0.0f};
        case Pivot::BottomLeft:
            return {0.0f, 1.0f};
        case Pivot::BottomRight:
            return {1.0f, 1.0f};
        default:
            return {0.0f, 0.0f};
        }
    }
}


/// ----------------------------------------------------------------------------------------------------
/// DEBUG UTILITIES
/// ----------------------------------------------------------------------------------------------------

#ifdef DEBUG
void CoordDumper(const char *aIdentifier, const bool isDown)
{
    if (isDown)
    {
        int x = G::MumbleLink->Context.Compass.PlayerPosition.X;
        int y = G::MumbleLink->Context.Compass.PlayerPosition.Y;

        // int x = G::MumbleLink->AvatarPosition.X;
        // int y = G::MumbleLink->AvatarPosition.Y;
        // int z = G::MumbleLink->AvatarPosition.Z;

        const std::filesystem::path dir = G::APIDefs->Paths_GetAddonDirectory(G::ADDON_NAME);
        std::filesystem::create_directories(dir);

        std::ofstream file(dir / "coords.txt", std::ios::app);
        if (file)
        {
            file << std::format("[{}, {}],\n", x, y);
            // file << std::format("[{}, {}, {}],\n", x, y, z);
        }

        Log::Debug(std::format("Logged coords: [{}, {}]", x, y));
        // Log::Debug(std::format("Logged coords: [{}, {}, {}]", x, y, z));
    }
}
#endif
