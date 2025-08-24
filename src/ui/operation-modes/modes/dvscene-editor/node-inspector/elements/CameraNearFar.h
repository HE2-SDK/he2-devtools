#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::CAMERA_NEAR_FAR>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementCameraNearFar::Description*>(element);
        changed |= CheckboxFlags("Disable Near Clip", data->flags, hh::dv::DvElementCameraNearFar::Description::Flags::DISABLED_NEAR_CLIP);
        changed |= CheckboxFlags("Disable Far Clip", data->flags, hh::dv::DvElementCameraNearFar::Description::Flags::DISABLED_FAR_CLIP);
        if(!data->flags.test(hh::dv::DvElementCameraNearFar::Description::Flags::DISABLED_NEAR_CLIP))
            changed |= Editor("Near Clip", data->nearClip);
        if(!data->flags.test(hh::dv::DvElementCameraNearFar::Description::Flags::DISABLED_FAR_CLIP))
            changed |= Editor("Far Clip", data->farClip);
        return changed;
    }
}
