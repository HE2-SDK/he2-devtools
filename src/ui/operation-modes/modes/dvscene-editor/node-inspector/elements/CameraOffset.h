#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::CAMERA_OFFSET>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementCameraOffset::Description*>(element);
        changed |= Editor("Target Offset Enabled", data->targetOffsetEnabled);
        changed |= Editor("Offset Position", data->offsetPosition);
        if (data->targetOffsetEnabled)
            changed |= Editor("Offset Target", data->offsetTarget);
        return changed;
    }
}
