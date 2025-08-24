#pragma once
#include "../Elements.h"

namespace ui::operation_modes::modes::dvscene_editor {
    template<>
    bool RenderElementInspector<hh::dv::DvNodeElement::ElementID::VISIBILITY_ANIM>(char* element) {
        bool changed = false;
        auto* data = reinterpret_cast<hh::dv::DvElementVisibilityAnim::Description*>(element);
		changed |= Editor("Visibility Anim Name", data->visAnimName);
		changed |= Editor("Speed", data->speed);
        return changed;
    }
}
