#include "Version.h"
#include <Common/Globals.h>
#include <Core/Addon.h>
#include <cstdint>
#include <nexus/Nexus.h>


///----------------------------------------------------------------------------------------------------
/// DLL MAIN
///----------------------------------------------------------------------------------------------------

BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        G::ModuleHandle = hModule;
    }

    return true;
}


///----------------------------------------------------------------------------------------------------
/// ADDON DEFINITION
///----------------------------------------------------------------------------------------------------

AddonDefinition_t addonDef;

extern "C" __declspec(dllexport) AddonDefinition_t *GetAddonDef()
{
    addonDef.Signature   = static_cast<uint32_t>(0xBADA55);
    addonDef.APIVersion  = NEXUS_API_VERSION;
    addonDef.Name        = G::ADDON_NAME;
    addonDef.Version     = {V_MAJOR, V_MINOR, V_BUILD, V_REVISION};
    addonDef.Author      = "Eredin.4187";
    addonDef.Description = "Location-aware damage buff reminders for Dungeons & Fractals";
    addonDef.Load        = AddonLoad;
    addonDef.Unload      = AddonUnload;
    addonDef.Flags       = AF_None;
    addonDef.Provider    = UP_GitHub;
    addonDef.UpdateLink  = "https://github.com/mriot/prep-alert";
    return &addonDef;
}
