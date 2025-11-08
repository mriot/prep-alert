#include "SettingsManager.h"
#include <imgui/imgui.h>

namespace
{
    bool s_overlayDragEnabled   = false;
    int s_overlayTimeoutSeconds = 30;
    ImVec2 s_overlayPosition    = {70, 40};
    bool s_overlayPositionDirty = false;
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
        SetOverlayPosition({70, 40});
        s_overlayPositionDirty = true;
    }

    // Overlay timeout
    int GetOverlayTimeoutSeconds()
    {
        return s_overlayTimeoutSeconds;
    }
    void SetOverlayTimeoutSeconds(int seconds)
    {
        s_overlayTimeoutSeconds = seconds;
    }

    // Overlay position
    ImVec2 GetOverlayPosition()
    {
        return s_overlayPosition;
    }
    void SetOverlayPosition(const ImVec2 &position)
    {
        s_overlayPosition = position;
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
        s_overlayPosition      = position;
        s_overlayPositionDirty = true;
    }
} // namespace SettingsManager

