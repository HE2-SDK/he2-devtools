#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::SUN>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementSun::Description*>(element);
		changed |= Editor("Curve Enabled", data->curveEnabled);
        changed |= Editor("Rotation", data->rotation);
        if(data->curveEnabled)
            changed |= Editor("Finish Rotation", data->finishRotation);
        return changed;
    }
}
