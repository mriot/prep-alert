#pragma once

#include <string>
#include <imgui/imgui.h>

#ifdef DEBUG
void CoordDumper(const char *aIdentifier, const bool isDown);
#endif

namespace Log
{
    void Trace(const std::string &message);

    void Debug(const std::string &message);

    void Info(const std::string &message);

    void Warning(const std::string &message);

    void Critical(const std::string &message);
} // namespace Log

namespace ImGui
{
    static void TextOutlined(const char *fmt, ...)
    {
        ImVec2 pos = GetCursorPos();

        va_list args;
        va_start(args, fmt);
        pos.x += 1;
        pos.y += 1;
        SetCursorPos(pos);

        const float alpha = ImGui::GetStyle().Alpha;
        TextColoredV(ImVec4(0, 0, 0, alpha), fmt, args);

        pos.x -= 1;
        pos.y -= 1;
        SetCursorPos(pos);
        TextV(fmt, args);
        va_end(args);
    }
}

