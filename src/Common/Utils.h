#pragma once

#include "imgui/imgui.h"
#include <Common/Types.h>
#include <string>

namespace Log
{
    void Trace(const std::string &message);

    void Debug(const std::string &message);

    void Info(const std::string &message);

    void Warning(const std::string &message);

    void Critical(const std::string &message);
}

namespace ImGuiUtil
{
    void HoverTooltip(const std::string &text);

    void TextOutlined(const char *fmt, ...);

    void InlineHelp(const char *desc);

    void TextInBox(const char *msg, const ImVec4 &bgColor, const ImVec4 &borderColor);
}

namespace Utils
{
    ImVec2 PivotToVec2(Pivot pivot);
}

#ifdef DEBUG
void CoordDumper(const char *aIdentifier, bool isDown);
#endif
