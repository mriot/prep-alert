#pragma once

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
}

#ifdef DEBUG
void CoordDumper(const char *aIdentifier, bool isDown);
#endif
