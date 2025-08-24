#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::CROSS_FADE>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementCrossFade::Description*>(element);
        changed |= Editor("Curve Enabled", data->curveEnabled);
        changed |= Editor("Length", data->length);
        return changed;
    }
}
