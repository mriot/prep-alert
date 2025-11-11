#pragma once

#include <Common/Types.h>
#include <imgui/imgui.h>

namespace SettingsManager
{
    bool LoadSettings();

    bool SaveSettings();

    // default buff reminder timeout
    int GetDefaultBuffReminderTimeout();

    void SetDefaultBuffReminderTimeout(const int seconds);

    // image size
    int GetImageSize();

    void SetImageSize(const int size);

    // compact mode
    bool IsCompactMode();

    void SetCompactMode(bool compact);

    // horizontal mode
    bool IsHorizontalMode();

    void SetHorizontalMode(bool horizontal);

    // overlay drag
    bool IsOverlayDragEnabled();

    void SetOverlayDragEnabled(bool enabled);

    // overlay position
    ImVec2 GetOverlayPosition();

    void SetOverlayPosition(const ImVec2 &position);

    void ResetSettings();

    // overlay position dirty flag
    bool IsOverlayPositionDirty();

    void SetPreciseOverlayPosition(const ImVec2 &position);

    // flashing duration
    int GetFlashingDuration();

    void SetFlashingDuration(int seconds);

    // shown buffs
    ShownBuffTypes GetShownBuffTypes();

    void SetShownBuffTypes(const ShownBuffTypes &types);

} // namespace SettingsManager
