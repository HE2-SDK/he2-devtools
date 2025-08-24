#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::CAMERA_SHAKE_LOOP>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementCameraShakeLoop::Description*>(element);
		changed |= Editor("Pattern", data->pattern);
		changed |= Editor("Enabled", data->enabled);
        if(data->enabled){
            changed |= Editor("Frequency", data->frequency);
            changed |= Editor("Unk4", data->unk4);
        }
        return changed;
    }
}
