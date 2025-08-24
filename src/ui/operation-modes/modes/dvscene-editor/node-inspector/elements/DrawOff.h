#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::DRAW_OFF>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementDrawOff::Description*>(element);
        changed |= CheckboxFlags("Visible", data->flags, hh::dv::DvElementDrawOff::Description::Flags::VISIBLE);
        changed |= CheckboxFlags("Ignore End", data->flags, hh::dv::DvElementDrawOff::Description::Flags::IGNORE_END);
        return changed;
    }
}
