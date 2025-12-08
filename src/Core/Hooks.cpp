#include "Hooks.h"

#include "UI/Debug/Debug.h"
#include "Util/Validation.h"
#include "aob-patterns/Patterns.h"
#include "memtools/memtools.h"
#include <format>
#include <Common/Utils.h>
#include <Common/Globals.h>
#include <string>
#include <Util/Hook.h>

FUNC_HOOKCREATE HookCreate   = nullptr;
FUNC_HOOKREMOVE HookRemove   = nullptr;
FUNC_HOOKENABLE HookEnable   = nullptr;
FUNC_HOOKDISABLE HookDisable = nullptr;

namespace Hooks
{
    using FN_SetMapFloorLevel = void(__fastcall*)(int floor_level);

    static GW2RE::Hook<FN_SetMapFloorLevel> *SetMapFloorLevelHook = nullptr;

    static void __fastcall hkSetMapFloorLevel(int floor_level)
    {
        const std::lock_guard lock(SetMapFloorLevelHook->Mutex);

        WorldState::CurrentMapFloor  = floor_level;
        Debug::Info.actualFloorLevel = floor_level;

        if (SetMapFloorLevelHook->OriginalFunction)
            SetMapFloorLevelHook->OriginalFunction(floor_level);
    }

    void Init()
    {
        HookCreate  = (FUNC_HOOKCREATE)G::APIDefs->MinHook_Create;
        HookRemove  = (FUNC_HOOKREMOVE)G::APIDefs->MinHook_Remove;
        HookEnable  = (FUNC_HOOKENABLE)G::APIDefs->MinHook_Enable;
        HookDisable = (FUNC_HOOKDISABLE)G::APIDefs->MinHook_Disable;

        std::string error;

        void *fnSetMapFloorLevel = memtools::PatternScan(PATTERNS::SET_MAP_FLOOR_LEVEL, memtools::AdvWcard(2),
                                                         memtools::Follow()).Scan();

        GW2RE::Validate(fnSetMapFloorLevel, error, "SetMapFloorLevel not found!");

        if (!error.empty())
        {
            Log::Critical(error);
            return;
        }

#ifdef DEBUG
        Log::Debug(std::format("SetMapFloorLevel() address: 0x{:X}", (uint64_t)fnSetMapFloorLevel));
#endif

        SetMapFloorLevelHook = new GW2RE::Hook(fnSetMapFloorLevel, hkSetMapFloorLevel);
        SetMapFloorLevelHook->Enable();
    }

    void Destroy()
    {
        GW2RE::Hook<FN_SetMapFloorLevel>::Destroy(SetMapFloorLevelHook);
    }
}
