#include <filesystem>
#include <fstream>
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

#ifdef DEBUG
void CoordDumper(const char *aIdentifier, const bool isDown)
{
    if (isDown)
    {

        int x = G::MumbleLink->Context.Compass.PlayerPosition.X;
        int y = G::MumbleLink->Context.Compass.PlayerPosition.Y;

        // float x = G::MumbleLink->AvatarPosition.X;
        // float y = G::MumbleLink->AvatarPosition.Y;
        // float z = G::MumbleLink->AvatarPosition.Z;

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
