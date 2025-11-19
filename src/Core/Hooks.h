#pragma once
#include "Util/Hook.h"

extern FUNC_HOOKCREATE HookCreate;
extern FUNC_HOOKREMOVE HookRemove;
extern FUNC_HOOKENABLE HookEnable;
extern FUNC_HOOKDISABLE HookDisable;

namespace Hooks
{
    void Init();

    void Destroy();
}
