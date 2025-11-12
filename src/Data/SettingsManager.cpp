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
    std::mutex settingsMutex;

    Settings defaultSettings{
        .position = {85.0f, 33.0f},
        .compact = false,
        .tooltips = false,
        .horizontal = false,
        .flashDuration = 10,
        .imageSize = 40,
        .shownBuffTypes = {
            .food = true,
            .utility = true,
            .sigil = true,
        }
    };
    Settings settings = defaultSettings;

    // volatile state (not persisted)
    bool overlayDragEnabled   = false;
    bool overlayPositionDirty = false;


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


/// ----------------------------------------------------------------------------------------------------
/// SETTINGS SERIALIZATION
/// ----------------------------------------------------------------------------------------------------

// shown buff types
void to_json(json &j, const ShownBuffTypes &t)
{
    j = {{"food", t.food}, {"utility", t.utility}, {"sigil", t.sigil}};
}

void from_json(const json &j, ShownBuffTypes &t)
{
    t.food    = j.value("food", true);
    t.utility = j.value("utility", true);
    t.sigil   = j.value("sigil", true);
}

// overlay position
void to_json(json &j, const Position &p)
{
    j = {{"x", p.x}, {"y", p.y}};
}

void from_json(const json &j, Position &p)
{
    p.x = j.value("x", 0.0f);
    p.y = j.value("y", 0.0f);
}

// settings main
void to_json(json &j, const Settings &s)
{
    j = {
        {"position", s.position},
        {"compact", s.compact},
        {"tooltips", s.tooltips},
        {"horizontal", s.horizontal},
        {"flash_duration", s.flashDuration},
        {"image_size", s.imageSize},
        {"shown_buffs", s.shownBuffTypes}
    };
}

void from_json(const json &j, Settings &s)
{
    s.position       = j.value("position", json(defaultSettings.position)).get<Position>();
    s.compact        = j.value("compact", defaultSettings.compact);
    s.tooltips       = j.value("tooltips", defaultSettings.tooltips);
    s.horizontal     = j.value("horizontal", defaultSettings.horizontal);
    s.flashDuration  = j.value("flash_duration", defaultSettings.flashDuration);
    s.imageSize      = j.value("image_size", defaultSettings.imageSize);
    s.shownBuffTypes = j.value("shown_buffs", json(defaultSettings.shownBuffTypes)).get<ShownBuffTypes>();
}


/// ----------------------------------------------------------------------------------------------------
/// SETTINGS MANAGER
/// ----------------------------------------------------------------------------------------------------

namespace SettingsManager
{
    // Image size
    int GetImageSize() { return settings.imageSize; }

    void SetImageSize(const int size)
    {
        settings.imageSize = size;
        DebouncedSave();
    }

    // Compact mode
    bool IsCompactMode() { return settings.compact; }

    void SetCompactMode(const bool compact)
    {
        settings.compact = compact;
        DebouncedSave();
    }

    // Tooltips
    bool IsTooltipsEnabled() { return settings.tooltips; }

    void SetTooltipsEnabled(const bool enabled)
    {
        settings.tooltips = enabled;
        DebouncedSave();
    }

    // Horizontal mode
    bool IsHorizontalMode() { return settings.horizontal; }

    void SetHorizontalMode(const bool horizontal)
    {
        settings.horizontal = horizontal;
        DebouncedSave();
    }

    // Shown buff types
    ShownBuffTypes GetShownBuffTypes() { return settings.shownBuffTypes; }

    void SetShownBuffTypes(const ShownBuffTypes &types)
    {
        settings.shownBuffTypes.food    = types.food;
        settings.shownBuffTypes.utility = types.utility;
        settings.shownBuffTypes.sigil   = types.sigil;
        DebouncedSave();
    }

    // Flashing duration
    int GetFlashingDuration() { return settings.flashDuration; }

    void SetFlashingDuration(const int seconds)
    {
        settings.flashDuration = seconds;
        DebouncedSave();
    }

    // Overlay drag
    bool IsOverlayDragEnabled() { return overlayDragEnabled; }

    void SetOverlayDragEnabled(const bool enabled)
    {
        overlayDragEnabled = enabled;
    }

    // Overlay position
    ImVec2 GetOverlayPosition() { return {settings.position.x, settings.position.y}; }

    void SetOverlayPosition(const ImVec2 &position)
    {
        settings.position.x = position.x;
        settings.position.y = position.y;
        DebouncedSave();
    }

    void SetPreciseOverlayPosition(const ImVec2 &position)
    {
        settings.position.x  = position.x;
        settings.position.y  = position.y;
        overlayPositionDirty = true;
        DebouncedSave();
    }

    // Overlay position dirty flag
    bool IsOverlayPositionDirty()
    {
        const bool isDirty   = overlayPositionDirty;
        overlayPositionDirty = false; // reset after reading
        return isDirty;
    }


    /// ----------------------------------------------------------------------------------------------------
    /// SETTINGS RESET / LOAD / SAVE
    /// ----------------------------------------------------------------------------------------------------

    std::filesystem::path GetSettingsPath()
    {
        const std::filesystem::path dir = G::APIDefs->Paths_GetAddonDirectory(G::ADDON_NAME);
        std::filesystem::create_directories(dir);
        return dir / "settings.json";
    }

    void ResetSettings()
    {
        settings             = defaultSettings;
        overlayPositionDirty = true;
        DebouncedSave();
    }


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
