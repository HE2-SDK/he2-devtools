#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::DENSITY_SECTOR_POINT>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementDensitySectorPoint::Description*>(element);
        changed |= Editor("Unk0", data->unk0);
        changed |= Editor("Unk1", data->unk1);
        changed |= Editor("Unk2", data->unk2);
        return changed;
    }
}
