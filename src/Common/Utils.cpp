#include <Common/Globals.h>
#include <nexus/Nexus.h>
#include <string>

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
} // namespace Log

