#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::CAMERA_HEDGEHOG>(char* element) {
		bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementCameraHedgehog::Description*>(element);
        changed |= Editor("Flags", data->flags);
        changed |= Editor("Camera Before", data->camBefore);
        changed |= Editor("Camera After", data->camAfter);
        changed |= Editor("Unk0", data->unk0);
        return changed;
    }
}
