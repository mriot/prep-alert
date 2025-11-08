#include "SettingsManager.h"
#include <Common/Globals.h>
#include <Common/Types.h>
#include <Common/Utils.h>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <imgui/imgui.h>
#include <ios>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>

using json = nlohmann::json;

namespace
{
    static std::mutex s_SettingsMutex;
    static Settings s_settings{{70.0f, 40.0f}};

    static bool s_overlayDragEnabled   = false;
    static bool s_overlayPositionDirty = false;
    // static int s_overlayTimeoutSeconds = 30;

    void DebouncedSave()
    {
        static std::atomic<uint64_t> token{0};
        auto threadToken = ++token; // invalidate previous tokens

        std::thread([threadToken]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if (token.load(std::memory_order_acquire) == threadToken)
            {
                SettingsManager::SaveSettings();
            }
        }).detach();
    }
} // namespace

namespace SettingsManager
{
    // Overlay drag
    bool IsOverlayDragEnabled()
    {
        return s_overlayDragEnabled;
    }
    void SetOverlayDragEnabled(bool enabled)
    {
        s_overlayDragEnabled = enabled;
    }

    // Reset overlay position
    void ResetOverlayPosition()
    {
        SetOverlayPosition({70.0f, 40.0f});
        s_overlayPositionDirty = true;
    }

    // Overlay timeout
    /*
    int GetOverlayTimeoutSeconds()
    {

        return s_overlayTimeoutSeconds;
    }
    void SetOverlayTimeoutSeconds(int seconds)
    {

        s_overlayTimeoutSeconds = seconds;
    }
    */

    // Overlay position
    ImVec2 GetOverlayPosition()
    {
        return ImVec2(s_settings.overlayPosition.x, s_settings.overlayPosition.y);
    }
    void SetOverlayPosition(const ImVec2 &position)
    {
        s_settings.overlayPosition.x = position.x;
        s_settings.overlayPosition.y = position.y;
        DebouncedSave();
    }

    // Overlay position dirty flag
    bool IsOverlayPositionDirty()
    {
        bool isDirty           = s_overlayPositionDirty;
        s_overlayPositionDirty = false; // reset after reading
        return isDirty;
    }
    void SetPreciseOverlayPosition(const ImVec2 &position)
    {
        s_settings.overlayPosition.x = position.x;
        s_settings.overlayPosition.y = position.y;
        s_overlayPositionDirty       = true;
        DebouncedSave();
    }

    // Settings load
    bool LoadSettings()
    {
        std::filesystem::path path = G::APIDefs->Paths_GetAddonDirectory(G::ADDON_NAME);
        std::filesystem::create_directories(path);

        path /= "settings.json";
        Log::Info(std::format("Loading settings from {}", path.string()).c_str());

        if (!std::filesystem::exists(path))
        {
            Log::Warning("Settings file not found; using defaults");
            SaveSettings();
            return false;
        }

        try
        {
            std::ifstream file(path);
            if (!file.is_open())
            {
                Log::Warning("Failed to open settings file; using defaults");
                SaveSettings();
                return false;
            }

            json settingsJson;
            file >> settingsJson;
            file.close();

            if (settingsJson.contains("overlay_position") && !settingsJson["overlay_position"].is_null())
            {
                const auto &pos = settingsJson["overlay_position"];
                if (pos.contains("x") && !pos["x"].is_null())
                    s_settings.overlayPosition.x = pos["x"].get<float>();
                if (pos.contains("y") && !pos["y"].is_null())
                    s_settings.overlayPosition.y = pos["y"].get<float>();
            }
        }
        catch (const std::exception &ex)
        {
            Log::Critical(std::format("Failed to load settings: {}", ex.what()).c_str());
            return false;
        }

        return true;
    }

    // Settings save
    bool SaveSettings()
    {
        Log::Info("Saving shit");
        std::lock_guard<std::mutex> lock(s_SettingsMutex);

        std::filesystem::path path = G::APIDefs->Paths_GetAddonDirectory(G::ADDON_NAME);
        std::filesystem::create_directories(path);

        path /= "settings.json";
        Log::Info(std::format("Saving settings to {}", path.string()).c_str());

        try
        {
            json j;
            j["overlay_position"]["x"] = s_settings.overlayPosition.x;
            j["overlay_position"]["y"] = s_settings.overlayPosition.y;

            std::ofstream file(path, std::ios::trunc);
            if (!file.is_open())
            {
                Log::Critical("Failed to open settings file for writing");
                return false;
            }

            file << j.dump(4);
            file.close();
        }
        catch (const std::exception &ex)
        {
            Log::Critical(std::format("Failed to save settings: {}", ex.what()).c_str());
            return false;
        }

        return true;
    }
} // namespace SettingsManager
