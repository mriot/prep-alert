#pragma once

#include <Common/Types.h>
#include <imgui/imgui.h>

namespace SettingsManager
{
    bool IsOverlayDragEnabled();
    void SetOverlayDragEnabled(bool enabled);

    void ResetOverlayPosition();

    /*int GetOverlayTimeoutSeconds();
    void SetOverlayTimeoutSeconds(int seconds);*/

    ImVec2 GetOverlayPosition();
    void SetOverlayPosition(const ImVec2 &position);

    bool IsOverlayPositionDirty();
    void SetPreciseOverlayPosition(const ImVec2 &position);

    bool LoadSettings();
    bool SaveSettings();
} // namespace SettingsManager
