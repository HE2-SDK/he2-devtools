#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::SHADOW_RESOLUTION>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementShadowResolution::Description*>(element);
		changed |= Editor("Resolution Width", data->width);
		changed |= Editor("Resolution Height", data->height);
        return changed;
    }
}
