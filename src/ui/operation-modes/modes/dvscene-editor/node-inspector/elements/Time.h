#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::TIME>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementTime::Description*>(element);
		changed |= Editor("Curve Enabled", data->curveEnabled);
        changed |= Editor("Time", data->time);
        if(data->curveEnabled)
            changed |= Editor("Finish Time", data->finishTime);
        return changed;
    }
}
