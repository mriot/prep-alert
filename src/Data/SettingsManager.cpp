#include "SettingsManager.h"
#include <imgui/imgui.h>

namespace
{
    bool s_overlayDragEnabled   = false;
    int s_overlayTimeoutSeconds = 30;
    ImVec2 s_overlayPosition    = {70, 40};
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
} // namespace SettingsManager

