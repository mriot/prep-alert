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
#include <Data/SettingsSerialization.h>

using json = nlohmann::json;

namespace
{
    static std::mutex settingsMutex;
    static constexpr Settings defaultSettings{
        .position = {85.0f, 33.0f},
        .compact = false,
        .horizontal = false,
        .flashDuration = 10,
        .defaultBuffTimeout = 30,
        .imageSize = 32,
        .shownBuffTypes = {
            .food = true,
            .utility = true,
            .sigil = true,
        }
    };
    static Settings settings = defaultSettings;

    // volatile state (not persisted)
    bool overlayDragEnabled   = false;
    bool overlayPositionDirty = false;

    std::filesystem::path GetSettingsPath()
    {
        const std::filesystem::path dir = G::APIDefs->Paths_GetAddonDirectory(G::ADDON_NAME);
        std::filesystem::create_directories(dir);
        return dir / "settings.json";
    }

    void DebouncedSave()
    {
        static std::atomic<uint64_t> token{0};
        unsigned long long threadToken = ++token; // invalidate previous tokens

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
    // Default buff reminder timeout
    int GetDefaultBuffReminderTimeout()
    {
        return settings.defaultBuffTimeout;
    }

    void SetDefaultBuffReminderTimeout(const int seconds)
    {
        settings.defaultBuffTimeout = seconds;
        DebouncedSave();
    }

    // Image size
    int GetImageSize()
    {
        return settings.imageSize;
    }

    void SetImageSize(const int size)
    {
        settings.imageSize = size;
        DebouncedSave();
    }

    // Compact mode
    bool IsCompactMode()
    {
        return settings.compact;
    }

    void SetCompactMode(const bool compact)
    {
        settings.compact = compact;
        DebouncedSave();
    }

    // Horizontal mode
    bool IsHorizontalMode()
    {
        return settings.horizontal;
    }

    void SetHorizontalMode(const bool horizontal)
    {
        settings.horizontal = horizontal;
        DebouncedSave();
    }

    // Shown buff types
    ShownBuffTypes GetShownBuffTypes()
    {
        return settings.shownBuffTypes;
    }

    void SetShownBuffTypes(const ShownBuffTypes &types)
    {
        settings.shownBuffTypes.food    = types.food;
        settings.shownBuffTypes.utility = types.utility;
        settings.shownBuffTypes.sigil   = types.sigil;
        DebouncedSave();
    }

    // Flashing duration
    int GetFlashingDuration()
    {
        return settings.flashDuration;
    }

    void SetFlashingDuration(const int seconds)
    {
        settings.flashDuration = seconds;
        DebouncedSave();
    }

    // Overlay drag
    bool IsOverlayDragEnabled()
    {
        return overlayDragEnabled;
    }

    void SetOverlayDragEnabled(const bool enabled)
    {
        overlayDragEnabled = enabled;
    }

    // Overlay position
    ImVec2 GetOverlayPosition()
    {
        return ImVec2(settings.position.x, settings.position.y);
    }

    void SetOverlayPosition(const ImVec2 &position)
    {
        settings.position.x = position.x;
        settings.position.y = position.y;
        DebouncedSave();
    }

    // Overlay position dirty flag
    bool IsOverlayPositionDirty()
    {
        const bool isDirty   = overlayPositionDirty;
        overlayPositionDirty = false; // reset after reading
        return isDirty;
    }

    // Precise overlay position
    void SetPreciseOverlayPosition(const ImVec2 &position)
    {
        settings.position.x  = position.x;
        settings.position.y  = position.y;
        overlayPositionDirty = true;
        DebouncedSave();
    }

    // ------------------------------

    // Reset settings
    void ResetSettings()
    {
        settings             = defaultSettings;
        overlayPositionDirty = true;
        DebouncedSave();
    }

    // ------------------------------

    // Settings load
    bool LoadSettings()
    {
        const std::filesystem::path path = GetSettingsPath();
        Log::Info(std::format("Loading settings from {}", path.string()));

        try
        {
            std::ifstream file(path);
            json settingsJson;
            file >> settingsJson;
            file.close();

            std::lock_guard<std::mutex> lock(settingsMutex);
            settings = settingsJson.get<Settings>();
        }
        catch (const std::exception &ex)
        {
            Log::Critical(std::format("Failed to load settings: {}", ex.what()));
            return false;
        }

        return true;
    }

    // Settings save
    bool SaveSettings()
    {
        std::lock_guard<std::mutex> lock(settingsMutex);

        const std::filesystem::path path = GetSettingsPath();
        Log::Info(std::format("Saving settings to {}", path.string()));

        try
        {
            json j;
            j = settings;

            std::ofstream file(path, std::ios::trunc);

            file << j.dump(4);
            file.close();
        }
        catch (const std::exception &ex)
        {
            Log::Critical(std::format("Failed to save settings: {}", ex.what()));
            return false;
        }

        return true;
    }
} // namespace SettingsManager
