#pragma once
#include "Common/Types.h"

namespace Debug
{
    struct DebugInfo
    {
        int actualFloorLevel; // this is the actual floor level used for sector detection
    };

    inline DebugInfo Info;

    void RenderDebugOverlay(const std::vector<Buff> &buffReminders);
}
