#include "Addon.h"

#include "Hooks.h"
#include <filesystem>
#include <Common/Globals.h>
#include <Common/Utils.h>
#include <Core/Events.h>
#include <Data/MapData.h>
#include <Data/SettingsManager.h>
#include <gw2re/Util/Validation.h>
#include <Controller/Controller.h>
#include <UI/Settings/Settings.h>
#include <format>
#include <imgui/imgui.h>
#include <mumble/Mumble.h>
#include <nexus/Nexus.h>

void AddonLoad(AddonAPI_t *api)
{
    G::APIDefs    = api; // this is the first thing to do (!)
    G::MumbleLink = static_cast<Mumble::Data *>(G::APIDefs->DataLink_Get(DL_MUMBLE_LINK));
    G::NexusLink  = static_cast<NexusLinkData_t *>(G::APIDefs->DataLink_Get(DL_NEXUS_LINK));

#ifdef DEBUG
    Log::Debug("!!! Addon loading in DEBUG mode !!!");
#endif

    if (!G::NexusLink)
    {
        Log::Critical("Failed to retrieve Nexus link");
        return;
    }

    if (!G::MumbleLink)
    {
        Log::Critical("Failed to retrieve Mumble link");
        return;
    }

    if (const std::string patterns = GW2RE::RunDiag(); !patterns.empty())
    {
        Log::Critical("GW2RE pattern validation failed:\n" + patterns);
        return;
    }

    if (!SettingsManager::LoadSettings())
    {
        Log::Critical("Failed to load settings! Try deleting the settings file " + SettingsManager::GetSettingsPath().string());
        return;
    }

    if ((G::MapDataMap = LoadMapData()).empty())
    {
        Log::Critical("Failed to load map data");
        return;
    }

    // --- passed all checks - let's go ---

    Hooks::Init();

    ImGui::SetCurrentContext((ImGuiContext *)G::APIDefs->ImguiContext);
    ImGui::SetAllocatorFunctions((void *(*)(size_t, void *))G::APIDefs->ImguiMalloc,
                                 (void (*)(void *, void *))G::APIDefs->ImguiFree);

    for (const auto &[id, data] : G::MapDataMap)
    {
        G::SupportedMaps.insert(id);
    }

    Log::Info((std::format("{} supported maps", G::SupportedMaps.size())));

    G::APIDefs->GUI_Register(RT_Render, OnRender);
    G::APIDefs->GUI_Register(RT_OptionsRender, OnOptionsRender);
    G::APIDefs->Events_Subscribe(EV_MUMBLE_IDENTITY_UPDATED, OnMumbleIdentityUpdated);

#ifdef DEBUG
    G::APIDefs->InputBinds_RegisterWithString("KB_COORD_DUMPER", CoordDumper, "CTRL+B");
#endif
}

/* -------------------------------------------------------------------------- */
/*                                ADDON UNLOAD                                */
/* -------------------------------------------------------------------------- */
void AddonUnload()
{
    Hooks::Destroy();
    G::APIDefs->GUI_Deregister(OnRender);
    G::APIDefs->GUI_Deregister(OnOptionsRender);
    G::APIDefs->Events_Unsubscribe(EV_MUMBLE_IDENTITY_UPDATED, OnMumbleIdentityUpdated);

#ifdef DEBUG
    G::APIDefs->InputBinds_Deregister("KB_COORD_DUMPER");
#endif
}
