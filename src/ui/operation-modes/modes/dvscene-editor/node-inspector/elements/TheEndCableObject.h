#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* visualTypeNames[]{
        "BOSS_RIFLEBEAST_PLUG",
        "BOSS_EGGPLUG"
    };

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::THE_END_CABLE_OBJECT>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementTheEndCable::Description*>(element);
        changed |= CheckboxFlags("Enabled", data->flags, app::dv::DvElementTheEndCable::Description::Flags::ENABLED);
        changed |= CheckboxFlags("Plug Enabled", data->flags, app::dv::DvElementTheEndCable::Description::Flags::PLUG_ENABLED);
        changed |= ComboEnum("Visual Type", data->visualType, visualTypeNames);
        return changed;
    }
}
