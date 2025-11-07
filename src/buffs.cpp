#include <Common/Utils.h>
#include <External/gw2re/Game/Char/ChBuffMgr.h>
#include <External/gw2re/Game/Char/ChCliContext.h>
#include <External/gw2re/Game/Char/ChCombatant.h>
#include <External/gw2re/Game/Char/Character.h>
#include <External/gw2re/Game/Char/EChBuffType.h>
#include <External/gw2re/Game/PropContext.h>
#include <format>

void BuffTest()
{
    static GW2RE::CPropContext propCtx = GW2RE::CPropContext::Get();

    GW2RE::CCharCliContext cctx = propCtx.GetCharCliCtx();

    GW2RE::CCharacter character = cctx.GetControlledCharacter();
    GW2RE::CCombatant combatant = character.GetCombatant();
    GW2RE::CBuffMgr buffMgr     = combatant.GetBuffMgr();
    GW2RE::BuffBar_t buffBar    = buffMgr.GetBuffBar();

    for (size_t i = 0; i < buffBar.Capacity; i++)
    {
        if (buffBar.Entries[i].Hash == 0)
            continue;

        if (buffBar.Entries[i].KVP.Value->EffectID == GW2RE::EBuffType::EFFECT_SHARPENING_STONE)
        {
            Log::Info("Got Sharpening Stone");
        }
        if (buffBar.Entries[i].KVP.Value->EffectID == GW2RE::EBuffType::EFFECT_FLAME_LEGION_SLAYING)
        {
            Log::Info("Got Flame Legion Slaying");
        }
        else
        {
            Log::Info("Empty buff slot");
        }
    }
}
