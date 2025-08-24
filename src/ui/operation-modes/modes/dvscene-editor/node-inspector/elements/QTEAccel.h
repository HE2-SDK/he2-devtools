#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    const char* QTEAccelButtonNames[]{
        "A",
        "B",
        "X",
        "Y"
    };

    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::QTE_ACCEL>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementQTEAccel::Description*>(element);
        changed |= ComboEnum("Button", data->button, QTEAccelButtonNames);
		changed |= Editor("Unk1", data->unk1);
		changed |= Editor("Unk2", data->dword8);
		changed |= Editor("Unk3", data->dwordC);
        changed |= Editor("Sound Name", data->soundName);
        return changed;
    }
}
