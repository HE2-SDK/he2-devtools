#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::CAMERA_PARAMS>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementCameraParams::Description*>(element);
        changed |= CheckboxFlags("Enabled Position", data->flags, hh::dv::DvElementCameraParams::Description::Flags::POSITION);
        changed |= CheckboxFlags("Enabled Target Position", data->flags, hh::dv::DvElementCameraParams::Description::Flags::TARGET_POSITION);
        changed |= CheckboxFlags("Enabled FOV", data->flags, hh::dv::DvElementCameraParams::Description::Flags::FOV);
        changed |= CheckboxFlags("Enabled Rotation", data->flags, hh::dv::DvElementCameraParams::Description::Flags::ROTATION);
        changed |= CheckboxFlags("Enabled Finish Position", data->flags, hh::dv::DvElementCameraParams::Description::Flags::FINISH_POSITION);
        changed |= CheckboxFlags("Enabled Finish Target Position", data->flags, hh::dv::DvElementCameraParams::Description::Flags::FINISH_TARGET_POSITION);
        changed |= CheckboxFlags("Enabled Finish FOV", data->flags, hh::dv::DvElementCameraParams::Description::Flags::FINISH_FOV);
        changed |= CheckboxFlags("Enabled Finish Rotation", data->flags, hh::dv::DvElementCameraParams::Description::Flags::FINISH_ROTATION);
        changed |= Editor("Camera Parameters", data->camera, data->flags);
        changed |= Editor("Finish Camera Parameters", data->finishCamera, data->flags, true);
        return changed;
    }
}
