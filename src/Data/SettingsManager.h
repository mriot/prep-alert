#pragma once

#include <filesystem>
#include <Common/Types.h>
#include <imgui/imgui.h>

namespace SettingsManager
{
    bool LoadSettings();

    bool SaveSettings();

    void ResetSettings();

    void ResetPosition();

    std::filesystem::path GetSettingsPath();

    // image size
    int GetImageSize();

    void SetImageSize(int size);

    // compact mode
    bool IsCompactMode();

    void SetCompactMode(bool compact);

    // tooltips
    bool IsTooltipsEnabled();

    void SetTooltipsEnabled(bool enabled);

    // horizontal mode
    bool IsHorizontalMode();

    void SetHorizontalMode(bool horizontal);

    // icon first
    bool IsIconFirst();

    void SetIconFirst(bool iconFirst);

    // overlay position
    ImVec2 GetOverlayPosition();

    void SetOverlayPosition(const ImVec2 &position);

    // window anchor
    Pivot GetWindowAnchor();

    void SetWindowAnchor(const Pivot &pivot);

    // overlay pivot
    Pivot GetOverlayOrigin();

    void SetOverlayOrigin(const Pivot &pivot);

    // anchor origin sync
    bool IsAnchorOriginSync();

    void SetAnchorOriginSync(bool sync);

    // flashing duration
    int GetFlashingDuration();

    void SetFlashingDuration(int seconds);

    // reminders
    MapTypeReminder GetReminder(Continent continent);

    Reminders GetReminders();

    void SetReminders(const Reminders &reminders);

    // debug window
    bool IsDebugWindowEnabled();

    void SetDebugWindowEnabled(bool enabled);

}
