#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
#ifdef DEVTOOLS_TARGET_SDK_rangers
    const char* bossNames[] = {
        "GIANT",
        "DRAGON",
        "KNIGHT",
        "RIFLE",
        "THEEND",
        "RIFLEBEAST"
    };
#elif DEVTOOLS_TARGET_SDK_miller
    const char* bossNames[] = {
        "BIOLIZARD",
        "METAL_OVERLORD",
        "MEPHILES",
        "DEVIL_DOOM",
        "PERFECT_BLACK_DOOM"
    };
#endif

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::BOSS_NAME>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementBossName::Description*>(element);
        return ComboEnum("Boss Name", data->bossId, bossNames);
    }
}
