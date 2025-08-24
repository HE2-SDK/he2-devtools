#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::CONTROLLER_VIBRATION>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementControllerVibration::Description*>(element);
        changed |= CheckboxFlags("Ignore End", data->flags, hh::dv::DvElementControllerVibration::Description::Flags::IGNORE_END);
        changed |= Editor("Group Name", data->groupName);
        changed |= Editor("Vibration Name", data->vibrationName);
        return changed;
    }
}
