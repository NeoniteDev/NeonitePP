#pragma once

namespace VEH {
    bool Init();

    bool AddHook(void* Target, void* Detour);

    bool RemoveHook(void* Target);
}
