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
        .tooltips = true,
        .horizontal = false,
        .flashDuration = 10,
        .imageSize = 40,
        .reminders = {
            .dungeons = {
                .enabled = true,
                .food = true,
                .utility = true,
                .sigil = true,
                .sigilSlaying = true,
                .defaultBuffs = true
            },
            .fractals = {
                .enabled = true,
                .food = true,
                .utility = true,
                .sigil = true,
                .sigilSlaying = true,
                .defaultBuffs = true
            }
        },
    };
    Settings settings = defaultSettings;

    // volatile state (not persisted)
    bool overlayPositionDirty = false;
#ifdef DEBUG
    bool debugWindowEnabled = true;
#else
    bool debugWindowEnabled = false;
#endif


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

// map type reminders
void to_json(json &j, const MapTypeReminder &t)
{
    j = {
        {"enabled", t.enabled},
        {"food", t.food},
        {"utility", t.utility},
        {"sigil", t.sigil},
        {"sigil_slaying", t.sigilSlaying},
        {"default_buffs", t.defaultBuffs}
    };
}

void from_json(const json &j, MapTypeReminder &t)
{
    t.enabled      = j.value("enabled", true);
    t.food         = j.value("food", true);
    t.utility      = j.value("utility", true);
    t.sigil        = j.value("sigil", true);
    t.sigilSlaying = j.value("sigil_slaying", true);
    t.defaultBuffs = j.value("default_buffs", true);
}

// reminders
void to_json(json &j, const Reminders &r)
{
    j = {
        {"dungeons", r.dungeons},
        {"fractals", r.fractals}
    };
}

void from_json(const json &j, Reminders &r)
{
    r.dungeons = j.value("dungeons", json(defaultSettings.reminders.dungeons)).get<MapTypeReminder>();
    r.fractals = j.value("fractals", json(defaultSettings.reminders.fractals)).get<MapTypeReminder>();
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
        {"reminders", s.reminders}
    };
}

void from_json(const json &j, Settings &s)
{
    s.position      = j.value("position", json(defaultSettings.position)).get<Position>();
    s.compact       = j.value("compact", defaultSettings.compact);
    s.tooltips      = j.value("tooltips", defaultSettings.tooltips);
    s.horizontal    = j.value("horizontal", defaultSettings.horizontal);
    s.flashDuration = j.value("flash_duration", defaultSettings.flashDuration);
    s.imageSize     = j.value("image_size", defaultSettings.imageSize);
    s.reminders     = j.value("reminders", json(defaultSettings.reminders)).get<Reminders>();
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

    // Reminders
    Reminders GetReminders() { return settings.reminders; }

    void SetReminders(const Reminders &reminders)
    {
        settings.reminders = reminders;
        DebouncedSave();
    }

    // Flashing duration
    int GetFlashingDuration() { return settings.flashDuration; }

    void SetFlashingDuration(const int seconds)
    {
        settings.flashDuration = seconds;
        DebouncedSave();
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

    // debug window
    bool IsDebugWindowEnabled() { return debugWindowEnabled; }
    void SetDebugWindowEnabled(const bool enabled) { debugWindowEnabled = enabled; }


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

        if (!std::filesystem::exists(path))
        {
            Log::Warning("Settings file does not exist, using default settings");
            settings = defaultSettings;
            return true;
        }

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
}
