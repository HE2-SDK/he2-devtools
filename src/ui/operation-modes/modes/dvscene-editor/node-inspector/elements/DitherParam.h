#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::DITHER_PARAM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementDitherParam::Description*>(element);
        changed |= Editor("Grass Dither Start", data->grassDitherStart);
        changed |= Editor("Grass Dither End", data->grassDitherEnd);
        return changed;
    }
}
