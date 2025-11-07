#pragma once

#include <string>

namespace Log
{
    void Trace(const std::string &message);
    void Debug(const std::string &message);
    void Info(const std::string &message);
    void Warning(const std::string &message);
    void Critical(const std::string &message);
} // namespace Log

