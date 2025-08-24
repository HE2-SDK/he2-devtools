#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::LOOK_AT_IK>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementLookAtIK::Description*>(element);
		changed |= Editor("Curve Enabled", data->curveEnabled);
        changed |= Editor("Object", data->obj);
        if(data->curveEnabled)
            changed |= Editor("Finish Object", data->finishObj);
        return changed;
    }
}
