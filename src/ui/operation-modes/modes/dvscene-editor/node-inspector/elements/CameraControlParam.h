#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
#ifdef DEVTOOLS_TARGET_SDK_rangers
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::CAMERA_CONTROL_PARAM>(char* element) {
#elif DEVTOOLS_TARGET_SDK_miller
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::CAMERA_CONTROL>(char* element) {
#endif
        bool changed = false;
        auto* data = reinterpret_cast<app::dv::DvElementCameraControlParam::Description*>(element);
		changed |= Editor("Exposure Value", data->exposureValue);
        return changed;
    }
}
